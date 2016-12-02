#include <TXLib.h>

enum
{
    WRONG_POINTER = 1,
};

typedef int listType;

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

int main()
{
    struct list* newList = ListCtor();
    InsertFirst (newList, 5);
    ListDtor (&newList);
    return 0;
}

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
    while (tmp)
    {
         next = tmp->next;
         free (tmp);
         tmp = next;
    }
    free (*lstPtr);
    *lstPtr = NULL;
}

void ListDump (struct list* lst)
{

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
    tmp->next = lst->head;
    tmp->prev = NULL;
    if (lst->head)
        lst->head->prev = tmp;
    lst->head = tmp;
    if (!lst->tail)
        lst->tail = tmp;
    lst->listSize++;
    return 0;
}

int InsertLast (struct list* lst, listType data)
{
    if (!lst)
        return (WRONG_POINTER);
    struct node* tmp = (struct node*) calloc (1, sizeof (struct node));
    tmp->data = data;
    tmp->prev = lst->tail;
    tmp->next = NULL;
    if (lst->tail)
        lst->tail->next = tmp;
    lst->tail = tmp;
    if (!lst->head)
        lst->head = tmp;
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
    if (node->next)
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
    if (node->prev)
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
    return (data1 - data2);
}

struct node* FindElem (struct list* lst, listType data)
{
    if (!lst)
        return (NULL);
    struct node* tmp = lst->head;
    while (ListCmp(tmp->data, data) && tmp)
        tmp = tmp->next;
    return tmp;
}

struct node* FindNth (struct list* lst, size_t N)
{
    if (!lst)
        return (NULL);
    struct node* tmp = lst->head;
    for (size_t count = 1; (count < N) && tmp; count++)
        tmp = tmp->next;
    return tmp;
}

int DeleteEveryHuman (struct list* lst)
{
    if (!lst)
        return (WRONG_POINTER);
    struct node* tmp = lst->head;
    struct node* next = NULL;
    while (tmp)
    {
         next = tmp->next;
         free (tmp);
         tmp = next;
    }
    lst->head = NULL;
    lst->tail = NULL;
    lst->listSize = 0;
    return 0;
}
