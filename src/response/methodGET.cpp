#include "../../include/webserv.hpp"
#include <sys/stat.h>

/*
stat function is used to get the status of a file or a directory.
takes 2 arguments
1. path of the file or directory (here, it is a string and we convert it to a const char* using c_str() function)
2. a pointer to a stat structure
--> wil return 0 if the file or directory exists, otherwise -1
*/

bool checkValidPath(const std::string &path)
{
    if (path.empty())
    {
		if (DEBUG_GET_METHOD)
			std::cerr << "\033[1;35m(GET METHOD) - Path is empty\033[0m" << std::endl;
        return false;
    }
	for (std::size_t i = 0; i < path.size(); ++i)
	{
		char c = path[i];
		if (!isalnum(c) && c != '/' && c != '.' && c != '-' && c != '_')
		{
			if (DEBUG_GET_METHOD)
				std::cerr << "\033[1;35m(GET METHOD) - Invalid character in path\033[0m" << std::endl;
			return false;
		}
	}
    return true;
}

bool checkValidVersion(const std::string &version)
{
	if (version.empty())
	{
		if (DEBUG_GET_METHOD)
			std::cerr << "\033[1;35m(GET METHOD) - Version is empty\033[0m" << std::endl;
		return false;
	}

	if (version != "HTTP/1.1")
	{
		if (DEBUG_GET_METHOD)
			std::cerr << "\033[1;35m(GET METHOD) - Invalid version\033[0m" << std::endl;
		return false;
	}

	return true;
}

bool checkEmptyBody(const std::string &body)
{
	if (body.empty() != true)
	{
		if (DEBUG_GET_METHOD)
			std::cerr << "\033[1;35m(GET METHOD) - Body is not empty\033[0m" << std::endl;
		return false;
	}

	return true;
}

int checkFileOrDir(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
    {
		if (DEBUG_GET_METHOD)
			std::cerr << "\033[1;35m(GET METHOD) - Path does not exist\033[0m" << std::endl;
        return (0);
    }

    if (S_ISDIR(st.st_mode))
    {
		if (DEBUG_GET_METHOD)
			std::cerr << "\033[1;35m(GET METHOD) - Path is a directory\033[0m" << std::endl;
        return (2);
    }
    else if (S_ISREG(st.st_mode))
    {
		if (DEBUG_GET_METHOD)
			std::cerr << "\033[1;35m(GET METHOD) - Path is a file\033[0m" << std::endl;
        return (1);
    }
    else
    {
		if (DEBUG_GET_METHOD)
			std::cerr << "\033[1;35m(GET METHOD) - Path is neither a file nor a directory\033[0m" << std::endl;
        return (0);
    }
}

int checkValidGetRequest(Request &request)
{
	int type = 0;
	if (!checkValidPath(request.getPathString()))
		return 0;
	if (!checkValidVersion(request.getVersionString()))
		return 0;
	if (!checkEmptyBody(request.getBodyString()))
		return 0;
	type = checkFileOrDir(request.getPathString());
	return type;
}

void GET_method_static_resource(Request &request, Response &response)
{
	int type = checkValidGetRequest(request);
	if (type == 0)
	{
		if (DEBUG_GET_METHOD)
			std::cerr << "\033[1;35m(GET METHOD) => Invalid GET request, path not found\033[0m" << std::endl;
		response.fillBadCode(404, request);
		return;
	}
	if (type == 1)
	{
		if (DEBUG_GET_METHOD)
			std::cerr << "\033[1;35m(GET METHOD) => starting GET request on a file\033[0m" << std::endl;
		get_file(request, response);
	}
	else if (type == 2)
	{
		if (DEBUG_GET_METHOD)
			std::cerr << "\033[1;35m(GET METHOD) => starting GET request on a directory\033[0m" << std::endl;
		get_dir(request, response);
	}
}