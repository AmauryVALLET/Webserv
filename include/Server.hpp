
// Ce fichier contient la classe Server qui a pour but de creer et gerer un serveur web.

#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.hpp"
#include <poll.h>

class Poll;

class Server
{
public:
    Server();
    ~Server();
    Server(const Server &s);
    Server &operator=(const Server &s);

    Server(t_configParams &configParams);

    bool            startServer();
    void            stopServer();
    int             getServerFD() const;
    t_configParams  getServerConfig() const;
    
private:
    uint32_t _getHost(std::string host);

    int                 _serverFD;
    struct sockaddr_in  _serverAddr;
    t_configParams      _configParam;
};


#endif