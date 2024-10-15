#include "fileSysr.h"
#include "Shell.h"
#include <iostream>
#include <string>
#include "TarHandler.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: shell_emulator <username> <hostname> <path_to_tar>" << std::endl;
        return 1;
    }

    std::string username = argv[1];
    std::string hostname = argv[2];
    std::string tarPath = argv[3];

    // Создание файловой системы и загрузка архива tar
    FileSystem fs;
    if (!TarHandler::extractTar(tarPath, fs)) {
        std::cerr << "Error: Could not load virtual file system from tar archive." << std::endl;
        return 1;
    }

    // Инициализация Shell
    Shell shell(username, hostname, fs);
    shell.run();

    return 0;
}
