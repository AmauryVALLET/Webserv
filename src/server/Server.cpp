#include "../../include/webserv.hpp"

Server::Server()
{
}

Server::~Server()
{

}

Server::Server(const Server &s)
{
    *this = s;
}

Server &Server::operator=(const Server &s) // Ne pas oublier de rajouter le reste des attributs au fur et a mesure
{
    if (this != &s)
    {
        _serverFD = s._serverFD;
        _serverAddr = s._serverAddr;
        _configParam = s._configParam;
    }
    return *this;
}

Server::Server(t_configParams &configParams)
{
    _configParam = configParams;
}

bool Server::startServer()
{   
\
    std::string ip(_configParam.host);
    _serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFD < 0)
    {
        std::cerr << "Error: socket creation failed" << std::endl;
        return false;
    }

    int flags = fcntl(_serverFD, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "Error: fcntl F_GETFL" << std::endl;
        return false;
    }

    if (fcntl(_serverFD, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::cerr << "Error: fcntl F_SETFL" << std::endl;
        return false;
    }

    if (_serverFD < 0)
    {
        std::cerr << "Error: socket creation failed" << std::endl;
        return false;
    }

    int opt = 1;
    if (setsockopt(_serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "Error: setsockopt failed" << std::endl;
        return false;
    }

    struct in_addr addr;
    if (inet_pton(AF_INET, ip.c_str(), &addr) <= 0) {
        std::cerr << "Error: invalid IP address" << std::endl;
        return false;
    }

    memset((char *)&_serverAddr, 0, sizeof(_serverAddr));
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_addr = addr;
    _serverAddr.sin_port = htons(_configParam.port);

    if (bind(_serverFD, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) < 0)
    {
        std::perror("bind");        
        std::cerr << "Error: bind failed" << std::endl;
        return false;
    }

    socklen_t addr_len = sizeof(_serverAddr);
    if (getsockname(_serverFD, (struct sockaddr *)&_serverAddr, &addr_len) < 0) {
        std::perror("getsockname");
        std::cerr << "Error: getsockname failed" << std::endl;
        return false;
    }

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    if (getnameinfo((struct sockaddr *)&_serverAddr, addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
        std::cout << "\033[0;34mListening on: " << host << ":" << service << "\033[0m" << std::endl;
    } else {
        std::cerr << "Error: getnameinfo failed" << std::endl;
        return false;
    }


    if (listen(_serverFD, 10) < 0)
    {
        std::cerr << "Error: listen failed" << std::endl;
        return false;
    }
    return true;
}

void    Server::stopServer()
{
    close(_serverFD);
}

/*----- Setters and getters -----*/

int Server::getServerFD() const
{
    return _serverFD;
}

t_configParams  Server::getServerConfig() const
{
    return _configParam;
}