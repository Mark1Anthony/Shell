#include "shell.h"
#include <iostream>
#include <windows.h>

shell::shell()
    : running(true), prompt("MyShell> ")
    {
    }

void shell::run()
{
    std::string cmdLine;
    while (running){
        std::cout << prompt;

        if (!std::getline(std::cin, cmdLine)) {
            break;
        }
        if (cmdLine.empty()) {
            continue;
        }

        // in Tokens zerlegen
        std::vector<std::string> args = parseCommand(cmdLine);
        if (args.empty()) {
            continue;
        }
        //Befehl ausführen
        if (!executeCommand(args)){
            std::cout << "Fehler: Befehl nicht gefunden oder konnte nicht ausgeführt werden. \n";
        }
    }
}