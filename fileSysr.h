#pragma once
#include<string>
#include<vector>
#include <algorithm>
class FileSystem {
public:
    struct Node {
        std::string name;
        bool isDirectory;
        std::vector<Node*> children;
        std::string content;
        int permissions;

        // Конструктор для инициализации узла
        Node(const std::string& name, bool isDirectory, const std::vector<Node*>& children, const std::string& content, int permissions)
            : name(name), isDirectory(isDirectory), children(children), content(content), permissions(permissions) {}
    };

    FileSystem() {
        root = new Node("/", true, {}, "", 0755);
        current = root;
    }

    Node* getRoot() const { return root; }
    Node* getCurrent() const { return current; }

    // Метод для изменения текущей директории
    Node* changeDirectory(const std::string& path) {
        Node* targetNode = findNode(path);
        if (targetNode && targetNode->isDirectory) {
            current = targetNode;
            return current;
        }
        return nullptr; // Возвращает nullptr, если директория не найдена или это не директория
    }

    // Метод для поиска узла по пути
    Node* findNode(const std::string& path) {
        if (path.empty() || path == "/") {
            return root; // Возвращает корневой узел для пустого пути или "/"
        }

        Node* node = (path[0] == '/') ? root : current; // Начинаем с root, если путь абсолютный
        std::vector<std::string> tokens = splitPath(path);

        for (const auto& token : tokens) {
            auto it = std::find_if(node->children.begin(), node->children.end(),
                [&](Node* child) { return child->name == token; });
            if (it == node->children.end()) {
                return nullptr; // Узел не найден
            }
            node = *it;
        }
        return node;
    }

private:
    Node* root;
    Node* current;

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