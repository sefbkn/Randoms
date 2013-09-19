#include "BinaryTree.hpp"

Node* NewNode(char* key, char* value)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(newNode != NULL)
    {
        memset(newNode, 0, sizeof(Node));

        newNode->key   = (char*)malloc(strlen(key)+1);
        newNode->value = (char*)malloc(strlen(value)+1);
        newNode->lnode = NULL;
        newNode->rnode = NULL;
        newNode->pnode = NULL;

        strcpy(newNode->key,   key);
        strcpy(newNode->value, value);
    }

    return newNode;
}

Node* AddNode(Node* node, char* key, char* value)
{
    Node** baseNode = &node;
    Node* lastNode = *baseNode;

    while(*baseNode != NULL)
    {
        lastNode = *baseNode;
        int result = strcmp(key, (*baseNode)->key);
        if(result == 0)     return *baseNode;
        else if(result < 0) baseNode = &((*baseNode)->lnode);
        else if(result > 0) baseNode = &((*baseNode)->rnode);
    }

    *baseNode = NewNode(key, value);
    (*baseNode)->pnode = lastNode;
    return *baseNode;
}

Node* FindNode(Node* node, char* key)
{
    if(node != NULL)
    {
        while(node != NULL)
        {
            int result = strcmp(key, node->key);
            if(result == 0)      return node;
            else if(result < 0)  node = node->lnode;
            else if(result > 0)  node = node->rnode;
        }
    }

    return node;
}

void FreeLeaf(Node* node)
{
    Node* parentNode = node->pnode;

    if(parentNode == NULL)
    {
        free(node->key);
        free(node->value);
        node->key   = NULL;
        node->value = NULL;

        free(node);
        node = NULL;
    }

    else
    {
        int cmpResult = strcmp(parentNode->key, node->key);

        free(node->key);
        free(node->value);
        node->key   = NULL;
        node->value = NULL;

        free(node);
        node = NULL;

        if(cmpResult < 0) parentNode->rnode = NULL;
        else              parentNode->lnode = NULL;
    }
}

void RemoveNode(Node* baseNode, char* key)
{
    if(baseNode == NULL)
        return;
    Node** base = &baseNode;
    Node*  node = FindNode(baseNode, key);

    if(node == NULL) return;

    if(node->lnode == NULL && node->rnode == NULL)
        FreeLeaf(node);
    else if(node->lnode != NULL && node->rnode == NULL)
    {
        if(node->pnode == NULL) base = &node->lnode;
        else
        {
            Node* pnode = node->pnode;
            Node* lnode = node->lnode;
            int cmpResult = strcmp(node->pnode->key, node->key);
            
            if(cmpResult < 0) pnode->rnode = lnode;
            else              pnode->lnode = lnode;
        }

        FreeLeaf(node);
        node = NULL;
    }
    else if(node->lnode == NULL && node->rnode != NULL) 
    {
        if(node->pnode == NULL) base = &node->rnode;
        else
        {
            Node* pnode = node->pnode;
            Node* rnode = node->rnode;
            int cmpResult = strcmp(node->pnode->key, node->key);
            if(cmpResult < 0) pnode->rnode = rnode;
            else              pnode->lnode = rnode;
        }

        FreeLeaf(node);
        node = NULL;
    }

    else if(node->lnode != NULL && node->rnode != NULL)
    {
        Node* replacementNode = node;
        if(replacementNode->lnode != NULL && CountSubnodes(node->lnode) >= CountSubnodes(node->rnode))
        {
            replacementNode = replacementNode->lnode;
            while(replacementNode->rnode != NULL)
                replacementNode = replacementNode->rnode;
        }

        else if(replacementNode->rnode != NULL)
        {
            replacementNode = replacementNode->rnode;
            while(replacementNode->lnode != NULL) 
                replacementNode = replacementNode->lnode;
        }

        else replacementNode = NULL;

        if(replacementNode != NULL)
        {
            if(node->pnode == NULL) *base = replacementNode;
            else
            {
                int cmpResult = strcmp(node->pnode->key, node->key);
                if(cmpResult < 0)   node->pnode->rnode = replacementNode;
                else                node->pnode->lnode = replacementNode;
            }

            int cmpResult = strcmp(replacementNode->pnode->key, replacementNode->key);
            if(cmpResult < 0) replacementNode->pnode->rnode = replacementNode->rnode;
            else              replacementNode->pnode->lnode = replacementNode->lnode;

            replacementNode->pnode = node->pnode;
            replacementNode->rnode = node->rnode;
            replacementNode->lnode = node->lnode;

            FreeLeaf(node);
        }
    }
}

void DeleteAllNodes(Node** baseNode)
{
    if(baseNode != NULL && *baseNode != NULL)
    {
        if((*baseNode)->lnode != NULL) DeleteAllNodes(&(*baseNode)->lnode);
        if((*baseNode)->rnode != NULL) DeleteAllNodes(&(*baseNode)->rnode);
        
        if((*baseNode)->lnode == NULL && (*baseNode)->rnode == NULL)
        {
            if((*baseNode)->pnode == NULL)
            {
                FreeLeaf(*baseNode);
                *baseNode = NULL;
                baseNode = NULL;
            }
            else 
                FreeLeaf(*baseNode);
            return;
        }
    }
}

void PrintTree(Node* baseNode)
{
    if(baseNode != NULL)
    {
        if(baseNode->lnode != NULL) PrintTree(baseNode->lnode);
        if(baseNode->rnode != NULL) PrintTree(baseNode->rnode);

        if(baseNode->key != NULL && baseNode->value != NULL) 
            printf("%s -> %s\n", baseNode->key, baseNode->value);
    }
}

int CountSubnodes(Node* node)
{
    if(node != NULL)
    {
        int count = 1;
        count += CountSubnodes(node->lnode);
        count += CountSubnodes(node->rnode);
        return count;
    }
    return 0;
}
 