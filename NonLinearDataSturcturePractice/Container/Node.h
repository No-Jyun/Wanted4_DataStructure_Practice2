#pragma once

#include <string>
#include <vector>

class Node
{
	friend class BST;

public:
	Node(const std::string& playerName = "", const int score = 0, Node* const parent = nullptr)
		: score(score), parent(parent)
	{
		playerNames.emplace_back(playerName);
	}

	~Node()
	{
		// 메모리 정리는 BST 에서 담당
	}

	// 연산자 오버로딩
	bool operator<(const Node& other)
	{
		return score < other.score;
	}

	bool operator<=(const Node& other)
	{
		return score <= other.score;
	}

	bool operator>(const Node& other)
	{
		return score > other.score;
	}

	bool operator>=(const Node& other)
	{
		return score >= other.score;
	}

private:
	void AddSameScorePlayer(const std::string& playerName)
	{
		playerNames.emplace_back(playerName);
	}

	void DeletePlayer(const std::string& playerName)
	{
		for (auto iter = playerNames.begin(); iter != playerNames.end(); iter++)
		{
			if (*iter == playerName)
			{
				playerNames.erase(iter);
				return;
			}
		}
	}

	bool FindPlayerInThisScore(const std::string& playerName)
	{
		int length = static_cast<int>(playerNames.size());

		for (int i = 0;i < length;i++)
		{
			if (playerName == playerNames[i])
			{
				return true;
			}
		}
		return false;
	}

	bool IsEmpty()
	{
		return playerNames.empty();
	}

private:
	std::vector<std::string> playerNames;
	int score = 0;

	Node* leftChild = nullptr;
	Node* rightChild = nullptr;
	Node* parent = nullptr;
};