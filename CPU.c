#include "stack.h"
#include "Assembler.h"

struct CPU
{
    struct stack *Stack;
    uint64_t *FlashPM; //Flash Program Memory
    uint64_t PC;    //Program Counter
    uint64_t R[32]; //Registers R0 - R31
    size_t PCMaxAdr;//Max command address
}

void CtorCPU (struct CPU *CPU);
void DtorCPU (struct CPU *CPU);
int CmdExecution (uint64_t *CPU->FlashPM);

int main()
{
    curCPU = CPUCtor();
    CmdExecution (
    CPUDtor (curCPU);
    return 0;
}

struct CPU* CPUCtor ()
{
    struct CPU* newCPU = (struct CPU*) calloc (1, sizeof (struct CPU) );
    newCPU->Stack = (struct stack*) calloc (1, sizeof(struct stack));
    StackCtor (newCPU->Stack, 50);
    FILE *cmdFile = fopen ("Output.txt", "rb");
    assert (cmdFile);
    newCPU->FlashPM = (uint64_t*) calloc (myFsize (cmdFile) + 1, sizeof (uint64_t) );
    fclose(cmdFile);
    assert (FlashPM);
    return newCPU;
}

void CPUDtor (struct CPU *curCPU)
{
    stackDtor (curCPU->Stack);
    free (curCPU->FlashPM);
    curCPU->FlashPM = 0xADDACDC;
    curCPU->PC = 0;
    curCPU->R[32] = {0};
    curCPU->PCMaxAdr = 0;
}

int CmdExecution (uint64_t *curCPU->FlashPM)
{


   /* while (!halted)
    fread
    case  */
//Exit -> terminated = 1
}

