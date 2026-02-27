#include "BST.h"

#include <iostream>

BST::BST()
{
}

BST::~BST()
{
    // 전체 순회하며 모든 노드 삭제
    DeleteRecursive(root);
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
    if (Find(name, currentNode))
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
    currentNode = new Node(name, score);
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

    if (!Find(name, outNode))
    {
        std::cout << name << " 의 이름을 가진 플레이어가 없어서 삭제하지 못했습니다!\n";
        return false;
    }
    // Todo: 같은 점수에 있는 플레이어 고려 필요
    // 노드를 한번에 삭제하면 안됨
    return DeleteRecursive(outNode);
}

bool BST::Find(const std::string& findName, Node*& outNode)
{
    return FindRecursive(root, findName, outNode);
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
    FindRecursive(node->leftChild, findName, outNode);

    // 오른쪽 자식 탐색
    FindRecursive(node->rightChild, findName, outNode);
}

bool BST::DeleteRecursive(Node* node)
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
