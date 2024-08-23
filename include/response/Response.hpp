//this class is made to handle the response of the server, after receiving the request from the client (Request)

#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "../webserv.hpp"
# include <iostream>
# include <string>
# include <map>
# include <vector>
# include <exception>
# include <fstream>
# include <sstream>
# include <algorithm>
# include <iterator>

class Response
{
    private:
        std::string _httpVersion;
        int _statusCode;
        std::string _statusMessage;
        std::map<std::string, std::string> _headers;
        std::string _body;

    public:
        Response();
        ~Response();
        Response(const Response &src);
        Response &operator=(const Response &src);

        //setters
        void setHttpVersion(std::string httpVersion);
        void setStatusCode(int statusCode);
        void setStatusMessage(std::string statusMessage);
        void setHeader(std::string key, std::string value);
        void setBody(std::string body);

        //getters
        std::string getHttpVersion() const;
        int getStatusCode() const;
        std::string getStatusMessage() const;
        std::map<std::string, std::string> getHeaders() const;
        std::string getHeader(std::string key) const;
        std::string getBody() const;

        //methods
        std::string buildResponse();
        void fillBadCode(int flag, Request &request);
        void fillGoodCode(int statusCode, Request &request, Response &response, std::string path, std::string responseBody);
        std::string setSessionIDCookie() const;
        void setCookie(Response &response, Request &request);
};

#endif
