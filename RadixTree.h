#ifndef RADIX_H
#define RADIX_H

#include <map>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>

template <typename ValueType>
class RadixTree {
public:
	RadixTree() { root = nullptr; };
	~RadixTree() {
		// add destructor to prevent memory leak!!!!!!!
		deleteNode(root);
	};

	void insert(std::string key, const ValueType& value) {
		db_data.push_back(value);
		auto it = db_data.end();
		it--;
		ValueType* data = &(*it);

		// if the radix tree is empty, allocate a new node and insert it to the root node
		if (root == nullptr) {
			root = new Node(key, data);
			return;
		}

		Node* currNode = root;
		Node* prevNode = root;

		if (root->prefix == "") {
			if (root->children[key[0] - NULL] != nullptr) {
				currNode = root->children[key[0] - NULL];
				prevNode = root;
			}
			else {
				root->children[key[0] - NULL] = new Node(key, data);
				return;
			}
		}
		for (;;) {
			// if the transition string match the entire prefix
			if (key == currNode->prefix) {
				// insert the data into current node
				currNode->value = data;
				currNode->EndMark = true;
				break;
			}

			std::string matched = findMatchingPrefix(key, currNode->prefix);
			std::string unMatchedPrefix = (currNode->prefix).substr(matched.size(), currNode->prefix.size() - matched.size());

			// doesn't match at all

			// if the transition string and prefix only share common parts
			if (matched == "" || key != "" && unMatchedPrefix != "") {
				// allocate a new node with the matched prefix
				Node* newNode = new Node(matched);
				// allocate a new node with the prefix equal to remainder of the key string
				Node* newPrefixNode = new Node(key, data);
				// change the prefix of the current node to the unmatched part of the prefix
				currNode->prefix = unMatchedPrefix;
				// insert the new node with the remainder of the key string and updated current Node into the new node
				newNode->children[key[0] - NULL] = newPrefixNode;
				newNode->children[unMatchedPrefix[0] - NULL] = currNode;
				if (currNode == root) {
					root = newNode;
				}
				else {
					currNode = newNode;
					prevNode->children[currNode->prefix[0] - NULL] = currNode;
				}
				break;
			}
			// if the transition string is contained by the prefix
			else if (key == "" && unMatchedPrefix != "") {
				// allocate a new node with matched prefix with data inserted
				Node* newNode = new Node(matched, data);
				// replace prefix in current node with unmatched part of the prefix
				// currNode also becomes a children of new node
				currNode->prefix = unMatchedPrefix;
				newNode->children[unMatchedPrefix[0] - NULL] = currNode;
				// if the current Node is the root poitner
				if (currNode == root) {
					// root points to new node
					root = newNode;
				}
				else {
					// prev node point to new node;
					prevNode->children[newNode->prefix[0] - NULL] = newNode;
				}
				break;
			}
			// if the prefix is contained by the transition string
			else if (matched == currNode->prefix) {
				// allocate a new node with the transition string
				// add the new node to the children node of the current node;
				if (currNode->children[key[0] - NULL] == nullptr) {
					Node* newNode = new Node(key, data);
					currNode->children[key[0] - NULL] = newNode;
					break;
				}
				// current node becomes new node
				// previous node becomes current node  

				prevNode = currNode;
				currNode = currNode->children[key[0] - NULL];
			}
		}
	};

	ValueType* search(std::string key) const {
		Node* currNode;
		// if the radix tree is empty, return null
		if (root == nullptr) {
			return nullptr;
		}
		// if the root node does not contain any prefix, go to the path of the first index of key
		if (root->prefix == "") {
			// if the root doesn't have childen that correspond to the first index of key, return false
			if (root->children[key[0] - NULL] == nullptr)
				return nullptr;
			currNode = root->children[key[0] - NULL];
		}
		// if the root does contain prefix, go to the root node
		else {
			currNode = root;
		}

		for (;;) {
			// find the matching prefix of current key and prefix of current node
			std::string matched = findMatchingPrefix(key, currNode->prefix);
			// if the prefix of current node doesn't have any matches with current key, then it doesn't exist
			if (matched == "" || matched != currNode->prefix)
				return nullptr;
			// if all letters of currrent key has been matched, then we found our target node
			// all node that holds some value has been marked by endmark
			if (key == "") {
				// if endmark is true, then your target node holds a value
				if (currNode->EndMark)
					return currNode->value;
				// else there is no value associated with the key;
				return nullptr;
			}
			currNode = currNode->children[key[0] - NULL];
			if (currNode == nullptr)
				return nullptr;
		}
	};
private:
	struct Node {
		Node(std::string p) {
			prefix = p;
			for (int i = 0; i < 128; i++) {
				children[i] = nullptr;
			}
		};
		Node(std::string p, ValueType* v) {
			prefix = p;
			value = v;
			EndMark = true;
			for (int i = 0; i < 128; i++) {
				children[i] = nullptr;
			}
		};

		std::string prefix;
		ValueType* value;
		bool EndMark = false;
		Node* children[128];
	};

	// return matching part and reformat the first string to the unmatched part
	std::string findMatchingPrefix(std::string& checkStr, const std::string& matchStr) const {
		// iteration is equal to the size of the string with shorter length
		int iteration = checkStr.size() < matchStr.size() ? checkStr.size() : matchStr.size();
		std::string matching = "";

		for (int i = 0; i < iteration; i++) {
			// if the char at index 0 (since we are reformatting checkStr to only have
			// the unmatched part)
			if (checkStr.at(0) == matchStr.at(i)) {
				matching += matchStr.at(i);
				checkStr = checkStr.substr(1, checkStr.size() - 1);
			}
			else {
				break;
			}
		}
		return matching;
	}

	// destructor helper function
	void deleteNode(Node* curr) {
		if (curr == nullptr)
			return;
		for (int i = 0; i < 128; i++) {
			if (curr->children[i] != nullptr)
				deleteNode(curr->children[i]);
		}
		delete curr;
	}

	// private member variables
	std::list<ValueType> db_data;
	Node* root;
};

#endif
