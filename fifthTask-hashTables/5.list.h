#include <TXLib.h>

enum
{
    WRONG_POINTER = 1,
    WRONG_SIZE,
    WRONG_INLIST_POINTERS
};

typedef char* listType;

struct list
{
    size_t listSize;
    struct node* head;
    struct node* tail;
};
struct node
{
    listType data;
    struct node* next;
    struct node* prev;
};
struct list* ListCtor();
void ListDtor (struct list** lst);
int ListOK (struct list* lst);
void ListDump (struct list* lst);
struct node* GetFirst (struct list* lst);
struct node* GetLast (struct list* lst);
struct node* GetNext (struct node* node);
struct node* GetPrev (struct node* node);
int InsertFirst (struct list* lst, listType data);
int InsertLast (struct list* lst, listType data);
int InsertNext (struct list* lst, struct node* node, listType data);
int InsertPrev (struct list* lst, struct node* node, listType data);
int DeleteByPtr (struct list* lst, struct node* node);
int ListCmp (listType data1, listType data2);
struct node* FindElem (struct list* lst, listType data);
struct node* FindNth (struct list* lst, size_t N);
int DeleteEveryHuman (struct list* lst);

struct list* ListCtor()
{
    struct list* lst = (struct list*) calloc (1, sizeof (struct list));
    lst->listSize = 0;
    lst->head = NULL;
    lst->tail = NULL;
    return (lst);
}

void ListDtor (struct list** lstPtr)
{
    struct node* tmp = (*lstPtr)->head;
    struct node* next = NULL;

    size_t count = 0;
    while (count < (*lstPtr)->listSize)
    {
         next = tmp->next;
         free (tmp);
         tmp = next;
         count++;
    }
    free (*lstPtr);
    *lstPtr = NULL;
}

int ListOK (struct list* lst)
{
    if ( (!lst->listSize) && ( (lst->head) || (lst->tail) ))
    return WRONG_SIZE;

    if ( (lst->head == NULL) ^ (lst->tail == NULL) )
    return WRONG_INLIST_POINTERS;

    if (lst->listSize)
    {
        struct node* tmp = (struct node*) calloc (1, sizeof (struct node) );
        tmp = lst->head;
        if ( (!tmp) || (tmp != tmp->next->prev) || (tmp->prev->next != tmp) )
                return WRONG_POINTER;
        tmp = tmp->next;

        size_t count = 1;
        for (; count < lst->listSize; count++)
        {
            if ( (!tmp) || (tmp != tmp->next->prev) || (tmp->prev->next != tmp) )
                return WRONG_POINTER;
            if (tmp == lst->head)
                return WRONG_SIZE;
            tmp = tmp->next;
        }
    }

    return 0;
}

void ListDump (struct list* lst)
{
    FILE* listDump = fopen ("5.dumpList.txt", "w");
    fprintf (listDump, "List pointer: %p\nList size: %u\nProblems? %d", lst, lst->listSize, ListOK (lst) );
    fclose (listDump);

    if (lst->listSize)
    {
        FILE* listGraph = fopen ("5.dumpList.gv", "w");
        fprintf (listGraph, "digraph List {\nrankdir=LR;\n");
        size_t count = 0;
        struct node* tmp = lst->head;
        for (; count < lst->listSize ; count++)
        {
            fprintf (listGraph, "%u [shape=record,label=\"<prev> %p| {data %p|<curr> %p} |<next> %p\"];\n", count, tmp->prev, tmp->data, tmp, tmp->next);
            tmp = tmp->next;
        }
        for (count = 0; count < lst->listSize - 1 ; count++)
            fprintf (listGraph, "%u:<next> -> %u:<curr>;\n %u:<prev> -> %u:<curr>;\n", count, count+1, count+1, count);
        if (count != 0)
            fprintf (listGraph, "%u:<next> -> %u:<curr>;\n %u:<prev> -> %u:<curr>;\n", count, 0, 0, count);
        fprintf (listGraph, "}");
        fclose (listGraph);
    }
}

struct node* GetFirst (struct list* lst)
{
    return (lst->head);
}

struct node* GetLast (struct list* lst)
{
    return (lst->tail);
}

struct node* GetNext (struct node* node)
{
    return (node->next);
}

struct node* GetPrev (struct node* node)
{
    return (node->prev);
}

int InsertFirst (struct list* lst, listType data)
{
    if (!lst)
        return (WRONG_POINTER);
    struct node* tmp = (struct node*) calloc (1, sizeof (struct node));
    tmp->data = data;
    if (lst->head)
    {
        lst->head->prev = tmp;
        tmp->next = lst->head;
        lst->tail->next = tmp;
        tmp->prev = lst->tail;
    }
    else
    {
        tmp->next = tmp;
        tmp->prev = tmp;
        lst->tail = tmp;
    }
    lst->head = tmp;
    lst->listSize++;
    return 0;
}

int InsertLast (struct list* lst, listType data)
{
    if (!lst)
        return (WRONG_POINTER);
    struct node* tmp = (struct node*) calloc (1, sizeof (struct node));
    tmp->data = data;
    if (lst->tail)
    {
        lst->tail->next = tmp;
        tmp->prev = lst->tail;
        lst->head->prev = tmp;
        tmp->next = lst->head;
    }
    else
    {
        tmp->next = tmp;
        tmp->prev = tmp;
        lst->head = tmp;
    }
    lst->tail = tmp;
    lst->listSize++;
    return 0;
}

int InsertNext (struct list* lst, struct node* node, listType data)
{
    if ( (lst) && (node) )
        return (WRONG_POINTER);
    struct node* tmp = (struct node*) calloc (1, sizeof (struct node));
    tmp->data = data;
    tmp->next = node->next;
    tmp->prev = node;
    node->next->prev = tmp;
    node->next = tmp;
    lst->listSize++;
    return 0;
}

int InsertPrev (struct list* lst, struct node* node, listType data)
{
    if ( (!lst)&&(!node) )
        return (WRONG_POINTER);
    struct node* tmp = (struct node*) calloc (1, sizeof (struct node));
    tmp->data = data;
    tmp->prev = node->prev;
    tmp->next = node;
    node->prev->next = tmp;
    node->prev = tmp;
    lst->listSize++;
    return 0;
}

int DeleteByPtr (struct list* lst, struct node* node)
{
    if ( (!lst) && (!node) )
        return (WRONG_POINTER);
    if (node->next)
        node->next->prev = node->prev;
    if (node->prev)
        node->prev->next = node->next;
    if (!node->next)
        lst->tail = node->prev;
    if (!node->prev)
        lst->head = node->next;
    free (node);
    lst->listSize--;
    return 0;
}

int ListCmp (listType data1, listType data2)
{
    return strcmp (data1, data2);
}

struct node* FindElem (struct list* lst, listType data)
{
    if ( (!lst) || (!lst->listSize) )
        return (NULL);
    struct node* tmp = lst->head;
    size_t count = 0;
    while ( (ListCmp(tmp->data, data)) && (tmp) && (count < lst->listSize) )
    {
        tmp = tmp->next;
        count++;
    }
    if (count == lst->listSize)
        return NULL;
    else return tmp;
}

struct node* FindNth (struct list* lst, size_t N)
{
    if ( (!lst) || ( N > lst->listSize) )
        return (NULL);
    struct node* tmp = lst->head;
    size_t count = 1;
    for (; (count < N) && tmp; count++)
        tmp = tmp->next;
    if (count == lst->listSize)
        return NULL;
    else return tmp;
}

int DeleteEveryHuman (struct list* lst)
{
    if (!lst)
        return (WRONG_POINTER);
    struct node* tmp = lst->head;
    struct node* next = NULL;
    size_t count = 0;
    while (count < lst->listSize)
    {
         next = tmp->next;
         free (tmp);
         tmp = next;
         count++;
    }
    lst->head = NULL;
    lst->tail = NULL;
    lst->listSize = 0;
    return 0;
}
