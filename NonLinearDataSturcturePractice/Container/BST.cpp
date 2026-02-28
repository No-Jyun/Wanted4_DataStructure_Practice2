#include "BST.h"

#include <iostream>

BST::BST()
{
}

BST::~BST()
{
	// 전체 순회하며 모든 노드 삭제
	DeleteRecursive(root);
	root = nullptr;
}

bool BST::Insert(const std::string& name, const int score)
{
	// root 없으면 생성
	if (!root)
	{
		root = new Node(name, score);
		return true;
	}

	Node* currentNode = nullptr;
	// 중복 이름 삽입 허용 안함
	if (FindRecursive(root, name, currentNode))
	{
		std::cout << "중복된 이름의 삽입은 허용하지 않습니다.\n";
		return false;
	}

	currentNode = root;
	Node* exNode = currentNode;
	while (currentNode)
	{
		exNode = currentNode;

		// 점수가 같다면 해당 노드에 이름 추가하고 종료
		if (currentNode->score == score)
		{
			currentNode->AddSameScorePlayer(name);
			return true;
		}
		// 점수가 더 크다면 오른쪽 자식 노드로 이동
		else if (currentNode->score < score)
		{
			currentNode = currentNode->rightChild;
		}
		// 점수가 더 작다면 왼쪽 자식 노드로 이동
		else
		{
			currentNode = currentNode->leftChild;
		}
	}

	// 결국 currentNode가 null인 곳까지 와서 탈출
	// exNode가 parentNode
	currentNode = new Node(name, score, exNode);
	if (exNode->score < score)
	{
		exNode->rightChild = currentNode;
	}
	else
	{
		exNode->leftChild = currentNode;
	}

	return true;
}

bool BST::Delete(const std::string& name)
{
	Node* outNode = nullptr;

	if (!FindRecursive(root, name, outNode))
	{
		std::cout << name << " 의 이름을 가진 플레이어가 없습니다!\n";
		return false;
	}

	// 현재 점수 노드에서 해당 이름을 가진 플레이어 제거
	outNode->DeletePlayer(name);
	std::cout << "플레이어 " << name << " 을 삭제했습니다!\n";

	// 현재 점수 노드에 플레이어 존재하면 처리 종료
	if (!outNode->IsEmpty())
	{
		return true;
	}

	// 현재 점수 노드에 플레이어가 비었다면 노드 삭제 처리
	return DeleteCurrentNode(outNode);
}

bool BST::Find(const std::string& findName, Node*& outNode)
{
	if (FindRecursive(root, findName, outNode))
	{
		std::cout << "플레이어 이름 : " << findName << ", 플레이어 점수 : " << outNode->score << "\n";
		return true;
	}
	else
	{
		std::cout << findName << " 의 이름을 가진 플레이어를 찾지 못했습니다!\n";
		return false;
	}
}

void BST::Top(const int printNum)
{
	// Todo: 구현 필요
}

bool BST::FindRecursive(Node* node, const std::string& findName, Node*& outNode)
{
	if (!node)
	{
		return false;
	}

	// 현재 점수 노드에서 발견시
	if (node->FindPlayerInThisScore(findName))
	{
		outNode = node;
		return true;
	}

	// 왼쪽 자식 탐색
	if (FindRecursive(node->leftChild, findName, outNode))
	{
		return true;
	}

	// 오른쪽 자식 탐색
	if (FindRecursive(node->rightChild, findName, outNode))
	{
		return true;
	}

	return false;
}

bool BST::DeleteRecursive(Node*& node)
{
	if (!node)
	{
		return false;
	}

	// 왼쪽 자식 트리 삭제
	DeleteRecursive(node->leftChild);

	// 오른쪽 자식 트리 삭제
	DeleteRecursive(node->rightChild);

	// 자기 자신 삭제
	delete node;
	node = nullptr;
	return true;
}

bool BST::DeleteCurrentNode(Node* node)
{
	// 자식 노드가 없는 경우
	if (!node->leftChild && !node->rightChild)
	{
		// 루트 노드인 경우
		if (node == root)
		{
			root = nullptr;
		}
		else
		{
			// 부모 노드의 연결 끊기
			if (node->parent->leftChild == node)
			{
				node->parent->leftChild = nullptr;
			}
			else
			{
				node->parent->rightChild = nullptr;
			}
		}
		delete node;
		node = nullptr;
		return true;
	}

	Node* outNode = nullptr;
	// 왼쪽 자식만 있는 경우
	// 왼쪽 자식트리에서 최대값을 가진 노드와 교체
	if (node->leftChild && !node->rightChild)
	{
		outNode = FindMaxValInNode(node->leftChild);
	}

	// 오른쪽 자식만 있는 경우
	// 둘다 있는 경우
	// 오른쪽 자식 트리에서 최소값을 가진 노드와 교체
	else
	{
		outNode = FindMinValInNode(node->rightChild);
	}

	// 노드 정보 교환
	std::swap<int>(node->score, outNode->score);
	std::swap<std::vector<std::string>>(node->playerNames, outNode->playerNames);

	// 교환한 노드 삭제
	return DeleteCurrentNode(outNode);
}

Node* BST::FindMaxValInNode(Node* node)
{
	Node* currentNode = node;
	while (true)
	{
		if (!currentNode->rightChild)
		{
			break;
		}
		currentNode = currentNode->rightChild;
	}

	return currentNode;
}

Node* BST::FindMinValInNode(Node* node)
{
	Node* currentNode = node;
	while (true)
	{
		if (!currentNode->leftChild)
		{
			break;
		}
		currentNode = currentNode->leftChild;
	}

	return currentNode;
}
