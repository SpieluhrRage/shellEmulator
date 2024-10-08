#pragma once
#include<string>
#include<vector>

class FileSystem {

	

	public:
		struct Node {
			std::string name;
			std::string content;
			std::vector<Node*> children;
			int permissions;
			bool isDirectory;
		};

		FileSystem() {
			root = new Node{ "", "", {}, 755, true };
			current = root;
		}

		Node* getRoot() const {
			return root;
		}
		Node* getCurrent() const {
			return current;
		}
		void setCurrent(Node* dir) {
			current = dir;
		}

	private:
		Node* root;
		Node* current;
};