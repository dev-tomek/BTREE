#include <iostream>

struct Node
{
	int* keys; //key array
	int t; //order
	Node** C; //array of children
	int n; //number of keys in a node
	bool leaf;
};

struct Btree
{
	Node* root;
	int t;
};

void initializeBtree(int order, Btree* btr)
{
	btr->root = NULL;
	btr->t = order;
}

Node* createNode(int order, bool isLeaf)
{
	Node* newNode = new Node;
	newNode->t = order;
	newNode->leaf = isLeaf;
	newNode->keys = new int[2 * order - 1]; //allocating max 2*order-1 int memory for keys
	newNode->C = new Node* [2 * order]; //allocating memory for children
	newNode->n = 0;
	return newNode;
}

void splitChild(int i, Node* y, Node* xd) 
{
	Node* z = createNode(y->t, y->leaf);
	z->n = y->t - 1;

	for (int j = 0; j < y->t - 1; j++)
	{
		z->keys[j] = y->keys[j + y->t];
	}
		
	if (y->leaf == false) 
	{
		for (int j = 0; j < y->t; j++)
		{
			z->C[j] = y->C[j + y->t];
		}	
	}

	y->n = y->t - 1;

	for (int j = xd->n; j >= i + 1; j--)
	{
		xd->C[j + 1] = xd->C[j];
	}
		
	xd->C[i + 1] = z;

	for (int j = xd->n - 1; j >= i; j--)
	{
		xd->keys[j + 1] = xd->keys[j];
	}
		

	xd->keys[i] = y->keys[y->t - 1];
	xd->n = xd->n + 1;
}

void insertNonFull(int k, Node* node) 
{
	int i = node->n - 1;

	if (node->leaf == true) {
		while (i >= 0 && node->keys[i] > k) 
		{
			node->keys[i + 1] = node->keys[i];
			i--;
		}

		node->keys[i + 1] = k;
		node->n = node->n + 1;
	}
	else 
	{
		while (i >= 0 && node->keys[i] > k) i--;

		if (node->C[i + 1]->n == 2* node->t - 1) 
		{
			splitChild(i + 1, node->C[i + 1], node);
			if (node->keys[i + 1] < k) i++;
		}
		insertNonFull(k, node->C[i + 1]);
	}
}

void Insert(int v, Btree* btr) 
{
	if (btr->root == NULL) //when there is no root
	{
		btr->root = createNode(btr->t, true); //creating root node of order t
		btr->root->keys[0] = v; //setting first key of a node to the value
		btr->root->n = 1; //saving that root has 1 element
	}
	else
	{
		if (btr->root->n == 2 * btr->t - 1) //when the node is full
		{
			Node* s = createNode(btr->t, false); //creating a new parent node
			s->C[0] = btr->root; //his first children is root
			splitChild(0, btr->root, s);

			int i = 0;
			if (s->keys[0] < v) i++;
			insertNonFull(v, s->C[i]);

			btr->root = s;
		}
		else insertNonFull(v, btr->root);
	}
}

Node* Search(int k, Node* root) 
{
	int i = 0;
	while (i < root->n && k > root->keys[i]) i++;

	if (root->keys[i] == k) return root;

	if (root->leaf == true) return NULL;

	return Search(k, root->C[i]);
}

int main()
{
	Btree* btree = new Btree;
	char c = ' ';
	int v;
	int btrinit = 0;
	while (c != 'X')
	{
		std::cin >> c;
		switch (c)
		{
		case 'I':
			if (btrinit == 1) break;
			std::cin >> v;
			initializeBtree(v, btree);
			btrinit = 1;
			break;
		case 'A':
			if (btrinit == 0) break;
			std::cin >> v;
			Insert(v, btree);
			break;
		case '?':
			if (btrinit == 0) break;
			std::cin >> v;
			if (Search(v, btree->root) != NULL)
			{
				std::cout << v << " +" << std::endl;
			}
			else
			{
				std::cout << v << " -" << std::endl;
			}
			break;
		case 'P':
			break;
		case 'L':
			std::cin >> v;
			break;
		case 'S':
			break;
		case 'R':
			std::cin >> v;
			break;
		case 'X':
			break;
		case 'C':
			break;

		default:
			std::cout << "Wrong command" << std::endl;
			break;
		}
	}
}