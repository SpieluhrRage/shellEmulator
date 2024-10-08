#pragma once
#include "fileSysr.h"
#include "Handler.h"
#include <iostream>
#include <string>

class Shell {

public:
	Shell(const std::string& user, const std::string& host, FileSystem& fs)
		: username(user), hostname(host), fileSystem(fs), currentDirectory("/") {}

	void run() {
		std::string command;
		while (true) {
			std::cout << username << "@" << hostname << ":" << currentDirectory << "$";
			std::getline(std::cin, command);

			if (command == "exit") {
				break;
			}
			CommandHandler::execute(command, fileSystem, currentDirectory);
		}
	}
private:
	std::string username;
	std::string hostname;
	FileSystem& fileSystem; 
	std::string currentDirectory;

};