#include <iostream>
#include <string>
//implementation of btree
//topic comprehension acquired from 
//Thomas' Cormen book p484 - 491

struct Node
{
    int* keys;
    int order;
    Node** children;
    int keyCount;
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
    newNode->keys = new int[2 * order - 1];
    newNode->children = new Node * [2 * order];
    newNode->keyCount = 0;
    return newNode;
}

void Split(int i, Node* child, Node* parent)
{
    Node* newNode = createNode(child->order, child->isLeaf);
    newNode->keyCount = child->order - 1;

    for (int j = 0; j < child->order - 1; j++)
    {
        newNode->keys[j] = child->keys[j + child->order];
    }
    if (child->isLeaf == false)
    {
        for (int j = 0; j < child->order; j++)
        {
            newNode->children[j] = child->children[j + child->order];
        }
    }

    child->keyCount = child->order - 1;

    for (int j = parent->keyCount; j >= i + 1; j--)
    {
        parent->children[j + 1] = parent->children[j];
    }

    parent->children[i + 1] = newNode;

    for (int j = parent->keyCount - 1; j >= i; j--)
    {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[i] = child->keys[child->order - 1];
    parent->keyCount = parent->keyCount + 1;
}

void Supplement(int k, Node* node)
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
            Split(i + 1, node->children[i + 1], node);
            if (node->keys[i + 1] < k) i++;
        }
        Supplement(k, node->children[i + 1]);
    }
}

void Insert(int v, Btree* btr)
{
    if (btr->root == NULL)
    {
        btr->root = createNode(btr->t, true);
        btr->root->keys[0] = v;
        btr->root->keyCount = 1;
    }
    else
    {
        if (btr->root->keyCount == 2 * btr->t - 1)
        {
            Node* newParent = createNode(btr->t, false);
            newParent->children[0] = btr->root;
            Split(0, btr->root, newParent);

            int i = 0;
            if (newParent->keys[0] < v) i++;
            Supplement(v, newParent->children[i]);

            btr->root = newParent;
        }
        else Supplement(v, btr->root);
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

void Save(Node* node, std::string& saveString)
{
    int i;
    saveString += "( ";
    for (i = 0; i < node->keyCount; i++)
    {
        if (!node->isLeaf) Save(node->children[i], saveString);
        saveString += std::to_string(node->keys[i]);
        saveString += " ";
    }
    if (!node->isLeaf) Save(node->children[i], saveString);
    saveString += ") ";
}

void Load(int order, std::string& loadString, Btree* btree)
{
    for (int i = 0; i < loadString.size(); i++)
    {
        if (isdigit(loadString[i]))
        {
            int j = 0;
            while (isdigit(loadString[i + j])) j++;
            std::string tmp = "";
            for (int k = 0; k < j; k++)
            {
                tmp += loadString[i + k];
            }
            int toPass = stoi(tmp);
            Insert(toPass, btree);
            i += j - 1;
        }
    }
}

void freeMemory(Node* node)
{
    if (!node->isLeaf)
    {
        for (int i = 0; i < node->keyCount; i++) freeMemory(node->children[i]);
    }
    delete node->children;
    delete node->keys;
    delete node;
}

int main()
{
    Btree* btree = new Btree;
    std::string save;
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
        {
            std::string buffor;
            std::cin >> v;
            initializeBtree(v, btree);
            btrinit = 1;
            std::cin.ignore();
            std::getline(std::cin, buffor);
            Load(v, buffor, btree);
            break;
        }

        case 'S':
            if (btrinit == 0) break;
            std::cout << btree->root->order << std::endl;
            Save(btree->root, save);
            std::cout << save << std::endl;
            break;
        case 'X':
            freeMemory(btree->root);
            delete btree;
            break;

        default:
            std::cout << "Wrong command" << std::endl;
            break;
        }
    }
}