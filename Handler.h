#pragma once
#include<iostream>
#include "fileSysr.h"
#include <sstream>
#include <algorithm>

class CommandHandler {
public:
    static void execute(const std::string& command, FileSystem& fs, std::string& currentDirectory) {
        if (command == "ls") {
            ls(fs, currentDirectory);
        }
        else if (command.find("cd") == 0) {
            cd(fs, command.substr(3), currentDirectory);
        }
        else if (command == "exit") {
            exitShell();
        }
        else if (command.find("chmod") == 0) {
            chmod(fs, command.substr(6), currentDirectory);
        }
        else if (command.find("wc") == 0) {
            wc(fs, command.substr(3), currentDirectory);
        }
        else if (command.find("exitShell" == 0)) {
            exitShell();
        }
        else {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }

    static void ls(FileSystem& fs, const std::string& currentDirectory) {
        FileSystem::Node* dir = fs.getCurrent();
        if (!dir->isDirectory) {
            std::cerr << "Error: " << currentDirectory << " is not a directory." << std::endl;
            return;
        }

        for (const auto& child : dir->children) {
            std::cout << child->name << (child->isDirectory ? "/" : "") << std::endl;
        }
    }

    static void cd(FileSystem& fs, const std::string& targetDirectory, std::string& currentDirectory) {
        FileSystem::Node* targetNode = fs.changeDirectory(targetDirectory);
        if (targetNode) {
            currentDirectory = targetDirectory;
        }
        else {
            std::cerr << "Error: Directory not found: " << targetDirectory << std::endl;
        }
    }

    static void exitShell() {
        std::cout << "Exiting the shell. Goodbye!" << std::endl;
        exit(0); // Завершает выполнение программы
    }

    static void chmod(FileSystem& fs, const std::string& args, const std::string& currentDirectory) {
        std::istringstream iss(args);
        std::string permissionsStr, filePath;
        iss >> permissionsStr >> filePath;

        if (permissionsStr.empty() || filePath.empty()) {
            std::cerr << "Usage: chmod <permissions> <file>" << std::endl;
            return;
        }

        int permissions = std::stoi(permissionsStr, nullptr, 8); // Преобразование строки в восьмеричное число
        FileSystem::Node* targetNode = fs.findNode(filePath);

        if (targetNode) {
            targetNode->permissions = permissions;
            std::cout << "Permissions for " << filePath << " changed to " << permissionsStr << std::endl;
        }
        else {
            std::cerr << "Error: File not found: " << filePath << std::endl;
        }
    }

    static void wc(FileSystem& fs, const std::string& filePath, const std::string& currentDirectory) {
        FileSystem::Node* targetNode = fs.findNode(filePath);
        if (!targetNode || targetNode->isDirectory) {
            std::cerr << "Error: " << filePath << " is not a valid file." << std::endl;
            return;
        }

        int wordCount = 0;
        std::istringstream iss(targetNode->content);
        std::string word;
        while (iss >> word) {
            ++wordCount;
        }
        std::cout << "Word count: " << wordCount << std::endl;
    }
};