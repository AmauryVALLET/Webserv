#include "../../include/webserv.hpp"


std::map<int, std::string> STATUS_CODES;

void initialize_status_codes()
{
    STATUS_CODES[400] = "Bad Request";
    STATUS_CODES[401] = "Unauthorized";
    STATUS_CODES[403] = "Forbidden";
    STATUS_CODES[404] = "Not Found";
    STATUS_CODES[405] = "Method Not Allowed";
    STATUS_CODES[409] = "Conflict";
    STATUS_CODES[413] = "Payload Too Large";
    STATUS_CODES[414] = "URI Too Long";
    STATUS_CODES[500] = "Internal Server Error";
    STATUS_CODES[501] = "Not Implemented";
    STATUS_CODES[502] = "Bad Gateway";
    STATUS_CODES[505] = "Version Not Supported";


	STATUS_CODES[200] = "OK";
    STATUS_CODES[204] = "No Content";
    STATUS_CODES[302] = "Found";
    STATUS_CODES[303] = "See Other";
}


std::map<std::string, std::string> CONTENT_TYPES;

void initialize_content_types()
{
	CONTENT_TYPES["html"] = "text/html";
	CONTENT_TYPES["hpp"] = "text/hpp";
	CONTENT_TYPES["css"] = "text/css";
	CONTENT_TYPES["txt"] = "text/txt";
	CONTENT_TYPES["plain"] = "text/plain";
	CONTENT_TYPES["md"] = "text/md";
	CONTENT_TYPES["jpeg"] = "image/jpeg";
	CONTENT_TYPES["jpg"] = "image/jpg";
	CONTENT_TYPES["cgi"] = "text/html";
    CONTENT_TYPES["gif"] = "image/gif";
    CONTENT_TYPES["ico"] = "image/ico";
    CONTENT_TYPES["png"] = "image/png";
    CONTENT_TYPES["js"] = "text/javascript";
    CONTENT_TYPES["scss"] = "text/x-scss";
    CONTENT_TYPES["eot"] = "application/vnd.ms-fontobject";
    CONTENT_TYPES["svg"] = "image/svg+xml";
    CONTENT_TYPES["ttf"] = "application/x-font-ttf";
    CONTENT_TYPES["woff"] = "application/x-font-woff";
    CONTENT_TYPES["woff2"] = "font/woff2";

}

std::map<std::string, std::string> FIXED_HEADERS;

void initialize_fixed_headers()
{
    FIXED_HEADERS["Accept"] = "";
    FIXED_HEADERS["Server"] = "Webserv/1.0";
}

std::map<int, std::pair<std::string, int > > ERROR_PAGES;

void initialize_error_pages()
{
    ERROR_PAGES[400] = std::make_pair("/error_pages/400.html", 0);
    ERROR_PAGES[401] = std::make_pair("/error_pages/401.html", 0);
    ERROR_PAGES[403] = std::make_pair("/error_pages/403.html", 0);
    ERROR_PAGES[404] = std::make_pair("/error_pages/404.html", 0);
    ERROR_PAGES[408] = std::make_pair("/error_pages/408.html", 0);
    ERROR_PAGES[500] = std::make_pair("/error_pages/500.html", 0);
}

void initialize_fixed_maps()
{
    if (DEBUG_MAPS_PRINT)
    {
        std::cout << "\033[1;34mInitializing Status Codes, Content Types, Fixed Headers, and Error Pages\033[0m" << std::endl;
    }
	initialize_status_codes();
	initialize_content_types();
	initialize_fixed_headers();
    initialize_error_pages();
    
    std::map<std::string, std::string>::iterator it = CONTENT_TYPES.begin();
    std::string acceptValue = "";
    while (it != CONTENT_TYPES.end())
    {
        acceptValue += it->second;
        if (it != --CONTENT_TYPES.end())
            acceptValue += ",";
        it++;
    }
    FIXED_HEADERS["Accept"] = acceptValue;
	return ;
}