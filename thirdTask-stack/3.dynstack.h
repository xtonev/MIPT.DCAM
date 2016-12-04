#include <TXLib.h>

enum WARNING
{
    STACK_OVERFLOW = 1,
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

int checkSum (struct stack *st);
void stackCtor (struct stack *st, size_t StSize);
void stackDtor (struct stack **st);
int stackOK (struct stack *st);
void stAssert (struct stack *st);
void dumpStack (struct stack *st);
void reall (struct stack *st);
void push (struct stack *st, stType elem);
stType pop (struct stack *st);
stType peek (struct stack *st);
size_t length (struct stack *st);

int checkSum (struct stack *st)
{
    int Sum = 0;
    for (size_t i = 0; i < st->sizeSt; i++)
    {
        Sum ^= st->data[i] + i;
    }
    Sum ^= st->sizeSt;
    Sum ^= st->maxsize;
    Sum ^= (int) st->data;
    Sum ^= (int) &(st->sizeSt);
    Sum ^= (int) &(st->maxsize);

    return Sum;
}

void stackCtor (struct stack *st, size_t StSize)
{
    assert(st);
    st->sizeSt = 0;
    st->maxsize = StSize;
    st->data = (stType*) calloc (st->maxsize, sizeof(stType));
    if (!st->data) assert(!"Calloc can not allocate some memory for me :(");
    st->checkSum = checkSum(st);
}

void stackDtor (struct stack **st)
{
    assert (*st);
    free ((*st)->data);
    (*st)->data = NULL;
    (*st)->maxsize = 0xDEADBEAF;
    (*st)->sizeSt = 0x0BADF00D;
    (*st)->checkSum = 0xDEFBADCD;
    *st = NULL;
}

void stAssert (struct stack *st)
{
    switch (stackOK(st))
    {
        case 0:
            break;
        case STACK_OVERFLOW:
            dumpStack (st);
            assert (!"STACK_OVERFLOW");
            break;
        case WRONG_POINTERS:
            dumpStack (st);
            assert (!"WRONG_POINTERS");
            break;
        case JUST_ERROR:
            dumpStack (st);
            assert (!"JUST_ERROR");
            break;
        default:
            assert (!"JUST_ERROR");
            break;
    }
}

int stackOK (struct stack *st)
{
    if ((!st) && (!st->data))
        return WRONG_POINTERS;
    if (st->sizeSt > st->maxsize)
        return STACK_OVERFLOW;
    if ( st->checkSum - checkSum(st) )
        return JUST_ERROR;

    return 0;
}

void dumpStack (struct stack *st)
{
    FILE* writeFile = fopen ("dump.txt", "a");
    fprintf (writeFile, "Stack status: problems? %d\n", StackOK (st) );
    size_t count = 0;
    for (; count < st->sizeSt; count++)
        fprintf (writeFile, "Element number %d: %d, its pointer: %p\n", count, st->data[count], &(st->data[count]));
    fprintf (writeFile, "Struct pointer: %p\n", st);
    fprintf (writeFile, "Data pointer: %p\n", st->data);
    fprintf (writeFile, "Size of stack: %d\n", st->sizeSt);
    fprintf (writeFile, "Max size of stack: %d\n", st->maxsize);
}

void reall (struct stack *st)
{
    stAssert (st);
    st->maxsize *= MULTIPLIER;
    (stType*) save = (stType*) realloc (st->data, st->maxsize * sizeof (stType));
    if (!save)
        st->data = save;
    else assert (!"OUT_OF_MEMORY");
    stAssert (st);
}

void push (struct stack *st, stType elem)
{
    stAssert (st);
    if (st->sizeSt > (st->maxsize * 0.9) )
    {
        reall (st);
    }
    st->data[st->sizeSt++] = elem;
    st->checkSum = checkSum(st);
    stAssert (st);
}

stType pop (struct stack *st)
{
    stAssert (st);
    if (st->sizeSt)
    {
        st->sizeSt--;
        st->checkSum = checkSum(st);
        return (st->data[st->sizeSt]);
    }
    else assert (!"EMPTY_STACK");
    return EMPTY_STACK;
}

stType peek (struct stack *st)
{
    stAssert (st);
    if (st->sizeSt)
    {
        return (st->data[st->sizeSt]);
    }
    else assert (!"EMPTY_STACK");
    return EMPTY_STACK;
}

size_t length (struct stack *st)
{
    stAssert (st);
    return (st->sizeSt);
}