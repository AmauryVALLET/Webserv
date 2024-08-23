

// Ce fichier est le fichier d'entête principal de notre serveur web. 
// Il contient les déclarations des classes et des fonctions qui seront utilisées dans le serveur web.

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

# define VERBOSE 0							//WHITE
# define GENERAL 0							//GREEN
# define DEBUG_PARSING 0					//BLUE
# define DEBUG_FLAG_PRINT 0					//GREEN
# define DEBUG_HEADERS_PRINT 0				//ORANGE
# define DEBUG_METHODS_PRINT 0				//ORANGE
# define DEBUG_AUTOINDEX_PRINT 0			//ORANGE
# define DEBUG_EXTENSION_PRINT 0			//ORANGE
# define DEBUG_PATHS_PRINT 0				//PURPLE
# define DEBUG_ROUTES 0						//BLUE
# define DEBUG_ERROR_PAGES 0				//BLUE
# define DEBUG_MAPS_PRINT 0					//BLUE

# define DEBUG_BAD_RESPONSE_CODES 0			//RED
# define DEBUG_BAD_RESPONSE_BODY 0			//RED //FAT print, un peu degueu

# define DEBUG_GOOD_RESPONSE_CODES 0		//GREEN
# define DEBUG_GOOD_RESPONSE_BODY 0			//GREEN //FAT print, un peu degueu

# define DEBUG_CGI 0						//CYAN
# define DEBUG_CGI_BODY 0					//CYAN //FAT print, un peu degueu

# define DEBUG_GET_METHOD 0					//VIOLET
# define DEBUG_POST_METHOD 0				//VIOLET
# define DEBUG_DELETE_METHOD 0				//VIOLET

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <fstream>
# include <sstream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <cstring>
# include <csignal>
# include <sys/epoll.h>
# include <netdb.h>
#include <filesystem>


# include "Request.hpp"
# include "Config.hpp"
# include "ServerManager.hpp"
# include "Server.hpp"
# include "StatusCodes_ContentTypes.hpp"
# include "response/ProcessRequest.hpp"
# include "response/Response.hpp"

extern int g_stop;


// PATH
const std::string RELATIVE_PATH = "/www";

// ############ PROTOTYPES ############

void CGI_GET_request(Request &inputRequest, Response &Response);
void CGI_POST_request(Request &inputRequest, Response &Response);

void GET_method_static_resource(Request &request, Response &response);
void get_file(Request &request, Response &response);
void get_dir (Request &request, Response &response);

void DELETE_method_static_resource(Request &request, Response &response);

void POST_method_static_resource(Request &input, Response &response);

void requestRouting(Request &request, Response &response);

void set_content_type_header(std::string &path, Response &response);
void initialize_fixed_maps();

void setThemeCookie(Response &response, Request &input);

std::string getAuthCookie(std::string sessionID);
std::string checkCookieThemeValue(Request &request);
extern std::map<int, std::pair<std::string, int > > ERROR_PAGES;
std::string create_full_path(Request &request, std::string otherPath);

int		checkIfIsItEnd(const std::string& httpRequest, const std::string& end);

// ##############################################
#endif