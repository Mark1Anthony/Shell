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

std::vector<std::string> shell::parseCommand(const std::string& cmdLine)
{
    std::vector<std::string> args;
    std::string current;

    bool inQuotes = false;


    for (char c : cmdLine) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ' ' && !inQuotes) {
            if (!current.empty()) {
                args.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }

    if (!current.empty()) {
        args.push_back(current);
    }

    return args;
}

bool shell::executeCommand(const std::vector<std::string>& args)
{
    if (args.empty()) {
        return true;
    }

    //Erst interne Befehle testen
    if (executeInternalCommand(args)) {
        return true;
    }

    //sonst versuche einen externen Befehl
    return executeExternalCommand(args);
}

bool shell::executeInternalCommand(const std::vector<std::string>& args)
{
    const std::string& cmd = args[0];


    // shell beenden
    if (cmd == "exit") {
        running = false;
        return true;
    }

    // Verzeichnis wechseln / anzeigen 
    if (cmd == "cd"){
        if (args.size() < 2) {
            char buffer [MAX_PATH];
            if (GetCurrentDirectoryA(MAX_PATH, buffer)) {
                std::cout << buffer << "\n";
            } else {
                std::cout << "Fehler: aktuelles Verzeichnis konnte nicht gelesen werden. \n";
            }
        } else {
            if (!SetCurrentDirectoryA(args[1].c_str())) {
                std::cout << "Verzeichnis nicht gefunden: " << args[1] << "\n";
            }
        }
        return true;
    }

    // Text asugabe
    if (cmd == "echo") {
        for (size_t i = 1; 1 < args.size(); ++i) {
            std::cout << args[i]; 
            if (i + 1 < args.size()) {
                std::cout << " ";
            }
        }
        std::cout << "\n";
        return true;
    }
    
    // Hilfe
    if (cmd == "help") {
        std::cout << "Interne Befehle:\n";
        std::cout << "  exit  - Shell beenden\n";
        std::cout << "  cd    - Verzeichnis anzeigen/wechseln\n";
        std::cout << "  echo  - Text ausgeben\n";
        std::cout << "  help  - diese Hilfe\n";
        std::cout << "\nHinweis: 'dir', 'copy', usw. sind CMD-Befehle,\n";
        std::cout << "nicht deine Shell. Nutze z.B. 'cmd /C dir'.\n";
    return true;
    }

    return false;
}

bool shell::executeExternalCommand(const std::vector<std::string>& args)
{
    std::string commandLine;

    for (const auto& arg : args) {
        if (!commandLine.empty()) {
            commandLine += " ";
        }

        if (arg.find(' ') != std::string::npos) {
            commandLine += "\"" + arg + "\"";
        } else {
            commandLine += arg;
        }
    }

    if (commandLine.empty()) {
        return true;
    }

    STARTUPINFOA si;
    PROCESS_INFORMATION pi; 

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    BOOL success = CreateProcessA(
        nullptr,
        commandLine.data(),
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        nullptr,
        &si,
        &pi
    );

    if (!success) {
        std::cout << "Fehler: CreateProcess fehlgeschlagen (" << GetLastError() <<")\n";
        return false;
    }

    // Auf Prozessende warten
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Optional Exit-Code holen
    DWORD exitCode = 0;
    if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
        // Debug
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;

}