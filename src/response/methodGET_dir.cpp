#include "../../include/webserv.hpp"
#include <dirent.h>
#include <sstream>
#include <ctime>
#include <cstring>
#include <fstream>
#include <unistd.h>

/*
dirent = directory entry
            (structure defined in dirent.h)
readir = read directory
            (read the next entry in the directory stream and return a pointer to a dirent structure representing the next directory entry in the directory stream
            or NULL if there are no more directory entries in the directory stream. The directory stream automatically advances to the next entry)
closedir = close directory
            (close the directory stream when done)
d_type = type of file
            (in our case we need only DT_DIR and DT_REG)
DT_DIR = directory
DT_REG = regular file
*/

int checkIfIndexExists(Request &request, Response &response)
{
    if (DEBUG_GET_METHOD)
        std::cerr << "\033[1;35m\t(GET METHOD) - checking if index exists\033[0m" << std::endl;
    if (DEBUG_AUTOINDEX_PRINT || DEBUG_GET_METHOD)
        std::cerr << "\033[1;35m\tAutoindex = " << request.getCurrentLocation().autoIndex << "\033[0m" << std::endl;
    if (request.getIsLocationSet())
    {
        if (request.getCurrentLocation().autoIndex == false)
        {
            if (DEBUG_GET_METHOD || DEBUG_AUTOINDEX_PRINT)
                std::cerr << "\033[1;35m\t(GET METHOD) - Autoindex is not allowed\033[0m" << std::endl;
            return 0;
        }
    }
    std::vector<std::string> possible_files;
    possible_files.push_back("index.html");
    possible_files.push_back("index.htm");
    possible_files.push_back("index.php");
    possible_files.push_back("default.html");
    possible_files.push_back("default.htm");

    std::vector<std::string>::iterator it = possible_files.begin();
    for (; it != possible_files.end(); it++)
    {
        std::string index_path = request.getPathString();
        index_path += *it;
        std::ifstream file(index_path.c_str());
        if (file.good())
        {
            if (DEBUG_GET_METHOD)
                std::cerr << "\033[1;35m\t(GET METHOD) - index file exists : " << it->c_str() << "\033[0m" << std::endl;
            std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            response.fillGoodCode(200, request, response, index_path, body);
            return 1;
        }
    }
    if (DEBUG_GET_METHOD)
        std::cerr << "\033[1;35m\t(GET METHOD) - index file does not exist\033[0m" << std::endl;
    return 0;
}


void generateDirListing(Request &request, Response &response)
{
    if (DEBUG_GET_METHOD)
        std::cerr << "\033[1;35m(GET METHOD) - Generating directory listing\033[0m" << std::endl;
    std::string path = request.getPathString();
    std::ostringstream output;

    output << "<html>\n";
    output << "<head>\n";
    output << "<title>Directory Listing for " << path << "</title>\n";
    output << "</head>\n";
    output << "<body>\n";
    output << "<h1>Directory Listing for " << path << "</h1>\n";
    output << "<ul>\n";

    DIR *dir = opendir(path.c_str());
    if (!dir)
    {
        if (DEBUG_GET_METHOD)
            std::cerr << "\033[1;35m(GET METHOD) - Error: Failed to open directory at path\033[0m " << path << std::endl;
        response.fillBadCode(403, request);
        return;
    }
    else
    {
        dirent *entry;
        while ((entry = readdir(dir)))
        {
            output << "<li>";
            if(entry->d_type == DT_DIR)
                output << "[DIR] ";
            output << entry->d_name << "</li>\n";
        }

        closedir(dir);
    }

    output << "</ul>\n";
    output << "</body>\n";
    output << "</html>\n";

    std::string body = output.str();
    if (DEBUG_GET_METHOD)
        std::cerr << "\033[1;35m(GET METHOD) - Successfully generated directory listing\033[0m" << std::endl;
    response.fillGoodCode(200, request, response, path, body);
}

void complete_dir_path(Request &request)
{
    std::string path = request.getPathString();
    if (path[path.size() - 1] != '/')
    {
        if (DEBUG_GET_METHOD)
            std::cerr << "\033[1;35m(GET METHOD) - Path does not end with a slash, adding one\033[0m" << std::endl;
        path += "/";
    }
    request.setPathString(path);
}


void get_dir (Request &request, Response &response)
{
    complete_dir_path(request);
    if (checkIfIndexExists(request, response))
        return;
    else
        generateDirListing(request, response);
    return;
}