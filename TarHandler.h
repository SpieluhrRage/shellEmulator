#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
#include "fileSysr.h"

class TarHandler {
public:
    // Функция для извлечения файлов из tar-архива и загрузки их в файловую систему
    static bool extractTar(const std::string& tarFilePath, FileSystem& fs) {
        std::ifstream tarFile(tarFilePath, std::ios::binary);
        if (!tarFile.is_open()) {
            std::cerr << "Error: Unable to open tar file." << std::endl;
            return false;
        }

        const size_t BLOCK_SIZE = 512;
        char buffer[BLOCK_SIZE];

        while (tarFile.read(buffer, BLOCK_SIZE)) {
            // Извлечение имени файла из заголовка
            if (std::all_of(buffer, buffer + BLOCK_SIZE, [](char c) { return c == '\0'; })) {
                break;
            }
            std::string fileName(buffer, 100);
            fileName.erase(fileName.find_last_not_of(' ') + 1); // Удаление лишних пробелов

            // Проверка конца архива
            if (fileName.empty()) {
                break;
            }

            // Определение размера файла
            int fileSize = 0;
            std::string sizeStr(buffer + 124, 12);
          /*  std::cout << "Reading tar block header: ";
            for (int i = 0; i < 100; ++i) {
                std::cout << buffer[i];
            }
            std::cout << std::endl;
            std::cout << "Raw size string from tar header: [" << sizeStr << "]" << std::endl;*/
            try {
                sizeStr.erase(std::remove(sizeStr.begin(), sizeStr.end(), ' '), sizeStr.end()); // Удаление пробелов

                fileSize = std::stoi(sizeStr, nullptr, 8); // Размер в восьмеричном формате
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid size value in tar header: [" << sizeStr << "]" << std::endl;
                return false;
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Error: Size value out of range in tar header: [" << sizeStr << "]" << std::endl;
                return false;
            }

            // Создание узла в виртуальной файловой системе
            createNodeInFileSystem(fs, fileName, fileSize > 0);

            // Пропуск данных файла
            int blocksToSkip = (fileSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
            tarFile.seekg(blocksToSkip * BLOCK_SIZE, std::ios::cur);
        }

        tarFile.close();
        return true;
    }

private:
    // Функция для создания узла (файла или директории) в виртуальной файловой системе
    static void createNodeInFileSystem(FileSystem& fs, const std::string& path, bool isFile) {
        std::vector<std::string> tokens = splitPath(path);
        FileSystem::Node* currentNode = fs.getRoot();

        for (const auto& token : tokens) {
            auto it = std::find_if(currentNode->children.begin(), currentNode->children.end(),
                [&](FileSystem::Node* child) { return child->name == token; });

            if (it == currentNode->children.end()) {
                // Если узел не найден, создаём его
                FileSystem::Node* newNode = new FileSystem::Node{ token, !isFile, {}, "", 0755 };
                currentNode->children.push_back(newNode);
                currentNode = newNode;
            }
            else {
                currentNode = *it; // Переходим в существующую директорию
            }
        }
    }

    // Функция для разделения пути на части
    static std::vector<std::string> splitPath(const std::string& path) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(path);
        while (std::getline(tokenStream, token, '/')) {
            if (!token.empty()) {
                tokens.push_back(token);
            }
        }
        return tokens;
    }
};
