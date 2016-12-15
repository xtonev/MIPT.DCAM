#include <TXLib.h>

enum WARNING
{
    STACK_OVERFLOW = 100,
    WRONG_POINTERS,
    OUT_OF_MEMORY,
    JUST_ERROR,
    EMPTY_STACK
};

const int MULTIPLIER = 2;

typedef int stType;
struct stack
{
    stType *data;
    size_t sizeSt;
    size_t maxsize;
    int checkSum;
} ;

int checkSum (struct stack *curSt);
struct stack* stackCtor (size_t StSize);
void stackDtor (struct stack **curSt);
int stackOK (struct stack *curSt);
void stAssert (struct stack *curSt);
void dumpStack (struct stack *curSt);
void reall (struct stack *curSt);
void push (struct stack *curSt, stType elem);
stType pop (struct stack *curSt);
stType peek (struct stack *curSt);
size_t length (struct stack *curSt);

int checkSum (struct stack *curSt)
{
    int Sum = 0;
    for (size_t i = 0; i < curSt->sizeSt; i++)
    {
        Sum ^= curSt->data[i] + i;
    }
    Sum ^= curSt->sizeSt;
    Sum ^= curSt->maxsize;
    Sum ^= (int) curSt->data;
    Sum ^= (int) &(curSt->sizeSt);
    Sum ^= (int) &(curSt->maxsize);

    return Sum;
}

struct stack* stackCtor (size_t StSize)
{
    struct stack* curSt = (struct stack*) calloc (1, sizeof (struct stack) );
    curSt->sizeSt = 0;
    curSt->maxsize = StSize;
    curSt->data = (stType*) calloc (curSt->maxsize, sizeof (stType) );
    if (!surSt->data)
        assert(!"Calloc can not allocate some memory for me :(");
    curSt->checkSum = checkSum(curSt);
    return curSt;
}

void stackDtor (struct stack **curSt)
{
    assert (*curSt);
    assert (curSt);
    free ((*curSt)->data);
    (*curSt)->data = NULL;
    (*curSt)->maxsize = 0xDEADBEAF;
    (*curSt)->sizeSt = 0x0BADF00D;
    (*curSt)->checkSum = 0xDEFBADCD;
    free (*curSt);
    *curSt = NULL;
}

void stAssert (struct stack *curSt)
{
    switch (stackOK(curSt))
    {
        case 0:
            break;
        case STACK_OVERFLOW:
            dumpStack (curSt);
            assert (!"STACK_OVERFLOW");
            break;
        case WRONG_POINTERS:
            dumpStack (curSt);
            assert (!"WRONG_POINTERS");
            break;
        case JUST_ERROR:
            dumpStack (curSt);
            assert (!"JUST_ERROR");
            break;
        default:
            assert (!"JUST_ERROR");
            break;
    }
}

int stackOK (struct stack *curSt)
{
    if ((!curSt) && (!curSt->data))
        return WRONG_POINTERS;
    if ( curSt->sizeSt > curSt->maxsize )
        return STACK_OVERFLOW;
    if ( curSt->checkSum - checkSum(curSt) )
        return JUST_ERROR;

    return 0;
}

void dumpStack (struct stack *curSt)
{
    FILE* writeFile = fopen ("dump.txt", "a");
    fprintf (writeFile, "Stack status: problems? %d\n", StackOK (curSt) );
    size_t count = 0;
    for (; count < curSt->sizeSt; count++)
        fprintf (writeFile, "Element number %d: %d, its pointer: %p\n", count, curSt->data[count], &(curSt->data[count]));
    fprintf (writeFile, "Struct pointer: %p\n", curSt);
    fprintf (writeFile, "Data pointer: %p\n", curSt->data);
    fprintf (writeFile, "Size of stack: %d\n", curSt->sizeSt);
    fprintf (writeFile, "Max size of stack: %d\n", curSt->maxsize);
}

void reall (struct stack *curSt)
{
    stAssert (curSt);
    curSt->maxsize *= MULTIPLIER;
    (stType*) save = (stType*) realloc (curSt->data, curSt->maxsize * sizeof (stType));
    if (!save)
        curSt->data = save;
    else assert (!"OUT_OF_MEMORY");
    stAssert (curSt);
}

void push (struct stack *curSt, stType elem)
{
    stAssert (curSt);
    if (curSt->sizeSt > (curSt->maxsize * 0.9) )
    {
        reall (curSt);
    }
    curSt->data[curSt->sizeSt++] = elem;
    curSt->checkSum = checkSum(curSt);
    stAssert (curSt);
}

stType pop (struct stack *curSt)
{
    stAssert (curSt);
    if (curSt->sizeSt)
    {
        curSt->sizeSt--;
        curSt->checkSum = checkSum(curSt);
        return (curSt->data[curSt->sizeSt]);
    }
    else assert (!"EMPTY_STACK");
    return EMPTY_STACK;
}

stType peek (struct stack *curSt)
{
    stAssert (curSt);
    if (curSt->sizeSt)
    {
        return (curSt->data[curSt->sizeSt]);
    }
    else assert (!"EMPTY_STACK");
    return EMPTY_STACK;
}

size_t length (struct stack *curSt)
{
    stAssert (curSt);
    return (curSt->sizeSt);
}
