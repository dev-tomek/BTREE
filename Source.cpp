#include <iostream>

struct Node
{
    int* keys; //key array
    int order; //order
    Node** children; //array of children
    int keyCount; //number of keys in a node
    bool isLeaf;
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
    newNode->order = order;
    newNode->isLeaf = isLeaf;
    newNode->keys = new int[2 * order - 1]; //allocating max 2*order-1 int memory for keys
    newNode->children = new Node * [2 * order]; //allocating memory for children
    newNode->keyCount = 0;
    return newNode;
}

void splitChild(int i, Node* child, Node* parent)
{
    Node* z = createNode(child->order, child->isLeaf);
    z->keyCount = child->order - 1;

    for (int j = 0; j < child->order - 1; j++)
    {
        z->keys[j] = child->keys[j + child->order];
    }

    if (child->isLeaf == false)
    {
        for (int j = 0; j < child->order; j++)
        {
            z->children[j] = child->children[j + child->order];
        }
    }

    child->keyCount = child->order - 1;

    for (int j = parent->keyCount; j >= i + 1; j--)
    {
        parent->children[j + 1] = parent->children[j];
    }

    parent->children[i + 1] = z;

    for (int j = parent->keyCount - 1; j >= i; j--)
    {
        parent->keys[j + 1] = parent->keys[j];
    }


    parent->keys[i] = child->keys[child->order - 1];
    parent->keyCount = parent->keyCount + 1;
}

void InsertDeficient(int k, Node* node)
{
    int i = node->keyCount - 1;

    if (node->isLeaf == true) {
        while (i >= 0 && node->keys[i] > k)
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        node->keys[i + 1] = k;
        node->keyCount = node->keyCount + 1;
    }
    else
    {
        while (i >= 0 && node->keys[i] > k) i--;

        if (node->children[i + 1]->keyCount == 2 * node->order - 1)
        {
            splitChild(i + 1, node->children[i + 1], node);
            if (node->keys[i + 1] < k) i++;
        }
        InsertDeficient(k, node->children[i + 1]);
    }
}

void Insert(int v, Btree* btr)
{
    if (btr->root == NULL) //when there is no root
    {
        btr->root = createNode(btr->t, true); //creating root node of order t
        btr->root->keys[0] = v; //setting first key of a node to the value
        btr->root->keyCount = 1; //saving that root has 1 element
    }
    else
    {
        if (btr->root->keyCount == 2 * btr->t - 1) //when the node is full
        {
            Node* newParent = createNode(btr->t, false); //creating a new parent node
            newParent->children[0] = btr->root; //his first children is root
            splitChild(0, btr->root, newParent);

            int i = 0;
            if (newParent->keys[0] < v) i++;
            InsertDeficient(v, newParent->children[i]);

            btr->root = newParent;
        }
        else InsertDeficient(v, btr->root);
    }
}

Node* Search(int v, Node* root)
{
    int i = 0;
    while (i < root->keyCount && v > root->keys[i]) i++;
    if (root->keys[i] == v) return root;
    if (root->isLeaf == true) return NULL;
    return Search(v, root->children[i]);
}

void Print(Node* node)
{
    int i;
    for (i = 0; i < node->keyCount; i++)
    {
        if (!node->isLeaf) Print(node->children[i]);
        std::cout << node->keys[i] << " ";
    }
    if (!node->isLeaf) Print(node->children[i]);
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
            if (Search(v, btree->root) != NULL) std::cout << v << " +" << std::endl;
            else std::cout << v << " -" << std::endl;
            break;

        case 'P':
            if (btrinit == 0) break;
            Print(btree->root);
            std::cout << std::endl;
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