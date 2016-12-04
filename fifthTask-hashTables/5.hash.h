#include "5.list.h"

enum Warnings
{
    ELEMENT_ALREADY_EXISTS = 6,
    NOT_ENOUGH_MEMORY_FOR_REALLOCATION,
};
struct hash
{
    struct list** H;
    size_t hashSize;
};

struct hash* HashTabCtor (size_t hashSize);
void HashTabDtor (struct hash** hashPtr);
int HashOK (struct hash* curHash);
size_t HashFnc1 (listType data);
size_t HashFnc2 (listType data);
size_t HashFnc3 (listType data);
size_t HashFnc4 (listType data);
size_t HashFnc5 (listType data);
size_t HashFnc6 (listType data);
struct node* FindElemHash (struct hash* curHash, listType data, size_t (*HashFnc)(listType data));
int InsertElem (struct hash* curHash, listType data, size_t (*HashFnc)(listType data));
int DeleteElem (struct hash* curHash, listType data, size_t (*HashFnc)(listType data));
int DeleteEveryRobot (struct hash* curHash);

struct hash* HashTabCtor (size_t hashSize)
{
    struct hash* newHash = (struct hash*) calloc (1, sizeof (struct hash));
    newHash->hashSize = hashSize;
    newHash->H = (struct list**) calloc (hashSize, sizeof (struct list*));
    for (size_t count = 0; count < hashSize; count++)
        newHash->H[count] = ListCtor();
    return newHash;
}

void HashTabDtor (struct hash** hashPtr)
{
    for (size_t count = 0; count < (*hashPtr)->hashSize; count++)
        ListDtor ( &((*hashPtr)->H[count]) );
    free ((*hashPtr)->H);
    free (*hashPtr);
    *hashPtr = NULL;
}

int HashOK (struct hash* curHash)
{
    int result = 0;
    size_t listCounter = 0;
    for (; listCounter < curHash->hashSize; listCounter++);
        result += ListOK (curHash->H[listCounter]);
    return result;
}

size_t HashFnc1 (listType data)
{
    return 0;
}

size_t HashFnc2 (listType data)
{
    return abs(*data);
}

size_t HashFnc3 (listType data)
{
    return strlen (data);
}

size_t HashFnc4 (listType data)
{
    size_t hashNum = 0;
    size_t len = strlen (data);
    for (size_t count = 0; count < len; count++)
        hashNum += data[count];
    return abs(hashNum);
}

size_t HashFnc5 (listType data)
{
    size_t hashNum = 0;
    for (; *data; data++)
        hashNum = ( (hashNum << 1) | ( hashNum >> (sizeof(hashNum) - 1) ) ) ^ *data;
    return abs(hashNum) % 10000;
}

size_t HashFnc6 (listType data)
{
    size_t hashNum = 0;
    for (; *data; data++)
    {
        hashNum += *data;
        hashNum -= (hashNum << 3) | (hashNum >> 9);
    }
    return abs(hashNum);
}

struct node* FindElemHash (struct hash* curHash, listType data, size_t (*HashFnc)(listType data))
{
    if (HashFnc(data) >= curHash->hashSize)
        return NULL;
    else return (FindElem (curHash->H[HashFnc(data)], data) );
}

int InsertElem (struct hash* curHash, listType data, size_t (*HashFnc)(listType data) )
{
    if (!curHash)
        return (WRONG_POINTER);
    size_t listNum = HashFnc (data);
    if (listNum >= curHash->hashSize)
    {
        struct list** save = (struct list**) realloc (curHash->H, (listNum + 1) * sizeof (struct list*));
        if (!save)
            return (NOT_ENOUGH_MEMORY_FOR_REALLOCATION);
        else curHash->H = save;
        for (; curHash->hashSize <= listNum; curHash->hashSize++)
            curHash->H[curHash->hashSize] = ListCtor();
    }
    if (FindElem (curHash->H[listNum], data))
        return (ELEMENT_ALREADY_EXISTS);
    else return (InsertLast (curHash->H[listNum], data));
}

int DeleteElem (struct hash* curHash, listType data, size_t (*HashFnc)(listType data))
{
    if (!curHash)
        return (WRONG_POINTER);
    struct list* lst = curHash->H[HashFnc(data)];
    struct node* node = FindElem (lst, data);
    while (node)
        DeleteByPtr (lst, node);
    return 0;
}

int DeleteEveryRobot (struct hash* curHash)
{
    if (!curHash)
        return (WRONG_POINTER);
    for (size_t count = 0; count < curHash->hashSize; count++)
        DeleteEveryHuman (curHash->H[count]);
    return 0;
}
