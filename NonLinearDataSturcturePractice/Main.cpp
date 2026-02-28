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
	bst.Insert("K", 100);
	bst.Insert("E", 25);
	bst.Insert("F", 31);

	//bst.Find("D");
	//bst.Find("D");
	//bst.Find("B");
	//bst.Find("T");

	bst.Top(1); 
	bst.Top(2);
	bst.Top(3);
	bst.Top(4);
	bst.Top(5);
	bst.Top(6);
	bst.Top(7);
	bst.Top(8);

	bst.Delete("D");
	bst.Top(7);

	bst.Delete("K");
	bst.Top(6);

}

/*
			50
	20				80
		30				100
	25		31

*/