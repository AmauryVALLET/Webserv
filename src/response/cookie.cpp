#include "../../include/webserv.hpp"

std::string checkCookieThemeValue(Request &request)
{
    std::string finalPath;
    std::map<std::string, std::string> headersMap = request.getHeadersMap();
    std::map<std::string, std::string>::const_iterator it = headersMap.find("cookie");

    if (it != headersMap.end())
    {
        std::string cookie = it->second;
        if (cookie.find("theme=dark") != std::string::npos)
            finalPath = "/preferences_cookie/dark.html";
        else if (cookie.find("theme=light") != std::string::npos)
            finalPath = ("/preferences_cookie");
        else
            finalPath = ("/preferences_cookie");
    }
    else
        finalPath = "/preferences_cookie";

    return (finalPath);
}

std::string Response::setSessionIDCookie() const
{
    srand(time(NULL));
    
    std::string cookie = "sessionID=";
    std::string sessionID;

    for (int i = 0; i < 10; i++)
    {
        sessionID += (rand() % 10) + 48;
    }
    cookie += sessionID;
    if (VERBOSE)
        std::cout << "cookie: " << cookie << std::endl;
    return cookie;
}

void Response::setCookie(Response &response, Request &request)
{
    std::map<std::string, std::string> headersMap = request.getHeadersMap();
    std::map<std::string, std::string>::const_iterator it = headersMap.find("connection");
    
    it = headersMap.begin();
    it = headersMap.find("cookie");
    if (it == headersMap.end() || (it != headersMap.end() && it->second.find("sessionID") == std::string::npos))
        response.setHeader("Set-cookie", response.setSessionIDCookie());

}

void setThemeCookie(Response &response, Request &input)
{

    std::string theme = input.getBodyString();
    std::map<std::string, std::string> headersMap = input.getHeadersMap();
    std::map<std::string, std::string>::const_iterator it = headersMap.find("referer");
    std::string referer;
    std::string path = input.getPathString();

    if (theme == "theme=dark\n" && it != headersMap.end())
    {
        referer = it->second;
        response.setHeader("Set-cookie", "theme=dark");
        response.setHeader("Location", referer);
        response.fillGoodCode(303, input, response, path, " ");
    }
    else if (theme == "theme=light\n" && it != headersMap.end())
    {
        referer = it->second;
        response.setHeader("Set-cookie", "theme=light");
        response.setHeader("Location", referer);
        response.fillGoodCode(303, input, response, path, " ");
    }
    else
    {
        response.fillBadCode(400, input);
        return ;
    }
}