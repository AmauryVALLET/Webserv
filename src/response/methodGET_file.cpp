#include "../../include/webserv.hpp"
#include <fstream>
#include <sstream>
#include <string>

std::string readFile(const std::string &path)
{
	std::fstream file;
	file.open(path.c_str(), std::fstream::in);
    if (!file.is_open())
    {
		if (DEBUG_GET_METHOD)
			std::cerr << "\033[1;35m(GET METHOD) - Error: Failed to open file at path\033[0m " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void get_file(Request &request, Response &response)
{
	std::string path = request.getPathString();
	std::string filecontent = readFile(path);
	if (filecontent.empty())
	{
		response.fillBadCode(403, request);
		return;
	}
	if (DEBUG_GET_METHOD)
		std::cerr << "\033[1;35m(GET METHOD) - Successfully read file content\033[0m" << std::endl;
	response.fillGoodCode(200, request, response, path, filecontent);
	return;
}