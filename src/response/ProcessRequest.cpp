#include "../../include/webserv.hpp"

//ORTHODOX CANONICAL FORM

ProcessRequest::ProcessRequest(Request parseInput) : _parseInput(parseInput), _response()
{
    if (VERBOSE)
        std::cout << "\t(verbose) - ProcessRequest - constructor" << std::endl;
}

ProcessRequest::~ProcessRequest()
{
    if (VERBOSE)
        std::cout << "\t(verbose) - ProcessRequest - destructor" << std::endl;
}

ProcessRequest::ProcessRequest(const ProcessRequest &src)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - ProcessRequest - copy constructor" << std::endl;
    *this = src;
}

ProcessRequest &ProcessRequest::operator=(const ProcessRequest &src)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - ProcessRequest - assignation operator" << std::endl;
    if (this != &src)
    {
        _parseInput = src._parseInput;
        _response = src._response;
    }
    return (*this);
}

//SETTERS

void ProcessRequest::setHttpVersion(std::string httpVersion)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - ProcessRequest - setting HTTP version" << std::endl;
    _response.setHttpVersion(httpVersion);
}

void ProcessRequest::setStatusCode(int statusCode)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - ProcessRequest - setting status code" << std::endl;
    _response.setStatusCode(statusCode);
}

void ProcessRequest::setStatusMessage(std::string statusMessage)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - ProcessRequest - setting status message" << std::endl;
    _response.setStatusMessage(statusMessage);
}

void ProcessRequest::setHeader(std::string key, std::string value)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - ProcessRequest - setting header" << std::endl;
    _response.setHeader(key, value);
}

void ProcessRequest::setBody(std::string body)
{
    if (VERBOSE)
        std::cout << "\t(verbose) - ProcessRequest - setting body" << std::endl;
    _response.setBody(body);
}

//GETTERS

Request ProcessRequest::getParseInput() const
{
    if (VERBOSE)
        std::cout << "\t(verbose) - ProcessRequest - getting parse input" << std::endl;
    return _parseInput;
}

Response ProcessRequest::getResponse() const
{
    if (VERBOSE)
        std::cout << "\t(verbose) - ProcessRequest - getting response" << std::endl;
    return _response;
}

bool    ProcessRequest::getHeaderConnectionKeepaliveOrNot() const
{
    std::map<std::string, std::string> headersMap = _parseInput.getHeadersMap();
    std::map<std::string, std::string>::const_iterator it = headersMap.find("connection");
    if (it != headersMap.end())
        return it->second == "keep-alive";
    else
    return false;
}

//METHOD

std::string ProcessRequest::StartProcessRequest()
{
    if (GENERAL)
	    std::cout << "\033[1;32m>>>>>>>>>>>>> NEW REQUEST <<<<<<<<<<<<<<\033[0m" << std::endl;
    std::string finalResponseStr;
    initialize_fixed_maps();
    int flag = _parseInput.getFlag();
    if (DEBUG_FLAG_PRINT)
    	std::cout << "\033[1;32m\t>>>>>>>>>>>>>   checking flag...  =>\033[0m";
    if (flag != 0)
    {
        if (DEBUG_FLAG_PRINT)
    	    std::cout << "\033[1;32m  flag WRONG= "<< flag << "\033[0m" << std::endl;
        _response.fillBadCode(flag, _parseInput);
    }
    else
    {
        if (DEBUG_FLAG_PRINT)
        	std::cout << "\033[1;32m  flag OK= " << flag << "\033[0m" << std::endl;
        requestRouting(_parseInput, _response);
    }
    finalResponseStr = _response.buildResponse();

    return (finalResponseStr);
}