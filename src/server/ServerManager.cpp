#include "../../include/webserv.hpp"


ServerManager::ServerManager()
{
}

ServerManager::~ServerManager()
{
    for (size_t i = 0; i < _servers.size(); i++)
    {
        delete _servers[i];
    }
    _servers.clear();
}

ServerManager::ServerManager(const ServerManager &s)
{
    *this = s;
}

ServerManager &ServerManager::operator=(const ServerManager &s)
{
    if (this != &s)
    {
        _servers = s._servers;
        _isRunning = s._isRunning;
        _nbServers = s._nbServers;
        _epollFD = s._epollFD;
        _nbEvents = s._nbEvents;
        _clientList = s._clientList;
        _clientInfo = s._clientInfo;
        _clientRequestReceive = s._clientRequestReceive;
        _request_map = s._request_map;
        _request_objects = s._request_objects;
        _response_objects = s._response_objects;

    }
    return *this;
}

ServerManager::ServerManager(Config &config) : _nbServers(0)
{
    std::vector<t_configParams> configParams = config.getConfigParams();

    for (size_t i = 0; i < configParams.size(); i++)
    {
        Server *server = new Server(configParams[i]);
        _servers.push_back(server);
        _nbServers++;
    }
}

bool ServerManager::isRunning()
{
    if (g_stop)
        _isRunning = 0;
    return _isRunning;
}

void ServerManager::startServers()
{
    _isRunning = 1;
    if (!_initServers())
        _isRunning = 0;
    
    std::cout << std::endl << "\033[0;32m\033[4mServerManager: Starting servers\033[0m" << std::endl << std::endl;
    while (isRunning())
    {
        _nbEvents = epoll_wait(_epollFD, _epollEvents, MAX_EVENTS, -1);
        if (_nbEvents == -1)
        {
            std::cerr << "Error: epoll_wait return -1" << std::endl;
            break ;
        }

        for (int i = 0; i < _nbEvents; i++)
        {
            int listeningSocket = -1;
            if ((listeningSocket = _isOneOfListeningServerFD(_epollEvents[i].data.fd)) > 0)
            {
                _acceptNewConnection(listeningSocket);
            }
            else
            {
                _handleClientEvent(_epollEvents[i]);
            }
        }
        
    }
    std::cout << std::endl << "\033[0;31m\033[4mServerManager: Stopping servers\033[0m" << std::endl << std::endl;
    _stopServers();
}

bool ServerManager::_initServers()
{
    if (g_stop)
        return false;
    _epollFD = epoll_create1(0);
    if (_epollFD == -1)
    {
        g_stop = 1;
        return false;
    }
    for (int i = 0; i < _nbServers; i++)
    {
        if (_servers[i]->startServer() == false)
            return false;
        int serverFD = _servers[i]->getServerFD();
        
        struct epoll_event event;

        event.events = EPOLLIN | EPOLLOUT;
        event.data.fd = serverFD;

        if (epoll_ctl(_epollFD, EPOLL_CTL_ADD, serverFD, &event))
        {
            g_stop = 1;
            return false;
        }
        
    }

    return true;
}

int ServerManager::_isOneOfListeningServerFD(int fd)
{
    if (g_stop)
        return -1;
    for (size_t i = 0; i < _servers.size(); i++)
    {
        if (_servers[i]->getServerFD() == fd)
            return fd;
    }
    return -1;
}

t_configParams ServerManager::getServerConfig(int fd)
{
    t_configParams noConfigFound;
    noConfigFound.port = 0;
    if (g_stop)
        return noConfigFound;
    for (size_t i = 0; i < _servers.size(); i++)
    {
        if (_servers[i]->getServerFD() == fd)
            return _servers[i]->getServerConfig();
    }
    return noConfigFound;
}

void ServerManager::_acceptNewConnection(int serverFD)
{
    if (g_stop)
        return ;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(serverFD, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket < 0)
    {
        std::cerr << "Error: accept failed" << std::endl;
        g_stop = 1;
        return ;
    }
    int flags = fcntl(clientSocket, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "Error: fcntl F_GETFL" << std::endl;
        g_stop = 1;
        return ;
    }

    if (fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::cerr << "Error: fcntl F_SETFL" << std::endl;
        g_stop = 1;
        return ;
    }

    struct epoll_event event;

    event.events = EPOLLIN | EPOLLOUT;
    event.data.fd = clientSocket;

    if (epoll_ctl(_epollFD, EPOLL_CTL_ADD, clientSocket, &event))
    {
        g_stop = 1;
        close(clientSocket);
        return ;
    }
    t_client clientInfo;
    clientInfo.bytesRead = 0;
    clientInfo.serverInfo = getServerConfig(serverFD);
    if (clientInfo.serverInfo.port == 0) {
        std::cerr << "Error, no config found for this server" << std::endl;
        g_stop = 1;
    }
    _clientList.push_back(event.data.fd);
    _clientInfo.insert(std::make_pair(event.data.fd, clientInfo));
    _clientRequestReceive.insert(std::make_pair(event.data.fd, false));

}

void    ServerManager::_handleClientEvent(struct epoll_event event)
{
    if (g_stop)
        return ;
    if (event.events & EPOLLIN)
    {
        if (_readRequest(event) == true)
            return ;
    }
    if (event.events & EPOLLOUT)
    {
        _sendResponse(event);
    }
}


bool    ServerManager::_readRequest(struct epoll_event event)
{
    if (g_stop)
        return false;
    
    int eventFD = event.data.fd;
    if (_clientRequestReceive[eventFD] == true)
        return false;
    char buffer[BUFFER_SIZE];
    memset(&buffer, 0, BUFFER_SIZE);
    ssize_t bytesRead = recv(eventFD, buffer, BUFFER_SIZE, 0);
    if (bytesRead == 0)
    {
        _closeConnection(eventFD);
        return false;
    }
    if (bytesRead < 0)
    {
        std::cout << "Error with recv" << std::endl;
        g_stop = 1;
        return false;
    }
    _addBufferToRequest(buffer, eventFD, bytesRead);
    if (_clientRequestReceive[eventFD] == true)
        return true;
    return false;
}

void    ServerManager::_sendResponse(struct epoll_event event)
{
    if (g_stop)
        return ;

    int     eventFD = event.data.fd;
    bool    keepAlive = false;

    if (_clientRequestReceive[eventFD] == false)
        return ;

    if (_request_map.count(eventFD) > 0)
    {
        std::string httpRequest = _request_map[eventFD];
        if (_request_objects.count(eventFD) == 0)
        {
            _createRequestObject(eventFD, httpRequest);
        }
        if (_request_objects.count(eventFD) != 0)
        {
            keepAlive = _createResponseObject(eventFD);
        }
        else
            return ;

        if (_sendResponseToClient(eventFD) && !keepAlive)
        {
            _closeConnection(eventFD);
        }
        else if (keepAlive)
            _cleanConnection(eventFD);
    }
}

void ServerManager::_cleanChunkedRequest( int clientFD )
{
    if (_request_objects.count(clientFD) > 0)
        _request_objects.erase(clientFD);
    if (_response_objects.count(clientFD) > 0)
        _response_objects.erase(clientFD);

    _clientInfo[clientFD].bytesRead = 0;
    _clientRequestReceive[clientFD] = false;

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT;
    ev.data.fd = clientFD;
    epoll_ctl(_epollFD, EPOLL_CTL_MOD, clientFD, &ev);
}

void ServerManager::_cleanConnection( int clientFD )
{
    _request_map.erase(clientFD);
    _request_objects.erase(clientFD);
    _response_objects.erase(clientFD);
    _clientInfo[clientFD].bytesRead = 0;
    _clientRequestReceive[clientFD] = false;

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT;
    ev.data.fd = clientFD;
    if (epoll_ctl(_epollFD, EPOLL_CTL_MOD, clientFD, &ev))
    {
        std::cerr << "Error: epoll_ctl failed" << std::endl;
        g_stop = 1;
    }
}

void ServerManager::_closeConnection(int clientFD)
{
    if (epoll_ctl(_epollFD, EPOLL_CTL_DEL, clientFD, NULL))
    {
        g_stop = 1;
    }
    
    if (close(clientFD) == -1)
    {
        g_stop = 1;
    }


    _request_map.erase(clientFD);
    _request_objects.erase(clientFD);
    _response_objects.erase(clientFD);
    _clientList.erase(std::remove(_clientList.begin(), _clientList.end(), clientFD), _clientList.end());
    _clientInfo.erase(clientFD);
    _clientRequestReceive.erase(clientFD);

}

void    ServerManager::_addBufferToRequest(char buffer[BUFFER_SIZE], int clientFD, int bufferLen)
{
    if (g_stop)
        return ;
    int count = _request_map.count(clientFD);
    if (count == 0)
    {
        std::string request;
        _request_map.insert(std::make_pair(clientFD, request));
    }
    _clientInfo[clientFD].bytesRead += bufferLen;
    _request_map[clientFD].append(buffer, bufferLen);
    if (_request_map[clientFD].find("Transfer-Encoding: chunked") != std::string::npos)
    {
        if (checkIfIsItEnd(_request_map[clientFD], "0\r\n\r\n") == 1)
        {
            _request_map[clientFD] = _processChunkedRequest(_request_map[clientFD]);
            _clientRequestReceive[clientFD] = true;
        }
        else
            return ;
    }

    size_t CRLF = _request_map[clientFD].find("\r\n\r\n");
    if (CRLF != std::string::npos && _request_map[clientFD].find("Content-Length: ") == std::string::npos)
    {
        _clientRequestReceive[clientFD] = true;
    }
    else if (CRLF != std::string::npos && _request_map[clientFD].find("Content-Length: ") != std::string::npos)
    {
        size_t	len = std::atoi(_request_map[clientFD].substr(_request_map[clientFD].find("Content-Length: ") + 16, 10).c_str());
        if (_request_map[clientFD].size() >= CRLF + 4 + len) 
        {
            _clientRequestReceive[clientFD] = true;
        }
    }
}

void    ServerManager::_createRequestObject(int clientFD, std::string httpRequest)
{
    if (g_stop)
        return ;
    if (VERBOSE)
        std::cout << httpRequest << std::endl;
    t_configParams  clientParam = _clientInfo[clientFD].serverInfo;

    Request finalRequest;
    finalRequest.parseFromString(httpRequest, clientParam);
    if (finalRequest.isChunkedRequest())
    {
        _cleanChunkedRequest(clientFD);
        return ;
    }
    _request_objects[clientFD] = finalRequest;
}

bool    ServerManager::_createResponseObject(int clientFD)
{
    if (g_stop)
        return false;
    
    std::string     response;

    ProcessRequest  myProcess(_request_objects[clientFD]);

    response = myProcess.StartProcessRequest();

    bool statusKeepAlive = myProcess.getHeaderConnectionKeepaliveOrNot();

    if (VERBOSE)
        std::cout << "-------------\n" << response << std::endl << "-----------\n";
    _response_objects.insert(std::make_pair(clientFD, response));
    if (statusKeepAlive == true)
        return true;
    return false;
}

bool    ServerManager::_sendResponseToClient(int clientFD)
{
    std::string response = _response_objects[clientFD];
    ssize_t bytesSent = 0;
    
    bytesSent = send(clientFD, response.c_str(), response.length(), 0);

    if (bytesSent == 0) 
    {
        _closeConnection(clientFD);
        return false;
    }
    if (bytesSent < 0)
    {
        std::cout << "Error: Send fail" << std::endl;
        g_stop = 1;
    }
    return true;
}

std::string ServerManager::_processChunkedRequest(std::string fullRequest)
{
	std::string	head = fullRequest.substr(0, fullRequest.find("\r\n\r\n"));
	std::string	chunks = fullRequest.substr(fullRequest.find("\r\n\r\n") + 4, fullRequest.size() - 1);
	std::string	subchunk = chunks.substr(0, 100);
	std::string	body = "";

	int			chunksize = strtol(subchunk.c_str(), NULL, 16);
	size_t		i = 0;
    while (chunksize)
	{
		i = chunks.find("\r\n", i) + 2;
		body += chunks.substr(i, chunksize);
		i += chunksize + 2;
		subchunk = chunks.substr(i, 100);
		chunksize = strtol(subchunk.c_str(), NULL, 16);
	}
    std::string finalRequest;
	finalRequest = head + "\r\n\r\n" + body + "\r\n\r\n";
    return finalRequest;
}






void    ServerManager::_stopServers()
{
    usleep(2000000);
    for (int i = 0; i < _nbServers; i++)
    {
        _servers[i]->stopServer();
        if (epoll_ctl(_epollFD, EPOLL_CTL_DEL, _servers[i]->getServerFD(), NULL))
            close(_servers[i]->getServerFD());
    }
    int nbClient = _clientList.size();

    usleep(2000000);

    std::cout << "Closing " << nbClient << " clients" << std::endl;
    while (_clientList.size() > 0)
    {
        _closeConnection(_clientList[0]);
    }

    if (close(_epollFD) == -1)
    {
        std::cerr << "Error: close failed" << std::endl;
        g_stop = 1;
    }
}

int		checkIfIsItEnd(const std::string& httpRequest, const std::string& end)
{
    if (httpRequest.size() < end.size())
        return (0);
    if (httpRequest.compare(httpRequest.size() - end.size(), end.size(), end) == 0)
        return (1);
    return (0);
}