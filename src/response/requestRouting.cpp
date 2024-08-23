#include "../../include/webserv.hpp"

int	isAllowedmethodsSet(Request &request)
{
	if (request.getIsLocationSet() == 1)
	{
		std::vector<std::string> methods = request.getCurrentLocation().methods;
		if (methods.size() > 0)
			return (1);
		else
			return (-1);
	}
	else
		return (-1);
}

int choseMethod(Request &inputRequest, Response &response)
{
    std::string method = inputRequest.getMethodString();
    std::string path = inputRequest.getPathString();

	bool isCgi = false;
	if (path.length() > 4)
    	isCgi = path.substr(path.length() - 4) == ".cgi";
	if (DEBUG_METHODS_PRINT)
		std::cout << "\033[1;33m\t(method) - Method is: " << method << "\033[0m" << std::endl;
	if (method == "GET")
	{
		if (DEBUG_METHODS_PRINT)
			std::cout << "\033[1;33m\t(choseMethod GET) - GET method request\033[0m" << std::endl;
		if (isAllowedmethodsSet(inputRequest) > 0)
		{
			std::vector<std::string> methods = inputRequest.getCurrentLocation().methods;
			if (std::find(methods.begin(), methods.end(), "GET") == methods.end())
			{
				if (DEBUG_METHODS_PRINT)
					std::cout << "\033[1;33m\t\t=> GET method NOT ALLOWED\033[0m" << std::endl;
				response.fillBadCode(405, inputRequest);
				return (0);
			}
		}
		if (isCgi)
		{
			if (DEBUG_CGI || DEBUG_METHODS_PRINT)
				std::cout << "\033[1;33m\t\t=> CGI GET method ALLOWED\033[0m" << std::endl;
			CGI_GET_request(inputRequest, response);
		}
		else
		{
			if (DEBUG_METHODS_PRINT)
				std::cout << "\033[1;33m\t\t=> Static GET method ALLOWED\033[0m" << std::endl;
			GET_method_static_resource(inputRequest, response);
		}
	}
	else if (method == "POST")
	{
		if (DEBUG_METHODS_PRINT)
			std::cout << "\033[1;33m\t(method) - POST method request\033[0m" << std::endl;
		if (isAllowedmethodsSet(inputRequest) > 0)
		{
			std::vector<std::string> methods = inputRequest.getCurrentLocation().methods;
			if (std::find(methods.begin(), methods.end(), "POST") == methods.end())
			{
				if (DEBUG_METHODS_PRINT)
					std::cout << "\033[1;33m\t\t=> POST method NOT ALLOWED\033[0m" << std::endl;
				response.fillBadCode(405, inputRequest);
				return (0);
			}
		}
		if (isCgi)
		{
			if (DEBUG_CGI || DEBUG_METHODS_PRINT)
				std::cout << "\033[1;33m\t\t=> CGI POST method ALLOWED\033[0m" << std::endl;
			CGI_POST_request(inputRequest, response);
		}
		else
		{
			if (DEBUG_METHODS_PRINT)
				std::cout << "\033[1;33m\t\t=> Static POST method ALLOWED\033[0m" << std::endl;
			POST_method_static_resource(inputRequest, response);
		}
	}
	else if (method == "DELETE")
	{
		if (DEBUG_METHODS_PRINT)
			std::cout << "\033[1;33m\t(method) - DELETE method request\033[0m" << std::endl;
		if (isAllowedmethodsSet(inputRequest) > 0)
		{
			std::vector<std::string> methods = inputRequest.getCurrentLocation().methods;
			if (std::find(methods.begin(), methods.end(), "DELETE") == methods.end())
			{
				if (DEBUG_METHODS_PRINT)
					std::cout << "\033[1;33m\t\t=> DELETE method NOT ALLOWED\033[0m" << std::endl;
				response.fillBadCode(405, inputRequest);
				return (0);
			}
		}
		if (DEBUG_CGI || DEBUG_METHODS_PRINT)
			std::cout << "\033[1;33m\t\t=> DELETE method ALLOWED\033[0m" << std::endl;
		DELETE_method_static_resource(inputRequest, response);
	}
	else
	{
		if (DEBUG_METHODS_PRINT)
		{
			std::cout << "\033[1;31m\t(method) - Method is: " << method << "\033[0m" << std::endl;
			std::cout << "\033[1;31m\t(method) - Method is not one of the 3\033[0m" << std::endl;
		}
		response.fillBadCode(501, inputRequest);
	}
    return (1);
}


void set_content_type_header(std::string &path, Response &response)
{
	std::string extension = "html";
	size_t pos = path.find_last_of(".");
	if (pos != std::string::npos)
		extension = path.substr(pos + 1);
	std::string contentType = CONTENT_TYPES.at(extension);
	if (DEBUG_EXTENSION_PRINT)
		std::cout << "\033[1;33m\t(content type) - EXTENSION = " << extension << "\033[0m" << std::endl;
	response.setHeader("Content-Type", contentType);
	return ;

}

std::string create_full_path(Request &request, std::string otherPath)
{
	std::string inputPath;
	if (otherPath.empty())
    	inputPath = request.getPathString();
	else
		inputPath = otherPath;

	while (inputPath.length() > 1 && inputPath[inputPath.length() - 1] == '/' && inputPath[inputPath.length() - 2] == '/')
		inputPath = inputPath.substr(0, inputPath.length() - 1);
	
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
		if (DEBUG_PATHS_PRINT && otherPath.empty())
			std::cout << "\t\033[1;35m(full path) INPUT Path: " << request.getPathString() << "\033[0m" << std::endl;
		else if (DEBUG_PATHS_PRINT && !otherPath.empty())
			std::cout << "\t\033[1;35m(full path) OTHER Path: " << otherPath << "\033[0m" << std::endl;
        std::string cwdPath = cwd;
		if (inputPath == "/favicon.ico")
			inputPath = "/favicon/favicon.ico";
		if (inputPath == "/preferences_cookie/")
			inputPath = checkCookieThemeValue(request);
		inputPath = cwdPath + RELATIVE_PATH + inputPath;
		if (DEBUG_PATHS_PRINT)
			std::cout << "\t\033[1;35m(full path) FINAL Path: " << inputPath << "\033[0m" << std::endl;
		if (otherPath.empty())
			request.setPathString(inputPath);
    }
	return (inputPath);
}

void	printHeaders(std::map<std::string, std::string> headersMap)
{
	std::map<std::string, std::string>::const_iterator it;
	for (it = headersMap.begin(); it != headersMap.end(); ++it)
	{
		std::cout << "\t\033[1;33m" << "(header) - " << it->first << ":  " << it->second << "\033[0m" << std::endl;
	}
}

void initialize_locations(Request &request)
{
    std::vector<t_locations> locations = request.getConfigParamsLocations();
    request.setConfigParamsLocations(locations);
    for (std::vector<t_locations>::iterator it = locations.begin(); it != locations.end(); ++it)
    {
        std::string newLocation;
        for (std::string::iterator charIt = it->location.begin(); charIt != it->location.end(); ++charIt)
        {
            if (*charIt != '\n' && *charIt != '\r' && *charIt != ' ')
            {
                newLocation += *charIt;
            }
        }
        it->location = newLocation;

        std::string newRoot;
        for (std::string::iterator charIt = it->root.begin(); charIt != it->root.end(); ++charIt)
        {
            if (*charIt != '\n' && *charIt != '\r' && *charIt != ' ')
            {
                newRoot += *charIt;
            }
        }
        it->root = newRoot;
    }
	if (DEBUG_ROUTES)
	{
		std::cout << "\t\033[1;35m(root) INITIAL INPUT Path: " << request.getPathString() << "\033[0m" << std::endl;
		std::cout << "\033[1;34m" << "\t( --- LOCATIONS VECTOR ---)\033[0m" << std::endl;
		std::vector<t_locations>::iterator it = locations.begin();
		while (it != locations.end())
		{
			std::cout << "\033[1;34m" << "\t\t(routes) For this Location: " << it->location << " (length = " << it->location.length() << ")\033[0m" << std::endl;
			std::cout << "\033[1;34m" << "\t\t(routes)  -> Root: " << it->root << "\033[0m" << std::endl;
			it++;
		}
	}
	std::string requestInputPath = request.getPathString();
	size_t firstPos = requestInputPath.find("/");
	if (firstPos != std::string::npos)
	{
		size_t secondPos = requestInputPath.find("/", firstPos + 1);
		if (secondPos != std::string::npos)
		{
			request.setStartPathLocation(requestInputPath.substr(0, secondPos));
			request.setEndPathLocation(requestInputPath.substr(secondPos));
		}
	    else
		{
			request.setStartPathLocation(requestInputPath);
			request.setEndPathLocation("");
		}
	}
	else
	{
		request.setStartPathLocation(requestInputPath);
		request.setEndPathLocation("");
	}
	if (DEBUG_ROUTES)
	{
		std::cout << "\033[1;34m" << "\t( --- PARSE PATH ---)\033[0m" << std::endl;
		std::cout << "\033[1;34m" << "\t\t(routes) Start Path Location: " << request.getStartPathLocation() << " (length = " << request.getStartPathLocation().length() << ")\033[0m" << std::endl;
		std::cout << "\033[1;34m" << "\t\t(routes) End Path Location: " << request.getEndPathLocation() << "\033[0m" << std::endl;
	}
	std::vector<t_locations>::iterator it = locations.begin();
	while (it != locations.end())
	{
		if (it->location == request.getStartPathLocation())
		{
			request.setCurrentLocation(*it);
			request.setIsLocationSet(true);
			if (DEBUG_ROUTES)
			{
				std::cout << "\033[1;34m" << "\t( --- LOCATION : MATCHED ---)\033[0m" << std::endl;
				std::cout << "\033[1;34m" << "\t\t(routes) IsLocationSet: " << request.getIsLocationSet() << "\033[0m" << std::endl;
				std::cout << "\033[1;34m" << "\t\t(routes) Current Location: " << request.getCurrentLocation().location << "\033[0m" << std::endl;
				std::cout << "\033[1;34m" << "\t\t(routes) Current Root: " << request.getCurrentLocation().root << "\033[0m" << std::endl;
			}
			return ;
		}
		it++;
	}
	if (DEBUG_ROUTES)
	{
		std::cout << "\033[1;34m" << "\t( --- LOCATION : UNMATCHED ---)\033[0m" << std::endl;
		std::cout << "\033[1;34m" << "\t\t(routes) IsLocationSet: " << request.getIsLocationSet() << "\033[0m" << std::endl;
	}
}

void initialize_error_pages(Request &request)
{
	std::map<int, std::pair<std::string, int> > currentErrorPages = ERROR_PAGES;
	if (request.getIsLocationSet())
	{
		std::map<int, std::string> errorPages = request.getCurrentLocation().errorPages;
		std::map<int, std::string>::iterator it = errorPages.begin();
		while (it != errorPages.end())
		{
			currentErrorPages[it->first] = std::make_pair(it->second, 1);
			it++;
		}
	}
	request.setCurrentErrorPages(currentErrorPages);
	if (DEBUG_ROUTES)
	{
		std::cout << "\033[1;34m" << "\t( --- ERROR PAGES ---)\033[0m" << std::endl;
		std::map<int, std::pair<std::string, int> > currentErrorPages = request.getCurrentErrorPages();
		std::map<int, std::pair<std::string, int> >::iterator it = currentErrorPages.begin();
		while (it != currentErrorPages.end())
		{
			std::cout << "\033[1;34m" << "\t\t(routes) Error Page: " << it->first << " -> " << it->second.first << " (" << it->second.second << ")\033[0m" << std::endl;
			it++;
		}
	}
}

void upload_allow_path(Request &request)
{
	if (request.getIsLocationSet())
	{
		if (DEBUG_ROUTES)
		{
			std::cout << "\033[1;34m" << "\t(--- UPLOADS ---)\033[0m" << std::endl;
			std::cout << "\033[1;34m" << "\t\t(routes) AllowUpload: " << request.getCurrentLocation().allowUpload << "\033[0m" << std::endl;
		}
		if (request.getCurrentLocation().allowUpload)
		{
			if (DEBUG_ROUTES)
			{
				std::cout << "\033[1;34m" << "\t\t(--- UPLOAD ALLOWED ---)\033[0m" << std::endl;
			}
			if (!request.getCurrentLocUploadFilePath().empty())
			{
				if (DEBUG_ROUTES)
				{
					std::cout << "\033[1;34m" << "\t\t(--- FINAL UPLOAD PATH READY ---)\033[0m" << std::endl;
				}
				std::string finalpath = create_full_path(request, request.getCurrentLocUploadFilePath());
				request.setFinalUploadPathIfAllowed(finalpath);
			}
			else
			{
				if (DEBUG_ROUTES)
				{
					std::cout << "\033[1;34m" << "\t\t(--- UPLOAD PATH EMPTY >> FILLED WITH END PATH >> UPLOAD NOT ALLOWED ---)\033[0m" << std::endl;
				}
				request.setFinalUploadPathIfAllowed("");
			}
		}
		else
		{
			if (DEBUG_ROUTES)
			{
				std::cout << "\033[1;34m" << "\t\t(--- UPLOAD NOT ALLOWED ---)\033[0m" << std::endl;
			}
			request.setFinalUploadPathIfAllowed("");
		}
	}
	else
		request.setFinalUploadPathIfAllowed("");
}

void rerootPath(Request &request)
{
	if (request.getIsLocationSet())
	{
		std::string root = request.getCurrentLocation().root;
		std::string endPath = request.getEndPathLocation();
		std::string fullPath = root + endPath;
		request.setPathString(fullPath);
		if (DEBUG_ROUTES)
		{
			std::cout << "\033[1;34m" << "\t (--- REROOTING... ---)\033[0m" << std::endl;
			std::cout << "\t\033[1;35m(root) NEW INPUT Path: " << request.getPathString() << "\033[0m" << std::endl;
		}
	}
	return ;
}

int createLocationHeader(Request &request, Response &response)
{
	if (request.getIsLocationSet() && request.getPathString() == request.getCurrentLocation().location)
	{
		if (DEBUG_ROUTES)
		{
			std::cout << "\033[1;34m" << "\t(--- SETTING LOCATION HEADER ---)\033[0m" << std::endl;
			std::cout << "\033[1;34m" << "\t\t(routes) Location Header: " << request.getPathString() << "/" << "\033[0m" << std::endl;
		}
		response.setHeader("Location", request.getPathString() + "/");
		response.fillGoodCode(303, request, response, request.getPathString(), "");
		return (1);
	}	
	if (DEBUG_ROUTES)
	{
		std::cout << "\033[1;34m" << "\t(--- NO LOCATION HEADER SET ---)\033[0m" << std::endl;
	}
	return (0);
}

void requestRouting(Request &request, Response &response)
{
    if (GENERAL)
	{
		std::cout << "\033[1;32m>>>>>>>>>>>>> PROCESS REQUEST <<<<<<<<<<<<<<\033[0m" << std::endl;
		std::cout << "\033[1;32m\t>>>>>>>>>>>>>   Methode:   [" << request.getMethodString() << "]\033[0m" << std::endl;
		std::cout << "\033[1;32m\t>>>>>>>>>>>>>   Path:      [" << request.getPathString() << "]\033[0m" << std::endl;
	}
	initialize_locations(request);
	if (createLocationHeader(request, response))
		return ;
	rerootPath(request);
	upload_allow_path(request);
	create_full_path(request, "");
	initialize_error_pages(request);

	if (DEBUG_HEADERS_PRINT)
		printHeaders(request.getHeadersMap());

	choseMethod(request, response);
}