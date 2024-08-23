#include "../include/webserv.hpp"

int g_stop = 0;

void signalHandler(int signum)
{
    if (signum == SIGINT)
    {
        g_stop = 1;
    }
}

int main(int argc, char **argv)
{
    if (argc < 1 || argc > 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }
    try
    {
        Config config;
        if (argc == 2)
            config.setConfigFile(argv[1]);
        config.parseConfig();
        //config.printConfig();

        ServerManager serverManager(config);

        std::signal(SIGINT, signalHandler);
        serverManager.startServers();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}