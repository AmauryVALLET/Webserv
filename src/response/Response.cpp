#include "../../include/webserv.hpp"

//ORTHODOX CANONICAL FORM

Response::Response() : _httpVersion("HTTP/1.1"), _statusCode(200), _statusMessage("OK")
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - default constructor" << std::endl;
}

Response::~Response()
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - destructor" << std::endl;
}

Response::Response(const Response &src)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - copy constructor" << std::endl;
    *this = src;
}

Response &Response::operator=(const Response &src)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - assignation operator" << std::endl;
    if (this != &src)
    {
        _httpVersion = src._httpVersion;
        _statusCode = src._statusCode;
        _statusMessage = src._statusMessage;
        _headers = src._headers;
        _body = src._body;
    }
    return (*this);
}

//SETTERS

void Response::setHttpVersion(std::string httpVersion)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - setting HTTP version" << std::endl;
    _httpVersion = httpVersion;
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - HTTP version: " << _httpVersion << std::endl;
}

void Response::setStatusCode(int statusCode)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - setting status code" << std::endl;
    _statusCode = statusCode;
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - status code: " << _statusCode << std::endl;
}

void Response::setStatusMessage(std::string statusMessage)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - setting status message" << std::endl;
    _statusMessage = statusMessage;
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - status message: " << _statusMessage << std::endl;
}

void Response::setHeader(std::string key, std::string value)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - setting header with key: " << key << std::endl;
    _headers[key] = value;
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - header: " << key << " " << value << std::endl;
}

void Response::setBody(std::string body)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - setting body" << std::endl;
       _body = body;
}

//GETTERS

std::string Response::getHttpVersion() const
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - getting HTTP version" << std::endl;
    return (_httpVersion);
}

int Response::getStatusCode() const
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - getting status code" << std::endl;
    return (_statusCode);
}

std::string Response::getStatusMessage() const
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - getting status message" << std::endl;
    return (_statusMessage);
}

std::map<std::string, std::string> Response::getHeaders() const
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - getting headers" << std::endl;
    return (_headers);
}

std::string Response::getBody() const
{
    if (VERBOSE)
        std::cout << "\t(verbose) - Response - getting body" << std::endl;
    return (_body);
}

//METHOD
std::string Response::buildResponse()
{
    std::ostringstream responseStream;
    responseStream << _httpVersion << " " << _statusCode << " " << _statusMessage << "\r\n";

    std::map<std::string, std::string>::const_iterator it;
    for (it = _headers.begin(); it != _headers.end(); it++)
    {
        responseStream << it->first << ": " << it->second << "\r\n";
    }

    responseStream << "\r\n";

    responseStream << _body;

    return responseStream.str();
}

void Response::fillBadCode(int flag, Request &request)
{
    if (DEBUG_BAD_RESPONSE_CODES)
        std::cout << "\033[1;31m\t(FILL BAD CODE) Flag received = " << flag << "\033[0m" << std::endl;
    //si flag est pas 0 alors on a un code d'erreur
    if (flag != 0)
    {
        setStatusCode(flag);
        std::map<int, std::string>::iterator itcode = STATUS_CODES.find(flag);
        //si le code d'erreur est dans la map STATUS_CODES
        if (itcode != STATUS_CODES.end())
        {
            std::string statusMessage = itcode->second;
            if (DEBUG_BAD_RESPONSE_CODES)
                std::cout << "\033[1;31m\t(FILL BAD CODE) Status Message (code) = " << statusMessage << " (" << flag << ")\033[0m" << std::endl;
            setStatusMessage(statusMessage);
        }
        //sinon on a un code d'erreur inconnu qui n'est pas dans la map STATUS_CODES ==> error 500
        else
        {
            if (DEBUG_BAD_RESPONSE_CODES)
            {
                std::cerr << "\033[1;31m\t(FILL BAD CODE) ERROR: Flag (" << flag << ") not found in STATUS_CODES MAP\033[0m" << std::endl;
                //function to print the STATUS_CODES map
                std::cout << "\033[1;35m\t----- (FILL BAD CODE) STATUS_CODES MAP\033[0m" << std::endl;
                for (std::map<int, std::string>::iterator it = STATUS_CODES.begin(); it != STATUS_CODES.end(); ++it)
                {
                    std::cout << it->first << " => " << it->second << '\n';
                }
                std::cout << "\033[1;35m\t-----\033[0m" << std::endl;
            }
            // Handle the case where flag is not a key in STATUS_CODES
            fillBadCode(500, request);
            return;
        }

        //we will fill the body of the response with the content of the corresponding error page
        //to do so we will use the request._currentErrorPages map
            //we will use the "flag" as the key to find the corresponding pair in the map
            //the pair will contain the path to the error page and a 0 or 1 value
                //0 means the error page is located at the path /www/error_pages/... we will use the create_full_path function to get the full path
                //1 means the error page path changed because of a location block, and is located at the path /www/... we will use the root in the request._currentLocation and the create_full_path function to get the full path
        std::map<int, std::pair<std::string, int> > errorPages = request.getCurrentErrorPages();
        std::map<int, std::pair<std::string, int> >::const_iterator it = errorPages.find(flag);
        //if the error page flag is found in the map
        if (it != errorPages.end())
        {
            if (DEBUG_BAD_RESPONSE_CODES)
                std::cout << "\033[1;31m\t(FILL BAD CODE) IN MAP Error Page Path = " << it->second.first << ")\033[0m" << std::endl;
            std::string responseBodyPath;
            //default error page so path = [pwd + relative_path] + [error_pages default path] (create_full_path of [it->second.first])
            if (it->second.second == 0)
                responseBodyPath = create_full_path(request, it->second.first);
            //error page path changed because of a location block so path = [pwd + relative path] + [root + error_pages path] (create_full_path of [root + it->second.first])
            else if (it->second.second == 1)
            {
                responseBodyPath = request.getCurrentLocation().root + it->second.first;
                responseBodyPath = create_full_path(request, responseBodyPath);
            }
            if (DEBUG_BAD_RESPONSE_CODES)
                std::cout << "\033[1;31m\t(FILL BAD CODE) FINAL Error Page Path = " << responseBodyPath << ")\033[0m" << std::endl;
            //now that we have the path to the error page, we will fill the body with the content of the file at the path responseBodyPath
            std::ifstream file(responseBodyPath.c_str());
            if (file.is_open())
            {
                std::string responseBody((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                file.close();
                setBody(responseBody);
                if (DEBUG_BAD_RESPONSE_BODY)
                    std::cout << "\033[1;31m\t(FILL BAD CODE) Body Content = " << responseBody << "\033[0m" << std::endl;
                else if (DEBUG_BAD_RESPONSE_CODES)
                    std::cout << "\033[1;31m\t(FILL BAD CODE) Body filled with error page content\033[0m" << std::endl;
                if (DEBUG_BAD_RESPONSE_CODES)
                    std::cout << "\033[1;31m\t(FILL BAD CODE) Setting Content-Type header\033[0m" << std::endl;
                set_content_type_header(responseBodyPath, *this);
                if (DEBUG_BAD_RESPONSE_CODES)
                    std::cout << "\033[1;31m\t(FILL BAD CODE) Content-Type header = " << _headers["Content-Type"] << "\033[0m" << std::endl;
            }
            else
            {
                if (DEBUG_BAD_RESPONSE_CODES)
                {
                    std::cerr << "\033[1;31m\t(FILL BAD CODE) ERROR: Could not open error page file at path = " << responseBodyPath << "\033[0m" << std::endl;\
                    std::cerr << "\033[1;31m\t(FILL BAD CODE) Filling response body with a custom message: flag and status message\033[0m" << std::endl;
                }
                std::stringstream ss;
                ss << flag;
                std::string flagStr = ss.str();
                std::string responseBody = flagStr + " " + STATUS_CODES[flag];
                setBody(responseBody);
                if (DEBUG_BAD_RESPONSE_BODY)
                    std::cout << "\033[1;31m\t(FILL BAD CODE) Body Content = " << responseBody << "\033[0m" << std::endl;
                if (DEBUG_BAD_RESPONSE_CODES)
                    std::cout << "\033[1;31m\t(FILL BAD CODE) Setting Content-Type header\033[0m" << std::endl;
                setHeader("Content-Type", "text/plain");
                if (DEBUG_BAD_RESPONSE_CODES)
                    std::cout << "\033[1;31m\t(FILL BAD CODE) Content-Type header = " << _headers["Content-Type"] << "\033[0m" << std::endl;
            }
        }
        //if the error page flag is not found in the map, feeling the body with a custom message: the flag and the status message
        else
        {
            std::stringstream ss;
            ss << flag;
            std::string flagStr = ss.str();
            std::string responseBody = flagStr + " " + STATUS_CODES[flag];
            setBody(responseBody);
            if (DEBUG_BAD_RESPONSE_BODY)
                std::cout << "\033[1;31m\t(FILL BAD CODE) Body Content = " << responseBody << "\033[0m" << std::endl;
            if (DEBUG_BAD_RESPONSE_CODES)
                std::cout << "\033[1;31m\t(FILL BAD CODE) Setting Content-Type header\033[0m" << std::endl;
            setHeader("Content-Type", "text/plain");
            if (DEBUG_BAD_RESPONSE_CODES)
                std::cout << "\033[1;31m\t(FILL BAD CODE) Content-Type header = " << _headers["Content-Type"] << "\033[0m" << std::endl;
        }
    }
    // flag etait 0, c'est une erreur d'etre ici = probleme serveur ==> error 500
    else
    {
        if (DEBUG_BAD_RESPONSE_CODES)
            std::cout << "\033[1;31m\t(FILL BAD CODE) Flag is 0, filling error 500\033[0m" << std::endl;
        fillBadCode(500, request);
    }
}

void Response::fillGoodCode(int statusCode, Request &request, Response &response, std::string path, std::string responseBody)
{
    //GET REQUEST HEADER MAP
    std::map<std::string, std::string> headersMap = request.getHeadersMap();
    std::map<std::string, std::string>::const_iterator it = headersMap.find("connection");
    
    //FIRST LINE
    if (DEBUG_GOOD_RESPONSE_CODES)
        std::cout << "\033[1;32m\t(FILL GOOD CODE) Setting up first line\033[0m" << std::endl;
    setStatusCode(statusCode);
    std::string statusMessage = STATUS_CODES.at(statusCode);
    setStatusMessage(statusMessage);
    if (DEBUG_GOOD_RESPONSE_CODES)
        std::cout << "\033[1;32m\t\t(FILL GOOD CODE) Status message (code) = " << statusMessage << " (" << statusCode << ")\033[0m" << std::endl;

    //HEADERS
    if (DEBUG_GOOD_RESPONSE_CODES)
        std::cout << "\033[1;32m\t(FILL GOOD CODE) Setting up headers\033[0m" << std::endl;
        //Content-Type
    set_content_type_header(path, response);
        //Content-Length
    std::ostringstream lengthStream;
    lengthStream << responseBody.length();
    response.setHeader("Content-Length", lengthStream.str());
        //Date
    time_t now = time(0);
    char* dt = ctime(&now);
    char* newline = strchr(dt, '\n');
    if (newline) *newline = '\0';
    response.setHeader("Date", dt);
        //Connection
    it = headersMap.begin();
    it = headersMap.find("connection");
    if (it != headersMap.end())
        response.setHeader("Connection", it->second);
        //Accept
    response.setHeader("Accept", FIXED_HEADERS.at("Accept"));
        //Server
    response.setHeader("Server", FIXED_HEADERS.at("Server"));
        //Host
    it = headersMap.begin();
    it = headersMap.find("host");
    if (it != headersMap.end())
        response.setHeader("Host", it->second);

    response.setCookie(response, request);

    //BODY
    if (DEBUG_GOOD_RESPONSE_CODES)
        std::cout << "\033[1;32m\t(FILL GOOD CODE) Setting up body\033[0m" << std::endl;
    response.setBody(responseBody);
    if (DEBUG_GOOD_RESPONSE_BODY)
        std::cout << "\033[1;32m\t(FILL GOOD CODE) Body Content = " << responseBody << "\033[0m" << std::endl;
}
