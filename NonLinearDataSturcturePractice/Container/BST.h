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

	// 소멸자에서 사용하는 후위순회 삭제 함수
	bool DeleteRecursive(Node*& node);

	// Delete 함수에서 사용하는 현재 노드의 삭제 함수
	bool DeleteCurrentNode(Node* node);

	// 현재 노드의 하위 트리에서 최대값을 갖는 노드를 찾는 함수
	Node* FindMaxValInNode(Node* node);

	// 현재 노드의 하위 트리에서 최소값을 갖는 노드를 찾는 함수
	Node* FindMinValInNode(Node* node);

private:
	Node* root = nullptr;

};

