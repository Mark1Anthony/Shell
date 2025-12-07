#pragma once
#include <string>
#include <vector>

class shell{
    public:
        shell();

        void run();

    private: 
        bool running;
        std::string prompt; 
        
        std::vector<std::string> parseCommand(const std::string& cmdLine);
        bool executeCommand(const std::vector<std::string>& args);
        bool executeInternalCommand(const std::vector<std::string>& args);
        bool executeExternalCommand(const std::vector<std::string>& args);

};