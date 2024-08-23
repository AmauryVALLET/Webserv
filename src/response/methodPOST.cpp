#include "../../include/webserv.hpp"
#include <sys/stat.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

/*
stat function is used to get the status of a file or a directory.
takes 2 arguments
1. path of the file or directory (here, it is a string and we convert it to a const char* using c_str() function)
2. a pointer to a stat structure
--> wil return 0 if the file or directory exists, otherwise -1
*/

std::string fileContent(std::string bodyStr, std::string boundary)
{
    std::string content;
    content = bodyStr.substr(bodyStr.find(boundary) + boundary.size() + 2);
    content = content.substr(0, content.find(boundary) - 2);
    size_t headerLen = content.find("\r\n\r\n");
    if (headerLen != std::string::npos)
        content = content.substr(headerLen + 4);
    return content;
}

int uploadFile(Request &input, Response &response)
{
    std::map<std::string, std::string> headers = input.getHeadersMap();
    std::map<std::string, std::string>::iterator it = headers.find("content-type");


    if (it == headers.end())
        return 0;

    std::string contentType = it->second;
    if (contentType.find("multipart/form-data") == std::string::npos)
        return 0;
    if (contentType.find("boundary=") == std::string::npos)
        return 0;


    std::string boundary = contentType.substr(contentType.find("boundary=") + 9);
    std::string bodyStr = input.getBodyString();

    if (bodyStr.find(boundary) == std::string::npos)
        return 0;
    if (bodyStr.find("filename=") == std::string::npos)
        return 0;
    if (bodyStr.find("name=\"file\"") == std::string::npos)
        return 0;
    if (bodyStr.find("Content-Type: ") == std::string::npos)
        return 0;

    std::string filename;
    std::string fileContent;
    std::string currentBoundary = "--" + boundary;
    size_t startPos = bodyStr.find(currentBoundary);

    while (startPos != std::string::npos)
    {
        size_t endPos = bodyStr.find(currentBoundary, startPos + 1);
        if (endPos == std::string::npos)
            break;
        std::string block = bodyStr.substr(startPos, endPos - startPos);
        size_t filenamePos = block.find("filename=\"");
        if (filenamePos != std::string::npos)
        {
            filename = block.substr(filenamePos + 10);
            filename = filename.substr(0, filename.find("\""));
        }

        size_t contentPos = block.find("\r\n\r\n");
        if (contentPos != std::string::npos)
        {
            fileContent += block.substr(contentPos + 4);
        }
        startPos = bodyStr.find(currentBoundary, endPos);
    }

    if (filename.empty() || fileContent.empty())
    {
        response.fillBadCode(400, input);
        return -1;
    }  

    std::string uploadPath = input.getFinalUploadPathIfAllowed() + "/" + filename;

    std::ofstream file(uploadPath.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

    if (file.is_open())
    {
        file << fileContent;
        file.close();
        it = headers.find("referer");
        if (it != headers.end())
        {
            response.setHeader("Location", it->second);
        }

        return 1;
    }
    else
    {
        response.fillBadCode(500, input);
        return -1;
    }
}

bool clientTryToUploadFileIfNotAllowed(Request &input, Response &response)
{
    std::map<std::string, std::string> headers = input.getHeadersMap();
    std::map<std::string, std::string>::iterator it = headers.find("content-type");

    if (it == headers.end())
        return false;

    std::string contentType = it->second;
    if (contentType.find("multipart/form-data") == std::string::npos)
        return false;
    if (contentType.find("boundary=") == std::string::npos)
        return false;

    std::string boundary = contentType.substr(contentType.find("boundary=") + 9);
    std::string bodyStr = input.getBodyString();

    if (bodyStr.find(boundary) == std::string::npos)
        return false;
    if (bodyStr.find("filename=") == std::string::npos)
        return false;
    if (bodyStr.find("name=\"file\"") == std::string::npos)
        return false;
    if (bodyStr.find("Content-Type: ") == std::string::npos)
        return false;

    response.fillBadCode(403, input);
    return true;
}

void postContent(Request &input, Response &response)
{
    if (DEBUG_POST_METHOD)
        std::cout << "\033[1;35m\t(POST METHOD) - Processing POST content...\033[0m" << std::endl;
    std::string path = input.getPathString();
    std::string bodystr = input.getBodyString();
    if (bodystr.empty())
    {
        if (DEBUG_POST_METHOD)
            std::cout << "\033[1;35m\t(POST METHOD) - WARNING: Body is empty\033[0m" << std::endl;
    }

    if (path.find("/set-theme") != std::string::npos)
    {
        setThemeCookie(response, input);
        return ;
    }

    if (input.getFinalUploadPathIfAllowed() != "")
    {
        int uploadReturn = uploadFile(input, response);
        if (uploadReturn == 1)
        {
            response.fillGoodCode(303, input, response, path, " ");
            return ;
        }
        else if (uploadReturn == -1)
            return ;
    }
    else if (clientTryToUploadFileIfNotAllowed(input, response))
        return ;

    std::ofstream file(path.c_str(), std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file << input.getBodyString();
        file.close();
        response.fillGoodCode(200, input, response, path, bodystr);

    }
    else
    {
        if (DEBUG_POST_METHOD)
            std::cout << "\033[1;35m\t(POST METHOD) - ERROR: File could not be opened\033[0m" << std::endl;
        response.fillBadCode(403, input);
    }
}

bool fileWritableByUser(const std::string &path)
{
    if (path.find("/register") != std::string::npos || path.find("/login") != std::string::npos)
        return true;
    struct stat s;
	if (stat(path.c_str(),&s) == 0)
    {
		if( s.st_mode & S_IWUSR )
			return true;
		else
			return false;
	}
    else
		return false;
}

int checkIfCanprocessPost(Request &input, Response &response)
{
	if (!fileWritableByUser(input.getPathString()))
	{
        response.fillBadCode(403, input);
		return 0;
	}
	return 1;
}

int file_or_directory_post(std::string path)
{
    if(DEBUG_POST_METHOD)
        std::cout << "\033[1;35m\t(POST METHOD) - Checking if file or directory...\033[0m" << std::endl;
    struct stat s;
	if( stat(path.c_str(),&s) == 0 )
	{
		if( s.st_mode & S_IFDIR )
        {
            if (DEBUG_POST_METHOD)
                std::cout << "\033[1;35m\t(POST METHOD) - Directory detected\033[0m" << std::endl;
			return 2;
        }
		else if( s.st_mode & S_IFREG )
        {
            if (DEBUG_POST_METHOD)
                std::cout << "\033[1;35m\t(POST METHOD) - File detected\033[0m" << std::endl;
			return 1;
        }
		else
        {
            if (DEBUG_POST_METHOD)
                std::cout << "\033[1;35m\t(POST METHOD) - Existing resources being neither a file nor a directory detected\033[0m" << std::endl;
			return (-1);
        }
	}
	else
    {
        if (DEBUG_POST_METHOD)
            std::cout << "\033[1;35m\t(POST METHOD) - File/Directory does not exist\033[0m" << std::endl;
		return 0;
    }
}

int createNewFile(std::string path)
{
    if (DEBUG_POST_METHOD)
        std::cout << "\033[1;35m\t(POST METHOD) - Creating new file...\033[0m" << std::endl;
    std::string directoryPath = path.substr(0, path.find_last_of("/"));
    std::string newFileName = path.substr(path.find_last_of("/") + 1);
    if (DEBUG_POST_METHOD)
    {
        std::cout << "\033[1;35m\t(POST METHOD) - Directory path: " << directoryPath << "\033[0m" << std::endl;
        std::cout << "\033[1;35m\t(POST METHOD) - New file name: " << newFileName << "\033[0m" << std::endl;
    }
    std::ofstream newFile(path.c_str());
    if (newFile.is_open())
    {
        newFile.close();
        chmod(path.c_str(), S_IRUSR | S_IWUSR);
        if (DEBUG_POST_METHOD)
            std::cout << "\033[1;35m\t(POST METHOD) - New writable file created\033[0m" << std::endl;
        return 1;
    }
    else
    {
        if (DEBUG_POST_METHOD)
            std::cout << "\033[1;35m\t(POST METHOD) - New file could not be created\033[0m" << std::endl;
        return 0;
    }
}

int canCreateNewFile(std::string path)
{
    if (DEBUG_POST_METHOD)
        std::cout << "\033[1;35m\t(POST METHOD) - Checking if new file can be created...\033[0m" << std::endl;
    if (path[path.size() - 1] == '/')
    {
        if (DEBUG_POST_METHOD)
            std::cout << "\033[1;35m\t(POST METHOD) - Path is a directory\033[0m" << std::endl;
        return 0;
    }
    std::string pathWithoutLastPart = path.substr(0, path.find_last_of("/"));
    if (DEBUG_POST_METHOD)
        std::cout << "\033[1;35m\t(POST METHOD) - Path without last part: " << pathWithoutLastPart << "\033[0m" << std::endl;
    struct stat s;
    if( stat(pathWithoutLastPart.c_str(),&s) == 0 )
    {
        if( s.st_mode & S_IFDIR )
        {
            if (DEBUG_POST_METHOD)
                std::cout << "\033[1;35m\t(POST METHOD) - Directory exists, creation of new file should be possible\033[0m" << std::endl;
            return 1;
        }
        else
        {
            if (DEBUG_POST_METHOD)
                std::cout << "\033[1;35m\t(POST METHOD) - Directory does not exist, impossible to create new file\033[0m" << std::endl;
            return 0;
        }
    }
    else
    {
        if (DEBUG_POST_METHOD)
            std::cout << "\033[1;35m\t(POST METHOD) - Directory does not exist, impossible to create new file\033[0m" << std::endl;
        return 0;
    }
}

int checkValidLocation(Request &input, Response &response)
{
	std::string path = input.getPathString();
    if (path.find("/set-theme") != std::string::npos)
        return 1;
    int resourcesOfPathStatus = file_or_directory_post(path);
    if (resourcesOfPathStatus == -1)
    {
        response.fillBadCode(404, input);
        return 0;
    }
    else if (resourcesOfPathStatus == 2)
	{
        if (input.getFinalUploadPathIfAllowed() != "")
            return 1;
        else
        {
            response.fillBadCode(403, input);
            return 0;
        }
	}
	else if (resourcesOfPathStatus == 1)
		return (checkIfCanprocessPost(input, response));
	else if (resourcesOfPathStatus == 0)
	{
        if (canCreateNewFile(path))
        {
            if (createNewFile(path))
                return 1;
            else
            {
                response.fillBadCode(403, input);
                return 0;
            }
        }
        else
        {
            response.fillBadCode(403, input);
		    return 0;
	    }
    }
    return (0);
}

void POST_method_static_resource(Request &input, Response &response)
{
    if (checkValidLocation(input, response))
        postContent(input, response);
    return;
}