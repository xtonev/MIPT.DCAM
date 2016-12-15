#include "Stack.h"
#include "prStack.h"
#include "Commands.h"

const int Version = 1;
const double EPS = 1e-6;
const size_t RAMSIZE = 30;

enum Warnings
{
    WRONG_VERSION = 210
};

struct CPU
{
    struct stack *Stack;
    struct prstack *PrStack;
    double *FlashPM;
    size_t PC;
    double Ax;
    double Bx;
    double Cx;
    double Dx;
    double Ex;
    double *RAM;
};
typedef struct CPU CPU;

CPU* CPUCtor ();
void CPUDtor (CPU** curCPU);
size_t SizeOfFile (FILE* file);
int CmdExecution (FILE* input, CPU* PtrCPU, FILE* result);

int main()
{
    CPU* curCPU = CPUCtor();
    FILE* Input = fopen ("Output.txt", "rb");
    FILE* Result = fopen ("Result.txt", "w");
    CmdExecution (Input, curCPU, Result);
    fclose (Input);
    fclose (Result);
    CPUDtor (&curCPU);
    return 0;
}

CPU* CPUCtor ()
{
    CPU* newCPU = (CPU*) calloc (1, sizeof(CPU));
    newCPU->Stack = stackCtor (50);
    newCPU->PrStack = prStackCtor (10);
    newCPU->FlashPM = (double*) calloc (1, sizeof(double));
    assert (newCPU->FlashPM);
    newCPU->RAM = (double*) calloc (RAMSIZE, sizeof(double));
    assert (newCPU->RAM);
    newCPU->PC = 0;
    newCPU->Ax = 0;
    newCPU->Bx = 0;
    newCPU->Cx = 0;
    newCPU->Dx = 0;
    newCPU->Ex = 0;
    return newCPU;
}

void CPUDtor (CPU** curCPU)
{
    assert (*curCPU);
    stackDtor ( &((*curCPU)->Stack) );
    prStackDtor ( &((*curCPU)->PrStack) );
    free ((*curCPU)->FlashPM);
    free ((*curCPU)->RAM);
    free (*curCPU);
    *curCPU = NULL;
}

size_t SizeOfFile (FILE* file)
{
    fseek (file, 0, SEEK_END);
    size_t fileSize = ftell (file) / sizeof (double);
    rewind (file);
    return fileSize;
}

int CmdExecution (FILE* Input, CPU* curCPU, FILE* Result)
{
    size_t inputSize = SizeOfFile (Input);
    double* save = (double*) realloc (curCPU->FlashPM, (inputSize + 1) * sizeof (double) );
    if (save != NULL)
        curCPU->FlashPM = save;
    else assert (!"CANNOT_REALLOCATE_MEMORY");
    fread (curCPU->FlashPM, sizeof (double), inputSize + 1, Input);

    if ( (int)*(curCPU->FlashPM) != Version)
        return WRONG_VERSION;
    curCPU->PC = 1;

    double par1 = 0, par2 = 0;
    while ( (int)curCPU->FlashPM[curCPU->PC] != PROGRAMM_END_CODE)
    {
        par1 = 0, par2 = 0;
        switch ( (int) curCPU->FlashPM[curCPU->PC] )
        {
            case PUSH_CODE:
            {
                (curCPU->PC)++;
                switch ( (int) curCPU->FlashPM[curCPU->PC] )
                {
                    case AX_REG:
                    {
                        (curCPU->PC)++;
                        curCPU->Ax = curCPU->FlashPM[curCPU->PC];
                        break;
                    }
                    case BX_REG:
                    {
                        (curCPU->PC)++;
                        curCPU->Bx = curCPU->FlashPM[curCPU->PC];
                        break;
                    }
                    case CX_REG:
                    {
                        (curCPU->PC)++;
                        curCPU->Cx = curCPU->FlashPM[curCPU->PC];
                        break;
                    }
                    case DX_REG:
                    {
                        (curCPU->PC)++;
                        curCPU->Dx = curCPU->FlashPM[curCPU->PC];
                        break;
                    }
                    case EX_REG:
                    {
                        (curCPU->PC)++;
                        curCPU->Ex = curCPU->FlashPM[curCPU->PC];
                        break;
                    }
                    case ST_CODE:
                    {
                        (curCPU->PC)++;
                        push (curCPU->Stack, curCPU->FlashPM[curCPU->PC]);
                        break;
                    }
                    case RAM_CODE:
                    {
                        (curCPU->PC)++;
                        if ( (size_t) curCPU->FlashPM[curCPU->PC] < RAMSIZE )
                            curCPU->RAM[(size_t)curCPU->FlashPM[curCPU->PC]] = curCPU->FlashPM[(curCPU->PC) + 1];
                        else return WRONG_CMD_CODE;
                        (curCPU->PC)++;
                        break;
                    }
                    default:
                    {
                        return WRONG_CMD_CODE;
                        break;
                    }
                }
                (curCPU->PC)++;
                break;
            }
            case POP_CODE:
            {
                (curCPU->PC)++;
                switch ( (int) curCPU->FlashPM[curCPU->PC] )
                {
                    case AX_REG:
                    {
                        curCPU->Ax = 0;
                        break;
                    }
                    case BX_REG:
                    {
                        curCPU->Bx = 0;
                        break;
                    }
                    case CX_REG:
                    {
                        curCPU->Cx = 0;
                        break;
                    }
                    case DX_REG:
                    {
                        curCPU->Dx = 0;
                        break;
                    }
                    case EX_REG:
                    {
                        curCPU->Ex = 0;
                        break;
                    }
                    case ST_CODE:
                    {
                        pop (curCPU->Stack);
                        break;
                    }
                    case RAM_CODE:
                    {
                        (curCPU->PC)++;
                        if ( (size_t) curCPU->FlashPM[curCPU->PC] < RAMSIZE )
                            curCPU->RAM[(size_t)curCPU->FlashPM[curCPU->PC]] = 0;
                        else return WRONG_CMD_CODE;
                        break;
                    }
                    default:
                    {
                        return WRONG_CMD_CODE;
                        break;
                    }
                }
                (curCPU->PC)++;
                break;
            }
            case ADD_CODE:
            {
                par1 = pop (curCPU->Stack);
                par2 = pop (curCPU->Stack);
                push (curCPU->Stack, par1 + par2);
                (curCPU->PC)++;
                break;
            }
            case SUB_CODE:
            {
                par1 = pop (curCPU->Stack);
                par2 = pop (curCPU->Stack);
                push (curCPU->Stack, par2 - par1);
                (curCPU->PC)++;
                break;
            }
            case MUL_CODE:
            {
                par1 = pop (curCPU->Stack);
                par2 = pop (curCPU->Stack);
                push (curCPU->Stack, par1 * par2);
                (curCPU->PC)++;
                break;
            }
            case DIV_CODE:
            {
                par1 = pop (curCPU->Stack);
                par2 = pop (curCPU->Stack);
                push (curCPU->Stack, par2 / par1);
                (curCPU->PC)++;
                break;
            }
            case SQRT_CODE:
            {
                par1 = pop (curCPU->Stack);
                push (curCPU->Stack, sqrt(par1) );
                (curCPU->PC)++;
                break;
            }
            case MOV_CODE:
            {
                (curCPU->PC)++;
                switch ( (int) curCPU->FlashPM[curCPU->PC] )
                {
                    case AX_REG:
                    {
                        par1 = curCPU->Ax;
                        curCPU->Ax = 0;
                        break;
                    }
                    case BX_REG:
                    {
                        par1 = curCPU->Bx;
                        curCPU->Bx = 0;
                        break;
                    }
                    case CX_REG:
                    {
                        par1 = curCPU->Cx;
                        curCPU->Cx = 0;
                        break;
                    }
                    case DX_REG:
                    {
                        par1 = curCPU->Dx;
                        curCPU->Dx = 0;
                        break;
                    }
                    case EX_REG:
                    {
                        par1 = curCPU->Ex;
                        curCPU->Ex = 0;
                        break;
                    }
                    case ST_CODE:
                    {
                        par1 = pop (curCPU->Stack);
                        break;
                    }
                    case RAM_CODE:
                    {
                        (curCPU->PC)++;
                        if ( (size_t) curCPU->FlashPM[curCPU->PC] < RAMSIZE )
                        {
                            par1 = curCPU->RAM[(size_t)curCPU->FlashPM[curCPU->PC]];
                            curCPU->RAM[(size_t)curCPU->FlashPM[curCPU->PC]] = 0;
                        }
                        else return WRONG_CMD_CODE;
                        break;
                    }
                    default:
                    {
                        return WRONG_CMD_CODE;
                        break;
                    }
                }

                (curCPU->PC)++;
                switch ( (int) curCPU->FlashPM[curCPU->PC] )
                {
                    case AX_REG:
                    {
                        curCPU->Ax = par1;
                        break;
                    }
                    case BX_REG:
                    {
                        curCPU->Bx = par1;
                        break;
                    }
                    case CX_REG:
                    {
                        curCPU->Cx = par1;
                        break;
                    }
                    case DX_REG:
                    {
                        curCPU->Dx = par1;
                        break;
                    }
                    case EX_REG:
                    {
                        curCPU->Ex = par1;
                        break;
                    }
                    case ST_CODE:
                    {
                        push (curCPU->Stack, par1);
                        break;
                    }
                    case RAM_CODE:
                    {
                        (curCPU->PC)++;
                        if ( (size_t) curCPU->FlashPM[curCPU->PC] < RAMSIZE )
                        {
                            curCPU->RAM[(size_t)curCPU->FlashPM[curCPU->PC]] = par1;
                        }
                        else return WRONG_CMD_CODE;
                        break;
                    }
                    default:
                    {
                        return WRONG_CMD_CODE;
                        break;
                    }
                }

                (curCPU->PC)++;
                break;
            }
            case COPY_CODE:
            {
                (curCPU->PC)++;
                switch ( (int) curCPU->FlashPM[curCPU->PC] )
                {
                    case AX_REG:
                    {
                        par1 = curCPU->Ax;
                        break;
                    }
                    case BX_REG:
                    {
                        par1 = curCPU->Bx;
                        break;
                    }
                    case CX_REG:
                    {
                        par1 = curCPU->Cx;
                        break;
                    }
                    case DX_REG:
                    {
                        par1 = curCPU->Dx;
                        break;
                    }
                    case EX_REG:
                    {
                        par1 = curCPU->Ex;
                        break;
                    }
                    case ST_CODE:
                    {
                        par1 = peek (curCPU->Stack);
                        break;
                    }
                    case RAM_CODE:
                    {
                        (curCPU->PC)++;
                        if ( (size_t) curCPU->FlashPM[curCPU->PC] < RAMSIZE )
                            par1 = curCPU->RAM[(size_t)curCPU->FlashPM[curCPU->PC]];
                        else return WRONG_CMD_CODE;
                        break;
                    }
                    default:
                    {
                        return WRONG_CMD_CODE;
                        break;
                    }
                }

                (curCPU->PC)++;
                switch ( (int) curCPU->FlashPM[curCPU->PC] )
                {
                    case AX_REG:
                    {
                        curCPU->Ax = par1;
                        break;
                    }
                    case BX_REG:
                    {
                        curCPU->Bx = par1;
                        break;
                    }
                    case CX_REG:
                    {
                        curCPU->Cx = par1;
                        break;
                    }
                    case DX_REG:
                    {
                        curCPU->Dx = par1;
                        break;
                    }
                    case EX_REG:
                    {
                        curCPU->Ex = par1;
                        break;
                    }
                    case ST_CODE:
                    {
                        push (curCPU->Stack, par1);
                        break;
                    }
                    case RAM_CODE:
                    {
                        (curCPU->PC)++;
                        if ( (size_t) curCPU->FlashPM[curCPU->PC] < RAMSIZE )
                        {
                            curCPU->RAM[(size_t)curCPU->FlashPM[curCPU->PC]] = par1;
                        }
                        else return WRONG_CMD_CODE;
                        break;
                    }
                    default:
                    {
                        return WRONG_CMD_CODE;
                        break;
                    }
                }

                (curCPU->PC)++;
                break;
            }
            case DBL_CODE:
            {
                push (curCPU->Stack, peek (curCPU->Stack) );
                (curCPU->PC)++;
                break;
            }
            case JUMP_CODE:
            {
                curCPU->PC = (int) curCPU->FlashPM[curCPU->PC + 1];
                break;
            }
            case JE_CODE:
            {
                if ( fabs(curCPU->Stack->data[curCPU->Stack->sizeSt - 1] - curCPU->Stack->data[curCPU->Stack->sizeSt - 2]) < EPS )
                    curCPU->PC = (int) curCPU->FlashPM[curCPU->PC + 1];
                else curCPU->PC += 2;
                break;
            }
            case JNE_CODE:
            {
                if ( fabs(curCPU->Stack->data[(curCPU->Stack->sizeSt) - 1] - curCPU->Stack->data[(curCPU->Stack->sizeSt) - 2]) >= EPS )
                    curCPU->PC = (int) curCPU->FlashPM[(curCPU->PC) + 1];
                else curCPU->PC += 2;
                break;
            }
            case JB_CODE:
            {
                if (curCPU->Stack->data[curCPU->Stack->sizeSt - 2] < curCPU->Stack->data[curCPU->Stack->sizeSt - 1])
                    curCPU->PC = (int) curCPU->FlashPM[curCPU->PC + 1];
                else curCPU->PC += 2;
                break;
            }
            case JG_CODE:
            {
                if (curCPU->Stack->data[curCPU->Stack->sizeSt - 2] > curCPU->Stack->data[curCPU->Stack->sizeSt - 1])
                    curCPU->PC = (int) curCPU->FlashPM[curCPU->PC + 1];
                else curCPU->PC += 2;
                break;
            }
            case PRINT_CODE:
            {
                (curCPU->PC)++;
                switch ( (int) curCPU->FlashPM[curCPU->PC] )
                {
                    case AX_REG:
                    {
                        printf ("%lg ", curCPU->Ax);
                        break;
                    }
                    case BX_REG:
                    {
                        printf ("%lg ", curCPU->Bx);
                        break;
                    }
                    case CX_REG:
                    {
                        printf ("%lg ", curCPU->Cx);
                        break;
                    }
                    case DX_REG:
                    {
                        printf ("%lg ", curCPU->Dx);
                        break;
                    }
                    case EX_REG:
                    {
                        printf ("%lg ", curCPU->Ex);
                        break;
                    }
                    case ST_CODE:
                    {
                        printf ("%lg ", peek (curCPU->Stack) );
                        break;
                    }
                    case RAM_CODE:
                    {
                        (curCPU->PC)++;
                        if ( (size_t) curCPU->FlashPM[curCPU->PC] < RAMSIZE )
                            printf ("%lg ", curCPU->RAM[(size_t)curCPU->FlashPM[curCPU->PC]] );
                        else return WRONG_CMD_CODE;
                        break;
                    }
                    default:
                    {
                        return WRONG_CMD_CODE;
                        break;
                    }
                }

                (curCPU->PC)++;
                break;
            }
            case SCAN_CODE:
            {
                scanf ("%lg", &par1);
                (curCPU->PC)++;
                switch ( (int) curCPU->FlashPM[curCPU->PC] )
                {
                    case AX_REG:
                    {
                        curCPU->Ax = par1;
                        break;
                    }
                    case BX_REG:
                    {
                        curCPU->Bx = par1;
                        break;
                    }
                    case CX_REG:
                    {
                        curCPU->Cx = par1;
                        break;
                    }
                    case DX_REG:
                    {
                        curCPU->Dx = par1;
                        break;
                    }
                    case EX_REG:
                    {
                        curCPU->Ex = par1;
                        break;
                    }
                    case ST_CODE:
                    {
                        push (curCPU->Stack, par1);
                        break;
                    }
                    case RAM_CODE:
                    {
                        (curCPU->PC)++;
                        if ( (size_t) curCPU->FlashPM[curCPU->PC] < RAMSIZE )
                            curCPU->RAM[(size_t)curCPU->FlashPM[curCPU->PC]] = par1;
                        else return WRONG_CMD_CODE;
                        break;
                    }
                    default:
                    {
                        return WRONG_CMD_CODE;
                        break;
                    }
                }

                (curCPU->PC)++;
                break;
            }
            case WRITE_CODE:
            {
                (curCPU->PC)++;
                switch ( (int) curCPU->FlashPM[curCPU->PC] )
                {
                    case AX_REG:
                    {
                        fprintf (Result, "%lg ", curCPU->Ax);
                        break;
                    }
                    case BX_REG:
                    {
                        fprintf (Result, "%lg ", curCPU->Bx);
                        break;
                    }
                    case CX_REG:
                    {
                        fprintf (Result, "%lg ", curCPU->Cx);
                        break;
                    }
                    case DX_REG:
                    {
                        fprintf (Result, "%lg ", curCPU->Dx);
                        break;
                    }
                    case EX_REG:
                    {
                        fprintf (Result, "%lg ", curCPU->Ex);
                        break;
                    }
                    case ST_CODE:
                    {
                        fprintf (Result, "%lg ", peek (curCPU->Stack) );
                        break;
                    }
                    case RAM_CODE:
                    {
                        (curCPU->PC)++;
                        if ( (size_t) curCPU->FlashPM[curCPU->PC] < RAMSIZE )
                            fprintf (Result, "%lg ", curCPU->RAM[(size_t)curCPU->FlashPM[curCPU->PC]] );
                        else return WRONG_CMD_CODE;
                        break;
                    }
                    default:
                    {
                        return WRONG_CMD_CODE;
                        break;
                    }
                }
                (curCPU->PC)++;
                break;
            }
            case CALL_CODE:
            {
                prPush (curCPU->PrStack, curCPU->PC + 2);
                curCPU->PC = (size_t) curCPU->FlashPM[(curCPU->PC) + 1];
                break;
            }
            case BACK_CODE:
            {
                curCPU->PC = prPop (curCPU->PrStack);
                break;
            }
            default:
                return WRONG_CMD_CODE;
        }
    }

    return 0;
}
