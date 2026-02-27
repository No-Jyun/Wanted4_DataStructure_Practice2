#include <iostream>
#include "Container/BST.h"

int main()
{
	BST bst;
	bst.Insert("R", 50);
	bst.Insert("A", 20);
	bst.Insert("B", 30);
	bst.Insert("C", 80);
	bst.Insert("D", 100);
	bst.Insert("E", 25);
	bst.Insert("F", 31);

	// Todo: Delete 처리 이상함
	bst.Delete("A");
}