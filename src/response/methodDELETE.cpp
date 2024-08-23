#include "../../include/webserv.hpp"
#include <sys/stat.h>

/*
stat function is used to get the status of a file or a directory.
takes 2 arguments
1. path of the file or directory (here, it is a string and we convert it to a const char* using c_str() function)
2. a pointer to a stat structure
--> wil return 0 if the file or directory exists, otherwise -1
*/

int file_or_directory_del(std::string path)
{
    struct stat s;
    if( stat(path.c_str(),&s) == 0 )
    {
        if( s.st_mode & S_IFDIR )
            return 2;
        else if( s.st_mode & S_IFREG )
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

void delete_file(std::string path, Request &request, Response &response)
{
	if (access(path.c_str(), W_OK) != 0)
	{
		response.fillBadCode(403, request);
		return;
	}
	if (remove(path.c_str()) != 0)
	{
		response.fillBadCode(500, request);
	}
	else
	{
		response.fillBadCode(204, request);
	}
	return;
}

void delete_directory(std::string path, Request &request, Response &response)
{
	if (access(path.c_str(), W_OK) != 0)
	{
		response.fillBadCode(403, request);
		return;
	}
	if (rmdir(path.c_str()) != 0)
	{
		response.fillBadCode(500, request);
	}
	else
	{
		response.fillBadCode(204, request);
	}
	return;
}

void DELETE_method_static_resource(Request &request, Response &response)
{
	int type = file_or_directory_del(request.getPathString());
	if (type == 1)
		delete_file(request.getPathString(), request, response);
	else if (type == 2)
		delete_directory(request.getPathString(), request, response);
	else
		response.fillBadCode(404, request);
	return;
}