#include <TXLib.h>

enum PRWARNING
{
    PR_STACK_OVERFLOW = 200,
    PR_WRONG_POINTERS,
    PR_OUT_OF_MEMORY,
    PR_JUST_ERROR,
    PR_EMPTY_STACK
};

const int PRMULTIPLIER = 2;

typedef int prStType;
struct prstack
{
    prStType *data;
    size_t sizeSt;
    size_t maxsize;
};

struct prstack* prStackCtor (size_t StSize);
void prStackDtor (struct prstack **curSt);
void prReall (struct prstack *curSt);
void prPush (struct prstack *curSt, prStType elem);
prStType prPop (struct prstack *curSt);

struct prstack* prStackCtor (size_t StSize)
{
    struct prstack* curSt = (struct prstack*) calloc (1, sizeof (struct prstack) );
    curSt->sizeSt = 0;
    curSt->maxsize = StSize;
    curSt->data = (prStType*) calloc (curSt->maxsize, sizeof (prStType) );
    if (!curSt->data)
        assert(!"Calloc can not allocate some memory for me :(");
    return curSt;
}

void prStackDtor (struct prstack **curSt)
{
    assert (*curSt);
    assert (curSt);
    free ((*curSt)->data);
    free (*curSt);
    *curSt = NULL;
}

void prReall (struct prstack *curSt)
{
    curSt->maxsize *= PRMULTIPLIER;
    prStType* save = (prStType*) realloc (curSt->data, curSt->maxsize * sizeof (prStType));
    if (!save)
        curSt->data = save;
    else assert(!"PR_OUT_OF_MEMORY");
}

void prPush (struct prstack *curSt, prStType elem)
{
    if (curSt->sizeSt > (curSt->maxsize * 0.9) )
        prReall (curSt);
    curSt->data[curSt->sizeSt++] = elem;
}

prStType prPop (struct prstack *curSt)
{
    if (curSt->sizeSt)
    {
        curSt->sizeSt--;
        return (curSt->data[curSt->sizeSt]);
    }
    else return PR_EMPTY_STACK;
}
