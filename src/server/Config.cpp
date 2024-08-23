#include "../../include/webserv.hpp"

/* ---- Canonical form ----*/

Config::Config() : _error(false), _configFile(""), _configParams(), _nbServer(0), _isDefaultServerSet(false)
{
}


Config::~Config()
{
}

Config::Config(const Config &c)
{
    *this = c;
}

Config &Config::operator=(const Config &c)
{
    if (this != &c)
    {
        this->_configFile = c._configFile;
        this->_configParams = c._configParams;
        this->_nbServer = c._nbServer;
        this->_isDefaultServerSet = c._isDefaultServerSet;
    }
    return *this;
}

/* ---- Private functions ----*/



void    Config::parseConfig() // fonction principale de parsing du fichier de config
{

    if (basicCheck() == false)
        exit(1);

    while (std::getline(_file, _line) && !_error) // main loop de lecture du fichier de config
    {
        if (_line.empty())
            continue ;
        if (_line[0] == '#')
            continue ;
        if (_line.find("server") != std::string::npos)
        {
            if (!fillServer())
                exit(1);
        }
        else
        {
            std::cerr << "Error: Invalid configuration file" << std::endl;
            exit(1);
        }
    }
    _file.close();
    if (_error)
        exit(1);
    if (!_checkConfig())
        exit(1);
}

bool Config::fillServer( void )
{
    int bracket = 0;
    if (_line.find("{") != std::string::npos)
        bracket++;
    else if (_line.find("}") != std::string::npos)
        bracket--;
    t_configParams server;
    _setServerParamsToDefault(server);
    while (bracket > 0 && std::getline(_file, _line) && !_error)
    {

        if (_line.empty())
            continue ;
        if (_line.find("{") != std::string::npos)
            bracket++;
        if (_line.find("}") != std::string::npos)
            bracket--;
        if (bracket > 1 && _line.find("location") == 0)
            return false;
        if (bracket == 0)
            break ;
        if (_line.find("port") != std::string::npos && _line.find(";") != std::string::npos)
            server.port = std::atoi(_line.substr(_line.find("port") + 5, _line.find(";") - (_line.find("port") + 5)).c_str());
        else if (_line.find("host") != std::string::npos && _line.find(";") != std::string::npos)
            server.host = _line.substr(_line.find("host") + 5, _line.find(";") - (_line.find("host") + 5));
        else if (_line.find("default_server") != std::string::npos && _line.find("true") != std::string::npos && _line.find(";") != std::string::npos)
        {
            if (this->_isDefaultServerSet == true)
            {
                std::cerr << "Error: Multiple default servers" << std::endl;
                return false;
            }
            server.defaultServer = true;
            this->_isDefaultServerSet = true;
        }
        else if (_line.find("default_server") != std::string::npos && _line.find("false") != std::string::npos && _line.find(";") != std::string::npos)
            server.defaultServer = false;
        else if (_line.find("client_max_body_size") != std::string::npos)
            server.clientMaxBodySize = std::atoi(_line.substr(_line.find("client_max_body_size") + 20).c_str());
        else if (_line.find("location") != std::string::npos)
        {
            if (!fillLocation(&server))
                return false;
            if (_line.find("{"))
                bracket--;
        }
        else
        {
            std::cout << "Error: Invalid line in .conf file: " << _line << std::endl;
            return false;
        }
    }
    if (_error)
        return false;
    if (bracket != 0)
    {
            std::cerr << "Error: Invalid configuration file 1" << std::endl;
            return false;
    }
    this->_configParams.push_back(server);
    this->_nbServer++;
    return true;
}

bool Config::fillLocation(t_configParams *server)
{
    int bracket = 0;
    t_locations location;
    _setLocationParamsToDefault(location);
    if (_line.find("location") != std::string::npos && _line.find("{") 
        != std::string::npos && _line.find("location") < _line.find("{"))
        location.location = _line.substr(_line.find("location") + 9, _line.find("{") - (_line.find("location") + 9));
    else
    {
        std::cerr << "Error: Invalid location line" << std::endl;
        return false;
    }
    if (_line.find("{"))
        bracket++;
    while (std::getline(_file, _line) && !_error)
    {
        if (_line.empty())
            continue ;
        if (bracket > 1)
            return false;
        if (_line.find("{") != std::string::npos)
            bracket++;
        if (_line.find("}") != std::string::npos)
            bracket--;
        if (bracket == 0)
        {
            server->locations.push_back(location);
            return true;
        }
        if (checkLine("allow_upload") && _line.find("true") != std::string::npos)
            location.allowUpload = true;
        else if (checkLine("allow_upload") && _line.find("false") != std::string::npos)
            location.allowUpload = false;
        else if (checkLine("upload_path"))
            location.uploadFilePath = fillParam(_line.find("upload_path") + 12);
        else if (checkLine("root"))
            location.root = fillParam(_line.find("root") + 5);
        else if (checkLine("autoindex") && _line.find("on") != std::string::npos)
            location.autoIndex = true;
        else if (checkLine("autoindex") && _line.find("off") != std::string::npos)
            location.autoIndex = false;
        else if (checkLine("index"))
            location.index = _line.substr(_line.find("index") + 6);
        else if (checkLine("cgi_path"))
            location.cgiPath = _line.substr(_line.find("cgi_path") + 9);
        else if (checkLine("cgi_extension"))
            location.cgiExtension = _line.substr(_line.find("cgi_extension") + 14);
        else if (checkLine("allow_methods"))
        {
            if (_line.find("GET") != std::string::npos)
                location.methods.push_back("GET");
            if (_line.find("POST") != std::string::npos)
                location.methods.push_back("POST");
            if (_line.find("DELETE") != std::string::npos)
                location.methods.push_back("DELETE");
        }
        else if (checkLine("error_page"))
        {
            int code = std::atoi(_line.substr(_line.find("error_page") + 11).c_str());
            location.errorPages[code] = fillParam(_line.find_last_of(" ") + 1);
        }
        else
        {
            std::cout << "Error: Invalid line in .conf file: " << _line << std::endl;
            return false;
        }
    }
    if (bracket != 0)
    {
            std::cerr << "Error: Invalid configuration file 2" << std::endl;
            return false;
    }
    if (_error)
        return false;
    server->locations.push_back(location);
    return true;
}

bool Config::basicCheck( void )
{
    if (this->_configFile.empty())
    {
        _configFile = "config_files/default.config";
    }

    _file.open(this->_configFile.c_str(), std::fstream::in);
    
    if (!_file.is_open())
    {
        std::cerr << "Error: Could not open file " << this->_configFile << std::endl;
        return false;
    }
    return true;
}

bool Config::checkLine(std::string content)
{
    if (_line.find(content) == std::string::npos)
        return false;
    size_t lineLen = _line.size();
    size_t contentLen = content.size();
    size_t beginningOfContentInLine = _line.find(content);
    if ((lineLen - beginningOfContentInLine - contentLen) > 1)
        return true;
    return false;
}

std::string Config::fillParam(size_t index)
{
    size_t end = _line.find(";", index);
    if (end == std::string::npos)
    {
        std::cerr << "Error: Missing \";\" operand at line : " << _line << std::endl;
        _error = true;
        return "";
    }
    return _line.substr(index, end - index);
}

void    Config::setConfigFile(std::string configFile)
{
    const std::string ext = ".config";
    if (configFile.length() < ext.length()) {
        std::cout << "Wrong filename, default.config will be use." << std::endl;
        return ;
    }
    if (configFile.compare(configFile.length() - ext.length(), ext.length(), ext) == 0)
    {
        _configFile = configFile;
        return ;
    }
    std::cout << "Wrong filename, default.config will be use." << std::endl;
}


/* ---- Public functions ----*/



std::vector<t_configParams> Config::getConfigParams() const
{
    return this->_configParams;
}

int Config::getNbServer() const
{
    return this->_nbServer;
}

void    Config::_setServerParamsToDefault(t_configParams &server)
{
    server.clientMaxBodySize = 0;
    server.port = 0;
    server.defaultServer = false;
}
void    Config::_setLocationParamsToDefault(t_locations &location)
{
    location.allowUpload = false;
    location.autoIndex = false;
}

bool    Config::_checkConfig()
{
    bool isDefaultServerSet;
    if (_configParams.empty())
        return false;
    for (size_t i = 0; i < _configParams.size(); i++)
    {
        if (_configParams[i].host.empty())
        {
            std::cerr << "Error: Configuration file: host is missing" << std::endl;
            return false;
        }
        if (_configParams[i].clientMaxBodySize <= 0 || _configParams[i].port <= 0)
        {
            std::cerr << "Error: Configuration file: missing elements or negative value on server : " << _configParams[i].host << std::endl;
            return false;
        }
        if (_configParams[i].defaultServer == true)
            isDefaultServerSet = true;
    }
    if (isDefaultServerSet == false)
        _configParams[0].defaultServer = true;
    
    return true;
}

