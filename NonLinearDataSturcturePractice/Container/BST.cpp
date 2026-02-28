#include "BST.h"

#include <iostream>
#include <fstream>
#include <Windows.h>

BST::BST()
{
}

BST::~BST()
{
	// 전체 순회하며 모든 노드 삭제
	DeleteRecursive(root);
	root = nullptr;
}

void BST::ReadPlayerData(std::string filename)
{
	std::cout << "저장된 플레이어 정보를 불러옵니다..\n";

	// 파일 로드
	std::string path = "../Resources/" + filename;
	std::ifstream file(path);

	// 예외 처리
	if (!file.is_open())
	{
		std::cout << "파일 로드 오류..\n";
		__debugbreak();
	}

	std::string line;

	// 첫 줄은 아무 처리도 하지 않고 넘어감
	getline(file, line);

	// 한 줄씩 읽기
	while (getline(file, line))
	{
		// 개행 문자 처리
		size_t rPos = line.find('\r');
		if (rPos != std::string::npos)
		{
			line.erase(rPos);
		}

		// 빈 줄이면 스킵
		if (line.empty())
		{
			continue;
		}

		// CSV 파싱 (쉼표 기준 분리)
		std::vector<std::string> row;
		size_t start = 0;
		size_t end = 0;

		// start 인덱스부터 ',' 찾기
		while ((end = line.find(',', start)) != std::string::npos)
		{
			row.emplace_back(line.substr(start, end - start));
			start = end + 1;
		}

		// 마지막 데이터 추가
		row.emplace_back(line.substr(start));

		// BST에 삽입
		Insert(row[0], stoi(row[1]));
	}

	// 파일 닫기
	file.close();

	std::cout << "플레이어 정보를 전부 저장했습니다.\n";

	DWORD delay = static_cast<DWORD>(1.0f * 1000);
	Sleep(delay);
}

void BST::StartBST()
{
	ReadPlayerData("PlayerData.csv");
	system("cls");

	std::string order = "";

	while (1)
	{
		std::cout << "수행할 명령을 입력하세요\n";
		std::cout << "insert name score : 플레이어 정보를 저장합니다. \n"
			<< "delete name : 플레이어 정보를 삭제합니다. \n"
			<< "find name : 플레이어 정보를 검색합니다. \n"
			<< "top n : 상위 n명의 플레이어 정보를 출력합니다. \n"
			<< "quit : 프로그램을 종료합니다\n";

		std::cout << "명령 : ";
		std::cin >> order;

		std::string name;
		if (order == "insert")
		{
			int score;
			std::cin >> name >> score;

			std::cout << "------------------------------\n";
			Insert(name, score);
		}
		else if (order == "delete")
		{
			std::cin >> name;

			std::cout << "------------------------------\n";
			Delete(name);
		}
		else if (order == "find")
		{
			std::cin >> name;

			std::cout << "------------------------------\n";
			Find(name);
		}
		else if (order == "top")
		{
			int n;
			std::cin >> n;

			std::cout << "------------------------------\n";
			Top(n);
		}
		else if (order == "quit")
		{
			system("cls");

			std::cout << "\n프로그램을 종료합니다..\n";
			DWORD delay = static_cast<DWORD>(1.0f * 1000);
			Sleep(delay);
			break;
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			std::cout << "\n!!!!!!!!! Invalid Order : " << order << "!!!!!!!!!\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
				FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}

		DWORD delay = static_cast<DWORD>(1.0f * 1000);
		Sleep(delay);
		std::cout << "\n";
	}
}

bool BST::Insert(const std::string& name, const int score)
{
	// root 없으면 생성
	if (!root)
	{
		root = new Node(name, score);
		bstSize++;
		return true;
	}

	Node* currentNode = nullptr;
	// 중복 이름 삽입 허용 안함
	if (FindRecursive(root, name, currentNode))
	{
		std::cout << "중복된 이름의 삽입은 허용하지 않습니다.\n";
		return false;
	}

	bstSize++;
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
	bstSize--;
	std::cout << "플레이어 " << name << " 을 삭제했습니다!\n";

	// 현재 점수 노드에 플레이어 존재하면 처리 종료
	if (!outNode->IsEmpty())
	{
		return true;
	}

	// 현재 점수 노드에 플레이어가 비었다면 노드 삭제 처리
	return DeleteCurrentNode(outNode);
}

bool BST::Find(const std::string& findName)
{
	Node* outNode = nullptr;
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
	// 예외 처리
	if (bstSize == 0)
	{
		std::cout << "현재 BST가 비었습니다!\n"
			<< "먼저 insert 해주세요!\n";
		return;
	}
	if (0 >= printNum || printNum > bstSize)
	{
		std::cout << "현재 BST에 저장된 플레이어 수는 " << bstSize << "명 입니다!\n"
			<< "0 보다 크고 " << bstSize << " 보다 같거나 작은 수를 입력해주세요!\n";
		return;
	}

	std::cout << "점수가 높은 순으로 " << printNum << "명을 출력합니다!\n";

	std::vector<std::pair<std::string, int>> printVector = TopRecursive(root, printNum);

	for (std::pair<std::string, int> data : printVector)
	{
		std::cout << "플레이어 이름 : " << data.first << ", 플레이어 점수 : " << data.second << "\n";
	}
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

std::vector<std::pair<std::string, int>> BST::TopRecursive(Node* node, const int nowNum)
{
	if (!node || nowNum == 0)
	{
		return {};
	}
	
	// 오른쪽 자식 트리에서 n명 저장
	std::vector<std::pair<std::string, int>> rightPlayer = TopRecursive(node->rightChild, nowNum);

	// 오른쪽 자식 트리에서 저장된 n명을 제외하고 남은 저장해야할 플레이어 수
	int restPlayerNum = nowNum - static_cast<int>(rightPlayer.size());

	// 현재 노드에서 플레이어 저장
	for (int i = 0;i < static_cast<int>(node->playerNames.size());i++)
	{
		if (restPlayerNum == 0)
		{
			break;
		}
		
		restPlayerNum--;
		rightPlayer.emplace_back(std::make_pair(node->playerNames[i], node->score));
	}

	// 왼쪽 자식 트리에서 남은 플레이어 저장
	std::vector<std::pair<std::string, int>> leftPlayer = TopRecursive(node->leftChild, restPlayerNum);

	// 저장한 플레이어 합치기
	for (std::pair<std::string, int> data : leftPlayer)
	{
		rightPlayer.emplace_back(data);
	}

	return rightPlayer;
}
