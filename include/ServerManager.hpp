

// Ce fichier contient la classe ServerManager qui sera la classe principale de notre projet
// Elle prendra en parametre un objet Configuration et pourra lancer plusieurs serveur si besoin


#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "webserv.hpp"
#include <poll.h>

# define MAX_EVENTS 100000
# define BUFFER_SIZE 100000

class Server;


typedef struct s_client {
    t_configParams      serverInfo;
    unsigned long long  bytesRead;             
}   t_client;   

class ServerManager
{
public:
    ServerManager();
    ~ServerManager();
    ServerManager(const ServerManager &s);
    ServerManager &operator=(const ServerManager &s);

    ServerManager(Config &config);

    void startServers();

private:
    bool        isRunning();
    bool        _initServers();
    void        _stopServers();
    int         _isOneOfListeningServerFD(int fd);
    t_configParams getServerConfig(int fd);
    void        _acceptNewConnection(int ServerFd);
    void        _handleClientEvent(struct epoll_event event);
    void        _closeConnection( int clientFD );
    void        _cleanConnection( int clientFD );
    void        _cleanChunkedRequest( int clientFD );

    bool        _readRequest(struct epoll_event event);
    void        _sendResponse(struct epoll_event event);

    void        _addBufferToRequest(char buffer[BUFFER_SIZE], int clientFD, int bufferLen);
    void        _createRequestObject(int clientFD, std::string httpRequest);
    bool        _createResponseObject(int clientFD);
    bool        _sendResponseToClient(int clientFD);
    std::string _processChunkedRequest(std::string fullRequest);
    
    std::vector<Server *>       _servers;
    std::vector<epoll_event>    _epoll_servers;
    std::vector<int>            _clientList;
    std::map<int, t_client>     _clientInfo;

    std::map<int, bool>                 _clientRequestReceive;
    std::map<int, std::string>          _request_map; //every clients will have string dedicate to him to save the request
    std::map<int, Request>              _request_objects;
    std::map<int, std::string>          _response_objects;

    struct epoll_event         _epollEvents[MAX_EVENTS];
    int     _epollFD;
    bool    _isRunning;
    int     _nbServers;
    int     _nbEvents;
};

#endif