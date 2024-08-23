

// Ce fichier contiendra la classe Config qui est la classe qui nous
// permettra de gérer le fichier de configuration de notre serveur web.

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "webserv.hpp"

typedef struct s_locations
{
	std::string location;
	bool 	  	allowUpload;
	std::string uploadFilePath;
    std::string	root;
	bool        autoIndex;
	std::string index;
	std::string cgiPath;
    std::string cgiExtension;
    std::map<int, std::string>	errorPages;
    std::vector<std::string>  	methods;
} t_locations;

typedef struct configParams 
{
    int         port;
    std::string host;
    bool       	defaultServer;
    unsigned long long         clientMaxBodySize; 
    
	std::vector<t_locations>	locations;

} t_configParams;

class Config
{
    public:
        Config();
        ~Config();
        Config(const Config &c);
        Config &operator=(const Config &c);

		std::vector<t_configParams> getConfigParams() const;
		int getNbServer() const;
        void parseConfig();
        void setConfigFile(std::string configFile);
    
    private:
        bool                        _error;
        std::string                 _configFile;
        std::vector<t_configParams> _configParams;
        int                         _nbServer;
        bool                        _isDefaultServerSet;
        std::fstream                _file;
        std::string                 _line;


        bool basicCheck();
        bool fillServer();
        bool fillLocation(t_configParams *server);
        bool checkLine(std::string content);
        std::string fillParam(size_t index);
        bool _checkConfig();
        void _setServerParamsToDefault(t_configParams &server);
        void _setLocationParamsToDefault(t_locations &location);
};

#endif