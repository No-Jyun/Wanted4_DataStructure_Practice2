#pragma once

#include "Node.h"

class BST
{
public:
	BST();
	~BST();

	bool Insert(const std::string& name, const int score);

	bool Delete(const std::string& name);

	bool Find(const std::string& findName, Node*& outNode);

	void Top(const int printNum);

private:
	// Find 함수에서 사용하는 전위순회 탐색 함수
	bool FindRecursive(Node* node, const std::string& findName, Node*& outNode);

	// Delete 함수에서 사용하는 후위순회 삭제 함수
	bool DeleteRecursive(Node* node);

private:
	Node* root = nullptr;

};

