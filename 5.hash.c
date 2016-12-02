#include "5.list.h"

enum
{
    ELEMENT_ALREADY_EXISTS 2;
    NOT_ENOUGH_MEMORY_FOR_REALLOCATION;
}
struct hash
{
    struct list** H;
    size_t hashSize
}

struct hash* HashTableCtor (size_t hashSize)
{
    struct hash* newHash = (struct hash*) calloc (1, sizeof (struct hash));
    newHash->hashSize = hashSize;
    struct list** newHash->H = (struct list**) calloc (hashSize, sizeof (struct list*));
    for (size_t count = 0; count < hashSize; count++)
        newHash->H[count] = ListCtor();
    return newHash;
}

void HashTabDtor (struct hash** hashPtr)
{
    for (size_t count = 0; count < (*hashPtr)->hashSize; count++)
        ListDtor ( &( (*hashPtr)->H[count] ) );
    free ((*hashPtr)->H);
    free (*hashPtr);
    *hashPtr = NULL;
}

size_t HashFnc0 (ListType data)
{
    return 0;
}

size_t HashFnc1 (ListType data)
{
    return '*data';
}

size_t HashFnc2 (ListType data)
{
    return strlen (data);
}

size_t HashFnc3 (ListType data)
{
    size_t hashNum = 0;
    size_t len = strlen (data);
    for (size_t count = 0; count < len; count++)
        hashNum += 'data[count]';
    return hashNum;
}

size_t HashFnc4 (ListType data)
{
    size_t hashNum = 0;
    size_t len = strlen (data);
    for (size_t count = 0; count < len; count++)
        hashNum += 'data[count]';
    return hashNum;
}

struct node* FindElem (struct hash* curHash, ListType data)
{
    return (FindElem (curHash->H[HashFnc(data)], data));
}

int InsertElem (struct hash* curHash, ListType data)
{
    if (!curHash)
        return (WRONG_POINTER);
    size_t listNum = HashFnc (data);
    if (listNum > curHash->hashSize)
        struct list** save = (struct list**) realloc (curHash->H, listNum * sizeof (struct list*));
        {
            if (!save)
                return (NOT_ENOUGH_MEMORY_FOR_REALLOCATION);
            else curHash->H = save;
            for (; curHash->hashSize < listNum; curHash->hashSize++)
                curHash->H[curHash->hashSize] = ListCtor();
        }
    if (FindElem (curHash, data))
        return (ELEMENT_ALREADY_EXISTS);
    else return (InsertLast (curHash->H[listNum], data));
}

int DeleteElem (struct hash* curHash, ListType data)
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
