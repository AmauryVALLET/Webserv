#include "../../include/webserv.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/*
stat function is used to get the status of a file or a directory.
takes 2 arguments
1. path of the file or directory (here, it is a string and we convert it to a const char* using c_str() function)
2. a pointer to a stat structure
--> wil return 0 if the file or directory exists, otherwise -1
*/

bool check_valid_path(std::string path, Request inputRequest, Response &response)
{
	struct stat buffer;
	if (stat(path.c_str(), &buffer) != 0)
	{
        if (DEBUG_CGI)
            std::cout << "\033[31mCGI GET REQUEST : file does not exist\n" << std::endl;
		response.fillBadCode(404, inputRequest);
		return false;
	}
	if (!(buffer.st_mode & S_IXUSR))
	{
		response.fillBadCode(403, inputRequest);
		return false;
	}
	return true;
}

std::string executeCGIScript(const std::string &method, const std::string &path, const std::string &body)
{
    int in_fd[2], out_fd[2];
    pipe(in_fd);
    pipe(out_fd);

    pid_t pid = fork();
    if (pid == 0)
    {
        if (DEBUG_CGI)
            std::cout << "\033[36m\t(CGI) Child process created.\033[0m" << std::endl;
        close(in_fd[1]);
        close(out_fd[0]);

        setenv("REQUEST_METHOD", method.c_str(), 1);
        setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
        std::stringstream ss;
        ss << body.length();
        std::string content_length = ss.str();
        setenv("CONTENT_LENGTH", content_length.c_str(), 1);
        if (DEBUG_CGI)
        {
            std::cout << "\033[36m\t(CGI) Env variables created.\033[0m" << std::endl;
            std::cout << "\033[36m\t\t(CGI) Request-Method: " << method << "\033[0m" << std::endl;
            std::cout << "\033[36m\t\t(CGI) Content-Type: application/x-www-form-urlencoded\033[0m" << std::endl;
            std::cout << "\033[36m\t\t(CGI) Content-Length: " << content_length << "\033[0m" << std::endl;
        }

        dup2(in_fd[0], STDIN_FILENO);
        dup2(out_fd[1], STDOUT_FILENO);

        if (execl(path.c_str(), path.c_str(), (char*)NULL) == -1)
        {
            close(in_fd[0]);
            close(out_fd[1]);
            _exit(1); 
        }
    }
    else
    {
        if (DEBUG_CGI)
            std::cout << "\033[36m\t(CGI) Parent process created.\033[0m" << std::endl;
        close(in_fd[0]);
        close(out_fd[1]);

        write(in_fd[1], body.c_str(), body.length());
        close(in_fd[1]);

        if (DEBUG_CGI)
            std::cout << "\033[36m\t(CGI) Waiting for child process to terminate.\033[0m" << std::endl;
        int status;
        int wait_duration = 0;
        int max_wait_duration = 1000;
        while (waitpid(pid, &status, WNOHANG) == 0 && wait_duration < max_wait_duration)
        {
            usleep(10000);
            wait_duration++;
        }
        if (wait_duration >= max_wait_duration)
        {
            if (DEBUG_CGI)
                std::cout << "\033[36m\t(CGI) Child process terminated by timeout.\033[0m" << std::endl;
            return "";
        }
        if (DEBUG_CGI)
            std::cout << "\033[36m\t(CGI) Child process terminated.\033[0m" << std::endl;

        char buffer[4096];
        std::string result;
        ssize_t count;
        while ((count = read(out_fd[0], buffer, sizeof(buffer))) != 0)
        {
            result.append(buffer, count);
        }

        close(out_fd[0]);
        if (DEBUG_CGI && DEBUG_CGI_BODY)
            std::cout << "\033[36m\t(CGI) Script output: " << result << "\033[0m" << std::endl;
        else if (DEBUG_CGI)
            std::cout << "\033[36m\t(CGI) Script output received. (allow DEBUG_CGI_BODY for more details)\033[0m" << std::endl;
        return result;
    }
    return "";
}

void CGI_GET_request(Request &inputRequest, Response &response)
{
	std::string path = inputRequest.getPathString();

	if (check_valid_path(path, inputRequest, response))
	{
 		std::string scriptOutput = executeCGIScript("GET", path, "");
        if (scriptOutput.empty())
        {
            response.fillBadCode(500, inputRequest);
            if (DEBUG_CGI)
                std::cout << "\033[36m\t(CGI) GET REQUEST: output is empty\033[0m" << std::endl;
        }
		else
		{
            response.fillGoodCode(200, inputRequest, response, path, scriptOutput);
            if (DEBUG_CGI && DEBUG_CGI_BODY)
            {
                std::cout << "\033[36m\t(CGI) GET REQUEST: output is not empty\033[0m" << std::endl;
                std::cout << "\033[36m\t(CGI) GET REQUEST: scriptOutput = " << scriptOutput << "\033[0m" << std::endl;
            }
            else if (DEBUG_CGI)
                std::cout << "\033[36m\t(CGI) GET REQUEST: output is not empty. (allow DEBUG_CGI_BODY for more details)\033[0m" << std::endl;
		}
	}
	else
		return;
}

void CGI_POST_request(Request &inputRequest, Response &response)
{
    if (DEBUG_CGI && DEBUG_CGI_BODY)
    {
        std::cout << "\033[31mCGI POST REQUEST: body of the request: " << inputRequest.getBodyString() << "\033[0m" << std::endl;
        std::cout << "\033[31mCGI POST REQUEST: headers of the request: " << "\033[0m" << std::endl;
        std::map<std::string, std::string> headers = inputRequest.getHeadersMap();
        for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        {
            std::cout << it->first << ": " << it->second << std::endl;
        }
    }

    std::string path = inputRequest.getPathString();

    if (check_valid_path(path, inputRequest,response))
    {
        std::string requestBody = inputRequest.getBodyString();
        std::string scriptOutput = executeCGIScript("POST", path, requestBody);
        if (!scriptOutput.empty())
        {
            response.fillGoodCode(200, inputRequest, response, path, scriptOutput);
            if (DEBUG_CGI && DEBUG_CGI_BODY)
            {
                std::cout << "CGI POST REQUEST : output is not empty\n" << std::endl;
                std:: cout << "CGI POST REQUEST : scriptOutput = " << scriptOutput << std::endl;
            }
            else if (DEBUG_CGI)
                std::cout << "CGI POST REQUEST : output is not empty. (allow DEBUG_CGI_BODY for more details)\n" << std::endl;
        }
        else
        {
            response.fillBadCode(500, inputRequest);
			response.setBody("Internal Server Error");
            if (DEBUG_CGI)
                std::cout << "CGI POST REQUEST : output is empty\n" << std::endl;
        }
    }
    else
    {
        if (DEBUG_CGI)
         std::cout << "\033[31mCGI POST REQUEST : invalid path\n" << std::endl;
        response.fillBadCode(404, inputRequest);
    }
}

