//this class is made to handle the response of the server, after receiving the request from the client (Request)

#ifndef PROCESSREQUEST_HPP
# define PROCESSREQUEST_HPP
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

// class Response;
# include "Response.hpp"

class ProcessRequest
{
    private:
        Request _parseInput;
        Response _response;
        
    public:
        ProcessRequest(Request parseInput);
        ~ProcessRequest();
        ProcessRequest(const ProcessRequest &src);
        ProcessRequest &operator=(const ProcessRequest &src);

        //setters
        void setHttpVersion(std::string httpVersion);
        void setStatusCode(int statusCode);
        void setStatusMessage(std::string statusMessage);
        void setHeader(std::string key, std::string value);
        void setBody(std::string body);

        //getters
        Request getParseInput() const;
        Response getResponse() const;
        bool    getHeaderConnectionKeepaliveOrNot() const;

        //method
        std::string StartProcessRequest();

};

#endif

void requestRouting(Request &request, Response &response);

