#include "HashTable.hpp"

int GenerateHash(char* key)
{
    int hash = 5381;
    while (*key)
        hash = ((hash << 5) + hash) + *(key++);
    return hash;
}

HashTable* NewHashTable(int totalBuckets)
{
    if(totalBuckets > 0)
    {
        HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));

        if(newTable != NULL)
        {
            newTable->usedBucketCount = 0;
            newTable->totalBucketCount = totalBuckets;
            newTable->nodeTable = (Node**)malloc(totalBuckets * sizeof(Node));
            
            if(newTable->nodeTable != NULL)
            {
                memset(newTable->nodeTable, 0, sizeof(Node) * totalBuckets);
                return newTable;
            }

            free(newTable);
            newTable = NULL;
        }
    }
    return NULL;
}

void DestroyNodeChain(Node** baseNode)
{
    DeleteAllNodes(baseNode);
}

void DestroyHashTable(HashTable** hashTable)
{
    int i;

    if(hashTable == NULL || *hashTable == NULL)
        return;
    
    for(i = 0; i < (*hashTable)->totalBucketCount; i++)
        if((*hashTable)->nodeTable[i] != NULL)
            DestroyNodeChain(&(*hashTable)->nodeTable[i]);

    free((*hashTable)->nodeTable);
    (*hashTable)->nodeTable = NULL;
    (*hashTable)->totalBucketCount = 0;
    (*hashTable)->usedBucketCount = 0;

    free(*hashTable);
    *hashTable = NULL;
}

Node* AddValue(HashTable* hashTable, char* key, char* value)
{
    if(hashTable != NULL)
    {
        int hashOffset = GenerateHash((char*)key) % hashTable->totalBucketCount;
        if(hashTable->nodeTable[hashOffset] == NULL)
        {
            hashTable->nodeTable[hashOffset] = NewNode(key, value);
            return hashTable->nodeTable[hashOffset];
        }

        else
        {
            return AddNode(hashTable->nodeTable[hashOffset], key, value);
        }
    }
}

Node* FindValue(HashTable* hashTable, char* key)
{
    if(hashTable != NULL)
    {
        int hashOffset = GenerateHash((char*)key) % hashTable->totalBucketCount;
        if(hashTable->nodeTable[hashOffset] != NULL)
        {
            return FindNode(hashTable->nodeTable[hashOffset], key);
        }
    }
    return NULL;
}

void EditValue(HashTable* hashTable, char* key, char* value)
{
    if(hashTable != NULL && value != NULL)
    {
        Node* tmpNode = FindValue(hashTable, key);
        if(tmpNode != NULL)
        {
            tmpNode->value = (char*)realloc(tmpNode->value, strlen(value) + 1);
            strcpy(tmpNode->value, value);
        }
    }
}

void RemoveValue(HashTable* hashTable, char* key)
{
    if(hashTable != NULL)
    {
        int hashOffset = GenerateHash((char*)key) % hashTable->totalBucketCount;
        if(hashTable->nodeTable[hashOffset] != NULL)
        {
            RemoveNode(hashTable->nodeTable[hashOffset], key);
            hashTable->nodeTable[hashOffset] = NULL;
        }
    }
}

void PrintAllValues(HashTable* hashTable)
{
    int i;
    for(i = 0; i < hashTable->totalBucketCount; i++)
    {
        PrintTree(hashTable->nodeTable[i]);
    }
}

int main()
{

}