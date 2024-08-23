#include "../../include/webserv.hpp"

//ORTHODOX CANONICAL FORM
Request::Request()
{

	if (VERBOSE)
		std::cout << "\t(verbose) - Request - default constructor" << std::endl;
	_methodString = "";
	_pathString = "";
	_versionString = "";
	_headersVector = std::vector<std::string>();
	_headersMap = std::map<std::string, std::string>();
	_bodyString = "";
	_flag = 0;
	_configParams = t_configParams();
	_configParams.locations = std::vector<t_locations>();
	_startPathLocation = "";
	_endPathLocation = "";
	_currentLocation = t_locations();
	_isLocationSet = false;
	_currentLocation.uploadFilePath = "";
	_currentErrorPages = std::map<int, std::pair<std::string, int> >();
	_finalUploadPathIfAllowed = "";

}

Request::~Request()
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - destructor" << std::endl;
	return ;
}

Request::Request(const Request &src)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - copy constructor" << std::endl;
	*this = src;
}

Request &Request::operator=(const Request &src)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - assignation operator" << std::endl;
	if (this != &src)
	{
		_methodString = src._methodString;
		_pathString = src._pathString;
		_versionString = src._versionString;
		_headersVector = src._headersVector;
		_headersMap = src._headersMap;
		_bodyString = src._bodyString;
		_flag = src._flag;
		_configParams = src._configParams;
		_startPathLocation = src._startPathLocation;
		_endPathLocation = src._endPathLocation;
		_configParams.locations = src._configParams.locations;
		_currentLocation = src._currentLocation;
		_isLocationSet = src._isLocationSet;
		_currentLocation.uploadFilePath = src._currentLocation.uploadFilePath;
		_finalUploadPathIfAllowed = src._finalUploadPathIfAllowed;
		_currentErrorPages = src._currentErrorPages;
	}
	return (*this);
}

//SETTERS

void Request::setMethodString(std::string methodString)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting a string with the method of the request" << std::endl;
	_methodString = methodString;
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - methodString: " << _methodString << std::endl;
}

void Request::setPathString(std::string pathString)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting a string with the path of the request" << std::endl;
	_pathString = pathString;
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - pathString: " << _pathString << std::endl;
}

void Request::setVersionString(std::string versionString)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting a string with the version of the request" << std::endl;
	_versionString = versionString;
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - versionString: " << _versionString << std::endl;
}

void Request::setHeadersVector(std::vector<std::string> headersVector)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting a vector with the headers of the request" << std::endl;
	_headersVector = headersVector;
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - headersVector: ";
}

void Request::setHeadersMap(std::map<std::string, std::string> headersMap)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting a map with headers as key and header's value as value" << std::endl;
	_headersMap = headersMap;
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - headersMap: ";
}

void Request::setBodyString(std::string bodyString)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting a string with the body of the request" << std::endl;
	_bodyString = bodyString;
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - bodyString: " << _bodyString << std::endl;
}

void Request::setFlag(int flag)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting an int with the flag of the request" << std::endl;
    _flag = flag;
}

void Request::setConfigParamsLocations(std::vector<t_locations> configParamsLocations)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting the config params locations" << std::endl;
	_configParams.locations = configParamsLocations;
}

void Request::setCurrentErrorPages(std::map<int, std::pair<std::string, int> > currentErrorPages)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting the current error pages" << std::endl;
	_currentErrorPages = currentErrorPages;
}

void Request::setStartPathLocation(std::string startPathLocation)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting a string with the start path location" << std::endl;
	_startPathLocation = startPathLocation;
}

void Request::setEndPathLocation(std::string endPathLocation)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting a string with the end path location" << std::endl;
	_endPathLocation = endPathLocation;
}

void Request::setCurrentLocation(t_locations currentLocation)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting the current location" << std::endl;
	_currentLocation = currentLocation;
}

void Request::setIsLocationSet(bool isLocationSet)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting if the location is set" << std::endl;
	_isLocationSet = isLocationSet;
}

void Request::setCurrentLocUploadFilePath(std::string uploadFilePath)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting a string with the upload file path" << std::endl;
	_currentLocation.uploadFilePath = uploadFilePath;
}

void Request::setFinalUploadPathIfAllowed(std::string finalUploadPathIfAllowed)
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - setting a string with the final upload path if allowed" << std::endl;
	_finalUploadPathIfAllowed = finalUploadPathIfAllowed;
}

//GETTERS

std::string Request::getMethodString() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting a string with the method of the request" << std::endl;
	return (_methodString);
}

std::string Request::getPathString() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting a string with the path of the request" << std::endl;
	return (_pathString);
}

std::string Request::getVersionString() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting a string with the version of the request" << std::endl;
	return (_versionString);
}

std::vector<std::string> Request::getHeadersVector() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting a vector with the headers of the request" << std::endl;
	return (_headersVector);
}

std::map<std::string, std::string> Request::getHeadersMap() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting a map with headers as key and header's value as value" << std::endl;
	return (_headersMap);
}

std::string Request::getBodyString() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting a string with the body of the request" << std::endl;
	return (_bodyString);
}

int Request::getFlag() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting an int with the flag of the request" << std::endl;
    return _flag;
}

t_configParams Request::getConfigParamsStruct() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting the config params" << std::endl;
	return _configParams;
}

std::vector<t_locations> Request::getConfigParamsLocations() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting the config params locations" << std::endl;
	return _configParams.locations;
}

std::map<int, std::pair<std::string, int> > Request::getCurrentErrorPages() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting the current error pages" << std::endl;
	return _currentErrorPages;
}

std::string Request::getStartPathLocation() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting a string with the start path location" << std::endl;
	return _startPathLocation;
}

std::string Request::getEndPathLocation() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting a string with the end path location" << std::endl;
	return _endPathLocation;
}

t_locations Request::getCurrentLocation() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting the current location" << std::endl;
	return _currentLocation;
}

bool Request::getIsLocationSet() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting if the location is set" << std::endl;
	return _isLocationSet;
}

std::string Request::getCurrentLocUploadFilePath() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting a string with the upload file path" << std::endl;
	return _currentLocation.uploadFilePath;
}

std::string Request::getFinalUploadPathIfAllowed() const
{
	if (VERBOSE)
		std::cout << "\t(verbose) - Request - getting a string with the final upload path if allowed" << std::endl;
	return _finalUploadPathIfAllowed;
}

//METHOD


// Request parsing -------------------------------------------------------------------------

bool lengthURI(const std::string& uri) {
    // Vérifie que la longueur de l'URI n'est pas trop importante pour prévenir des attaques
    if (uri.length() > 2048) {
		if (DEBUG_PARSING)
	        std::cerr << "Error: URI too long" << std::endl;
        return false;
    }
    return true;
}

bool validateURI(const std::string& uri) {

    // on valide que ces char pour empecher code malveillant (ex injections SQL ou scripts XSS)
    for (std::string::const_iterator it = uri.begin(); it != uri.end(); ++it) {
        char c = *it;
        if (!(isalnum(c) || c == '-' || c == '_' || c == '.' || c == '/' || c == '%')) {
			if (DEBUG_PARSING)
	            std::cerr << "Error: Invalid character in URI" << std::endl;
            return false;
        }
    }

    // pour gerer les acces non auto de directories
    if (uri.find("../") != std::string::npos || uri.find("..\\") != std::string::npos) {
		if (DEBUG_PARSING)
	        std::cerr << "Error: Directory traversal attempt detected" << std::endl;
        return false;
    }

    return true;
}

void Request::clear() {
    _methodString.clear();
    _pathString.clear();
    _versionString.clear();
    _headersVector.clear();
    _headersMap.clear();
    _bodyString.clear();
    _flag = 0;

	_methodString = "GET";
	_pathString = "wrong.html";
	_versionString = "HTTP/1.1";
    _headersVector.push_back("error-Type: text/html");
    _headersMap["error-type"] = "error/html";
	_bodyString = "TEST";
}

void Request::parseFromString(const std::string& rawData, t_configParams &configParams) {
	this->setFlag(0);
	this->_configParams = configParams;
	this->_httpRequest = rawData;


    std::istringstream requestStream(rawData);
    std::string line;
    std::getline(requestStream, line);
    std::istringstream lineStream(line);

    // parsing 1ere ligne :  méthode, chemin, et version
    std::string method, path, version;
    lineStream >> method >> path >> version;

	int i = 0, j = 0;
	while (line[i]) {
		if (line[i] == ' ')
			j++;
		i++;
	}
	if (j != 2)
	{
		if (DEBUG_PARSING)
			std::cerr << "Error: 400 Bad Request - Line format incorrect" << std::endl;
		this->clear();
		this->setFlag(400);
		return ;
	}

	// gestion de la method et parsing 
	if (method != "GET" && method != "POST" && method != "DELETE")
	{
		if (DEBUG_PARSING)
			std::cerr << "Error : The request method is not supported by the server and cannot be handled." << std::endl;
		this->clear();
		this->setFlag(501);
        return ;
	}
	else
    	this->setMethodString(method);

    if (path.empty() || path[0] != '/' || path.find("..") != std::string::npos) {
        if (DEBUG_PARSING)
			std::cerr << "Error : 404 Not Found BECAUSE PATH = /../" << std::endl;
		this->clear();
		this->setFlag(404);
        return ;
    }
	if (!validateURI(path)) {
		this->clear();
		if (DEBUG_PARSING)
			std::cout << "Error: 400 Bad Request - Invalid URI" << std::endl;
		this->setFlag(400);
		return ;
	}
	if (!lengthURI(path)) {
		this->clear();
		if (DEBUG_PARSING)
			std::cout << "Error: 414 URI Too Long" << std::endl;
		this->setFlag(414);
		return ;
	}
    this->setPathString(path);

	if (version.substr(0, 5) != "HTTP/") {
		if (DEBUG_PARSING)
	    	std::cerr << "Error : 400 Bad Request" << std::endl;
		this->clear();
		if (DEBUG_PARSING)
			std::cout << "Error: 400 Bad Request - Invalid HTTP version" << std::endl;
		this->setFlag(400);
		return ;
	}
	std::string versionNum = version.substr(5);
	if (versionNum != "1.0" && versionNum != "1.1") {
		if (DEBUG_PARSING)
	   	 	std::cerr << "Error : The HTTP version used in the request is not supported by the server." << std::endl;
		this->clear();
		this->setFlag(505);
		return ;
	}
    this->setVersionString(version);

    // Parsing des en-têtes
	std::vector<std::string> headersVector;
	std::map<std::string, std::string> headersMap;
	while (std::getline(requestStream, line) && line != "\r") {
		std::size_t colonPos = line.find(": ");
		if (colonPos != std::string::npos) {
			std::string headerName = line.substr(0, colonPos);
			std::string headerValue = line.substr(colonPos + 2);
			if (!headerValue.empty() && headerValue[headerValue.size() - 1] == '\r') {
				headerValue.erase(headerValue.size() - 1);
			}

			// Conversion de l'en-tête name en min pour le case sensitive
			for (std::string::size_type i = 0; i < headerName.length(); ++i) {
				headerName[i] = std::tolower(headerName[i]);
			}

			headersVector.push_back(headerName + ": " + headerValue);
			headersMap[headerName] = headerValue;
		}
	}
	this->setHeadersVector(headersVector);
	this->setHeadersMap(headersMap);

    if (versionNum == "1.1") {
        std::map<std::string, std::string>::iterator it = headersMap.find("host");
        if (it == headersMap.end()) {
			if (DEBUG_PARSING)
	            std::cerr << "Error: The HTTP version used in the request is not supported by the server." << std::endl;
			this->clear();
			this->setFlag(505);
			return ;
        }
	}
	// const long MAX_CONTENT_LENGTH = 10485760;
	std::map<std::string, std::string>::const_iterator itLen = headersMap.find("content-length");
	if (itLen != headersMap.end()) {
		std::string contentLengthStr = itLen->second;

		contentLengthStr.erase(contentLengthStr.find_last_not_of(" \n\r\t") + 1);

		errno = 0;
		char* endPtr;
		// long contentLength = std::strtol(contentLengthStr.c_str(), &endPtr, 10);
		
		if (DEBUG_PARSING)
			std::cout << "Reste après strtol: '" << endPtr << "'" << std::endl;

		// if (errno == ERANGE || *endPtr != '\0' || contentLength < 0 || contentLength > MAX_CONTENT_LENGTH) {
		// 	if (DEBUG_PARSING)
		// 		std::cerr << "Error: Invalid Content-Length header value" << std::endl;
		// 	this->clear();
		// 	this->setFlag(400);
		// 	return ;
		// }
	}
		if (method == "POST") {
		std::map<std::string, std::string>::const_iterator itContentType = headersMap.find("content-type");
		if (itContentType == headersMap.end()) {
			if (DEBUG_PARSING)
				std::cerr << "Error: 400 Bad Request - Content-Type header is missing for POST request" << std::endl;
			this->clear();
			this->setFlag(400);
			return ;
		}
	}

    // Parsing du corps
    std::string body;
    while (std::getline(requestStream, line)) {
        body += line + "\n";
    }
    this->setBodyString(body);
	
	std::map<std::string, std::string>::const_iterator itContentLength = headersMap.find("content-length");
	if (itContentLength != headersMap.end()) {
		std::string contentLengthStr = itContentLength->second;
		contentLengthStr.erase(contentLengthStr.find_last_not_of(" \n\r\t") + 1);
	//je souhaite maintenant comparer la taille du body a contentLength, si elle est bonne, alors je check si contentLength est iinferieur ou egal a clientMaxBodySize
		if (contentLengthStr.empty() || contentLengthStr.find_first_not_of("0123456789") != std::string::npos) {
			if (DEBUG_PARSING)
				std::cerr << "Error: 400 Bad Request - Invalid Content-Length header value" << std::endl;
			this->clear();
			this->setFlag(400);
			return ;
		}
		unsigned long long contentLength = std::strtol(contentLengthStr.c_str(), NULL, 10);
		if (contentLength > _configParams.clientMaxBodySize) {
			if (DEBUG_PARSING)
				std::cerr << "Error: 413 Payload Too Large" << std::endl;
			this->clear();
			this->setFlag(413);
			return ;
		}
	}
}

bool Request::isChunkedRequest( void )
{
	std::map<std::string, std::string>::const_iterator it = _headersMap.find("content-length");

	if (it == _headersMap.end())
		return false;
	long unsigned int bodyLen = std::atoi(it->second.c_str());
	if (DEBUG_PARSING)
		std::cout << "Size of the body : " << bodyLen << std::endl;  //check cookie
	if (bodyLen > 0)
	{
		if (_bodyString.empty() || _bodyString.length() < bodyLen)
		{
			if (DEBUG_PARSING)			
				std::cout << "actual bodyLen : " << _bodyString.size() << std::endl;
			return true;
		}
		else if (_bodyString.length() == bodyLen + 1 && _bodyString[bodyLen] == '\n')
		{
			if (DEBUG_PARSING)
				std::cout << "actual bodyLen : " << _bodyString.size() << std::endl;
			return false;
		}
		else if (_bodyString.length() > bodyLen)
		{
			if (DEBUG_PARSING)
			{
				std::cout << "body len : " << _bodyString.length() << std::endl;
				std::cout << "body : " << _bodyString << std::endl;
			}
			this->clear();
			this->setFlag(400);
			return false;
		}
		else
		{
			if (DEBUG_PARSING)
				std::cout << "actual bodyLen : " << _bodyString.size() << std::endl;
			return false;
		}
	}
	else if (bodyLen == 0 && _bodyString.empty())
	{
		if (DEBUG_PARSING)
			std::cout << "actual bodyLen : " << _bodyString.size() << std::endl;
		return false;
	}
	else
	{
		if (DEBUG_PARSING)
		{
		std::cout << "actual bodyLen : " << _bodyString.size() << std::endl;
		std::cout << "body : " << _bodyString << std::endl;
		std::cerr << "Error: 400 Bad Request - Invalid Content-Length header value" << std::endl;
		}
		this->clear();
		this->setFlag(400);
		return false;
	}
}