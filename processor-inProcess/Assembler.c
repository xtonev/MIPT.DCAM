#include <TXLib.h>
#include "Commands.h"

const int VERSION = 1;

enum Warnings
{
    LABELS_OVERFLOW = 300,
};

const int LABELSSIZE = 30;

size_t SizeOfFile (FILE* file);
int* ReadLabels (char* inBuffer, double* outBuffer, size_t* outBufNum);
int ReadCmd (char* inBuffer, size_t* inBufNum, double* outBuffer, size_t* outBufNum, int* labels);

int main()
{
    FILE* Input = fopen ("SqrEqu.txt", "r");
    size_t fileSize = SizeOfFile (Input);
    char* inBuffer = (char*) calloc (fileSize, sizeof (char) );
    fread (inBuffer, sizeof (char), fileSize, Input);
    fclose (Input);

    double* outBuffer = (double*) calloc (fileSize, sizeof (double) );

    size_t inBufNum = 0;
    size_t outBufNum = 0;

    int* labels = ReadLabels (inBuffer, outBuffer, &outBufNum);

    int readingStatus = ReadCmd (inBuffer, &inBufNum, outBuffer, &outBufNum, labels);
    while (readingStatus != END_CODE)
    {
        if (readingStatus == WRONG_CMD_CODE)
        {
            free (labels);
            free (inBuffer);
            free (outBuffer);
            assert (!"WRONG_CMD_CODE");
        }
        readingStatus = ReadCmd (inBuffer, &inBufNum, outBuffer, &outBufNum, labels);
    }

    FILE* Output = fopen ("Output.txt", "wb");
    fwrite (outBuffer, sizeof (double), outBufNum, Output);
    free (labels);
    free (inBuffer);
    free (outBuffer);
    fclose (Output);

    return 0;
}

size_t SizeOfFile (FILE* file)
{
    fseek (file, 0, SEEK_END);
    size_t fileSize = ftell (file) / sizeof (char);
    rewind (file);
    return fileSize;
}

int* ReadLabels (char* inBuffer, double* outBuffer, size_t* outBufNum)
{
    int inBufNum = 0;
    int* labels = (int*) calloc (LABELSSIZE, sizeof(int) );
    int labelNum = 0;

    while ( (inBuffer[inBufNum] != '.') || (inBuffer[inBufNum - 1] != '\n') )
    {
        while (isspace(inBuffer[inBufNum]))
            inBufNum++;

        if ( inBuffer[inBufNum] == '|' )
        {
            inBufNum++;
            while ( inBuffer[inBufNum] != '|' )
                inBufNum++;
            inBufNum++;
        }
        while ( isspace(inBuffer[inBufNum]) )
            inBufNum++;

        (*outBufNum)++;

        if ( (inBuffer[inBufNum] == ':') && (inBuffer[inBufNum - 1] == '\n') )
        {
            inBuffer[inBufNum] = ' ';
            if ( inBuffer[inBufNum + 1] != EOF )
            {
                inBufNum++;
                labelNum = atoi ( &inBuffer[inBufNum] );

                if (labelNum >= LABELSSIZE)
                {
                    free (labels);
                    assert (!"WRONG_LABEL");
                }
                if ( labels[labelNum] )
                {
                    free (labels);
                    assert (!"LABELS_OVERFLOW");
                }
                labels[labelNum] = *outBufNum;
                (*outBufNum)--;

                while ( !isspace(inBuffer[inBufNum]) )
                {
                    inBuffer[inBufNum] = ' ';
                    inBufNum++;
                }
            }
        }
        else if ( (inBuffer[inBufNum] != '.') || (inBuffer[inBufNum - 1] != '\n') )
        {
            while ( !isspace(inBuffer[inBufNum]) )
                inBufNum++;
        }
        else (*outBufNum)--;
    }

    outBuffer[0] = (double) VERSION;
    *outBufNum = 1;
    return labels;
}

int ReadCmd (char* inBuffer, size_t* inBufNum, double* outBuffer, size_t* outBufNum, int* labels)
{
    char command[30] = {0};
    int cmdPtr = 0;

    while ( isspace(inBuffer[*inBufNum]) )
        (*inBufNum)++;

    if ( inBuffer[*inBufNum] == '|' )
    {
        (*inBufNum)++;
        while ( inBuffer[*inBufNum] != '|' )
            (*inBufNum)++;
        (*inBufNum)++;
    }

    while ( isspace(inBuffer[*inBufNum]) )
        (*inBufNum)++;

    while ( !isspace(inBuffer[*inBufNum]) && (inBuffer[*inBufNum] != EOF) )
    {
        command[cmdPtr] = inBuffer[*inBufNum];
        cmdPtr++;
        (*inBufNum)++;
    }

    if ( !strcmp (command, "PUSH") )
    {
        outBuffer [*outBufNum] = PUSH_CODE;
        (*outBufNum)++;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[(*inBufNum) + 1] == 'x') && isspace(inBuffer[(*inBufNum) + 2]) )
        {
            if ( inBuffer[*inBufNum] == 'A' )
                outBuffer[*outBufNum] = AX_REG;
            else if ( inBuffer[*inBufNum] == 'B' )
                outBuffer[*outBufNum] = BX_REG;
            else if ( inBuffer[*inBufNum] == 'C' )
                outBuffer[*outBufNum] = CX_REG;
            else if ( inBuffer[*inBufNum] == 'D' )
                outBuffer[*outBufNum] = DX_REG;
            else if ( inBuffer[*inBufNum] == 'E' )
                outBuffer[*outBufNum] = EX_REG;
            (*outBufNum)++;
            (*inBufNum) += 2;
        }
        else if (inBuffer[*inBufNum] == 'S')
        {
            outBuffer[*outBufNum] = ST_CODE;
            (*outBufNum)++;
            (*inBufNum)++;
        }
        else if ( (inBuffer[*inBufNum] == 'R') && (inBuffer[(*inBufNum) + 1] == 'A') && (inBuffer[(*inBufNum) + 2] == 'M') )
        {
            outBuffer[*outBufNum] = RAM_CODE;
            (*inBufNum) += 3;
            (*outBufNum)++;

            while ( isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            outBuffer[*outBufNum] = atoi (&inBuffer[*inBufNum]);
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;
        }
        else return WRONG_CMD_CODE;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        outBuffer [*outBufNum] = atof (&inBuffer[*inBufNum]);
        (*outBufNum)++;

        while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

        return 0;
    }

    if ( !strcmp (command, "POP") )
    {
        outBuffer [*outBufNum] = POP_CODE;
        (*outBufNum)++;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[(*inBufNum) + 1] == 'x') && isspace(inBuffer[(*inBufNum) + 2]) )
        {
            if ( inBuffer[*inBufNum] == 'A' )
                outBuffer[*outBufNum] = AX_REG;
            else if ( inBuffer[*inBufNum] == 'B' )
                outBuffer[*outBufNum] = BX_REG;
            else if ( inBuffer[*inBufNum] == 'C' )
                outBuffer[*outBufNum] = CX_REG;
            else if ( inBuffer[*inBufNum] == 'D' )
                outBuffer[*outBufNum] = DX_REG;
            else if ( inBuffer[*inBufNum] == 'E' )
                outBuffer[*outBufNum] = EX_REG;
            (*outBufNum)++;
            (*inBufNum) += 2;
        }
        else if (inBuffer[*inBufNum] == 'S')
        {
            outBuffer[*outBufNum] = ST_CODE;
            (*outBufNum)++;
            (*inBufNum)++;
        }
        else if ( (inBuffer[*inBufNum] == 'R') && (inBuffer[(*inBufNum) + 1] == 'A') && (inBuffer[(*inBufNum) + 2] == 'M') )
        {
            outBuffer[*outBufNum] = RAM_CODE;
            (*inBufNum) += 3;
            (*outBufNum)++;

            while ( isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            outBuffer[*outBufNum] = atoi (&inBuffer[*inBufNum]);
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;
        }
        else return WRONG_CMD_CODE;

        return 0;
    }

    if ( !strcmp (command, "ADD") )
    {
        outBuffer [*outBufNum] = ADD_CODE;
        (*outBufNum)++;
        return 0;
    }

    if ( !strcmp (command, "SUB") )
    {
        outBuffer [*outBufNum] = SUB_CODE;
        (*outBufNum)++;
        return 0;
    }

    if ( !strcmp (command, "MUL") )
    {
        outBuffer [*outBufNum] = MUL_CODE;
        (*outBufNum)++;
        return 0;
    }

    if ( !strcmp (command, "DIV") )
    {
        outBuffer [*outBufNum] = DIV_CODE;
        (*outBufNum)++;
        return 0;
    }

    if ( !strcmp (command, "SQRT") )
    {
        outBuffer [*outBufNum] = SQRT_CODE;
        (*outBufNum)++;
        return 0;
    }

    if ( !strcmp (command, "MOV") )
    {
        outBuffer [*outBufNum] = MOV_CODE;
        (*outBufNum)++;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[(*inBufNum) + 1] == 'x') && isspace(inBuffer[(*inBufNum) + 2]) )
        {
            if ( inBuffer[*inBufNum] == 'A' )
                outBuffer[*outBufNum] = AX_REG;
            else if ( inBuffer[*inBufNum] == 'B' )
                outBuffer[*outBufNum] = BX_REG;
            else if ( inBuffer[*inBufNum] == 'C' )
                outBuffer[*outBufNum] = CX_REG;
            else if ( inBuffer[*inBufNum] == 'D' )
                outBuffer[*outBufNum] = DX_REG;
            else if ( inBuffer[*inBufNum] == 'E' )
                outBuffer[*outBufNum] = EX_REG;
            (*outBufNum)++;
            (*inBufNum) += 2;
        }
        else if (inBuffer[*inBufNum] == 'S')
        {
            outBuffer[*outBufNum] = ST_CODE;
            (*outBufNum)++;
            (*inBufNum)++;
        }
        else if ( (inBuffer[*inBufNum] == 'R') && (inBuffer[(*inBufNum) + 1] == 'A') && (inBuffer[(*inBufNum) + 2] == 'M') )
        {
            outBuffer[*outBufNum] = RAM_CODE;
            (*inBufNum) += 3;
            (*outBufNum)++;

            while ( isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            outBuffer[*outBufNum] = atoi (&inBuffer[*inBufNum]);
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;
        }
        else return WRONG_CMD_CODE;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[(*inBufNum) + 1] == 'x') && isspace(inBuffer[(*inBufNum) + 2]) )
        {
            if ( inBuffer[*inBufNum] == 'A' )
                outBuffer[*outBufNum] = AX_REG;
            else if ( inBuffer[*inBufNum] == 'B' )
                outBuffer[*outBufNum] = BX_REG;
            else if ( inBuffer[*inBufNum] == 'C' )
                outBuffer[*outBufNum] = CX_REG;
            else if ( inBuffer[*inBufNum] == 'D' )
                outBuffer[*outBufNum] = DX_REG;
            else if ( inBuffer[*inBufNum] == 'E' )
                outBuffer[*outBufNum] = EX_REG;
            (*outBufNum)++;
            (*inBufNum) += 2;
        }
        else if (inBuffer[*inBufNum] == 'S')
        {
            outBuffer[*outBufNum] = ST_CODE;
            (*outBufNum)++;
            (*inBufNum)++;
        }
        else if ( (inBuffer[*inBufNum] == 'R') && (inBuffer[(*inBufNum) + 1] == 'A') && (inBuffer[(*inBufNum) + 2] == 'M') )
        {
            outBuffer[*outBufNum] = RAM_CODE;
            (*inBufNum) += 3;
            (*outBufNum)++;

            while ( isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            outBuffer[*outBufNum] = atoi (&inBuffer[*inBufNum]);
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;
        }
        else return WRONG_CMD_CODE;

        return 0;
    }

    if ( !strcmp (command, "COPY") )
    {
        outBuffer [*outBufNum] = COPY_CODE;
        (*outBufNum)++;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[(*inBufNum) + 1] == 'x') && isspace(inBuffer[(*inBufNum) + 2]) )
        {
            if ( inBuffer[*inBufNum] == 'A' )
                outBuffer[*outBufNum] = AX_REG;
            else if ( inBuffer[*inBufNum] == 'B' )
                outBuffer[*outBufNum] = BX_REG;
            else if ( inBuffer[*inBufNum] == 'C' )
                outBuffer[*outBufNum] = CX_REG;
            else if ( inBuffer[*inBufNum] == 'D' )
                outBuffer[*outBufNum] = DX_REG;
            else if ( inBuffer[*inBufNum] == 'E' )
                outBuffer[*outBufNum] = EX_REG;
            (*outBufNum)++;
            (*inBufNum) += 2;
        }
        else if (inBuffer[*inBufNum] == 'S')
        {
            outBuffer[*outBufNum] = ST_CODE;
            (*outBufNum)++;
            (*inBufNum)++;
        }
        else if ( (inBuffer[*inBufNum] == 'R') && (inBuffer[(*inBufNum) + 1] == 'A') && (inBuffer[(*inBufNum) + 2] == 'M') )
        {
            outBuffer[*outBufNum] = RAM_CODE;
            (*inBufNum) += 3;
            (*outBufNum)++;

            while ( isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            outBuffer[*outBufNum] = atoi (&inBuffer[*inBufNum]);
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;
        }
        else return WRONG_CMD_CODE;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[(*inBufNum) + 1] == 'x') && isspace(inBuffer[(*inBufNum) + 2]) )
        {
            if ( inBuffer[*inBufNum] == 'A' )
                outBuffer[*outBufNum] = AX_REG;
            else if ( inBuffer[*inBufNum] == 'B' )
                outBuffer[*outBufNum] = BX_REG;
            else if ( inBuffer[*inBufNum] == 'C' )
                outBuffer[*outBufNum] = CX_REG;
            else if ( inBuffer[*inBufNum] == 'D' )
                outBuffer[*outBufNum] = DX_REG;
            else if ( inBuffer[*inBufNum] == 'E' )
                outBuffer[*outBufNum] = EX_REG;
            (*outBufNum)++;
            (*inBufNum) += 2;
        }
        else if (inBuffer[*inBufNum] == 'S')
        {
            outBuffer[*outBufNum] = ST_CODE;
            (*outBufNum)++;
            (*inBufNum)++;
        }
        else if ( (inBuffer[*inBufNum] == 'R') && (inBuffer[(*inBufNum) + 1] == 'A') && (inBuffer[(*inBufNum) + 2] == 'M') )
        {
            outBuffer[*outBufNum] = RAM_CODE;
            (*inBufNum) += 3;
            (*outBufNum)++;

            while ( isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            outBuffer[*outBufNum] = atoi (&inBuffer[*inBufNum]);
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;
        }
        else return WRONG_CMD_CODE;

        return 0;
    }

    if ( !strcmp (command, "DBL") )
    {
        outBuffer [*outBufNum] = DBL_CODE;
        (*outBufNum)++;
        return 0;
    }

    if ( !strcmp (command, "PROGRAMM_END") )
    {
        outBuffer [*outBufNum] = PROGRAMM_END_CODE;
        (*outBufNum)++;
        return 0;
    }

    if ( !strcmp (command, ".") )
    {
        (*outBufNum)++;
        return END_CODE;
    }

    if ( !strcmp (command, "JUMP") )
    {
        outBuffer[*outBufNum] = JUMP_CODE;
        (*outBufNum)++;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[*inBufNum] == ':') && ( isdigit (inBuffer[(*inBufNum) + 1]) ) )
        {
            (*inBufNum)++;
            outBuffer[*outBufNum] = labels[atoi (&inBuffer[*inBufNum])];
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            return 0;
        }
        else return WRONG_CMD_CODE;
    }

    if ( !strcmp (command, "JE") )
    {
        outBuffer[*outBufNum] = JE_CODE;
        (*outBufNum)++;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[*inBufNum] == ':') && ( isdigit (inBuffer[(*inBufNum) + 1]) ) )
        {
            (*inBufNum)++;
            outBuffer[*outBufNum] = labels[atoi (&inBuffer[*inBufNum])];
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            return 0;
        }
        else return WRONG_CMD_CODE;
    }

    if ( !strcmp (command, "JNE") )
    {
        outBuffer[*outBufNum] = JNE_CODE;
        (*outBufNum)++;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[*inBufNum] == ':') && ( isdigit (inBuffer[(*inBufNum) + 1]) ) )
        {
            (*inBufNum)++;
            outBuffer[*outBufNum] = labels[atoi (&inBuffer[*inBufNum])];
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            return 0;
        }
        else return WRONG_CMD_CODE;
    }

    if ( !strcmp (command, "JB") )
    {
        outBuffer[*outBufNum] = JB_CODE;
        (*outBufNum)++;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[*inBufNum] == ':') && ( isdigit (inBuffer[(*inBufNum) + 1]) ) )
        {
            (*inBufNum)++;
            outBuffer[*outBufNum] = labels[atoi (&inBuffer[*inBufNum])];
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            return 0;
        }
        else return WRONG_CMD_CODE;
    }

    if ( !strcmp (command, "JG") )
    {
        outBuffer[*outBufNum] = JG_CODE;
        (*outBufNum)++;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[*inBufNum] == ':') && ( isdigit (inBuffer[(*inBufNum) + 1]) ) )
        {
            (*inBufNum)++;
            outBuffer[*outBufNum] = labels[atoi (&inBuffer[*inBufNum])];
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            return 0;
        }
        else return WRONG_CMD_CODE;
    }

    if ( !strcmp (command, "PRINT") )
    {
        outBuffer[*outBufNum] = PRINT_CODE;
        (*outBufNum)++;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[(*inBufNum) + 1] == 'x') && isspace(inBuffer[(*inBufNum) + 2]) )
        {
            if ( inBuffer[*inBufNum] == 'A' )
                outBuffer[*outBufNum] = AX_REG;
            else if ( inBuffer[*inBufNum] == 'B' )
                outBuffer[*outBufNum] = BX_REG;
            else if ( inBuffer[*inBufNum] == 'C' )
                outBuffer[*outBufNum] = CX_REG;
            else if ( inBuffer[*inBufNum] == 'D' )
                outBuffer[*outBufNum] = DX_REG;
            else if ( inBuffer[*inBufNum] == 'E' )
                outBuffer[*outBufNum] = EX_REG;
            (*outBufNum)++;
            (*inBufNum) += 2;
        }
        else if (inBuffer[*inBufNum] == 'S')
        {
            outBuffer[*outBufNum] = ST_CODE;
            (*outBufNum)++;
            (*inBufNum)++;
        }
        else if ( (inBuffer[*inBufNum] == 'R') && (inBuffer[(*inBufNum) + 1] == 'A') && (inBuffer[(*inBufNum) + 2] == 'M') )
        {
            outBuffer[*outBufNum] = RAM_CODE;
            (*inBufNum) += 3;
            (*outBufNum)++;

            while ( isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            outBuffer[*outBufNum] = atoi (&inBuffer[*inBufNum]);
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;
        }
        else return WRONG_CMD_CODE;

        return 0;
    }

    if ( !strcmp (command, "SCAN") )
    {
        outBuffer[*outBufNum] = SCAN_CODE;
        (*outBufNum)++;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[(*inBufNum) + 1] == 'x') && isspace(inBuffer[(*inBufNum) + 2]) )
        {
            if ( inBuffer[*inBufNum] == 'A' )
                outBuffer[*outBufNum] = AX_REG;
            else if ( inBuffer[*inBufNum] == 'B' )
                outBuffer[*outBufNum] = BX_REG;
            else if ( inBuffer[*inBufNum] == 'C' )
                outBuffer[*outBufNum] = CX_REG;
            else if ( inBuffer[*inBufNum] == 'D' )
                outBuffer[*outBufNum] = DX_REG;
            else if ( inBuffer[*inBufNum] == 'E' )
                outBuffer[*outBufNum] = EX_REG;
            (*outBufNum)++;
            (*inBufNum) += 2;
        }
        else if (inBuffer[*inBufNum] == 'S')
        {
            outBuffer[*outBufNum] = ST_CODE;
            (*outBufNum)++;
            (*inBufNum)++;
        }
        else if ( (inBuffer[*inBufNum] == 'R') && (inBuffer[(*inBufNum) + 1] == 'A') && (inBuffer[(*inBufNum) + 2] == 'M') )
        {
            outBuffer[*outBufNum] = RAM_CODE;
            (*inBufNum) += 3;
            (*outBufNum)++;

            while ( isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            outBuffer[*outBufNum] = atoi (&inBuffer[*inBufNum]);
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;
        }
        else return WRONG_CMD_CODE;

        return 0;
    }

    if ( !strcmp (command, "WRITE") )
    {
        outBuffer[*outBufNum] = WRITE_CODE;
        (*outBufNum)++;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[(*inBufNum) + 1] == 'x') && isspace(inBuffer[(*inBufNum) + 2]) )
        {
            if ( inBuffer[*inBufNum] == 'A' )
                outBuffer[*outBufNum] = AX_REG;
            else if ( inBuffer[*inBufNum] == 'B' )
                outBuffer[*outBufNum] = BX_REG;
            else if ( inBuffer[*inBufNum] == 'C' )
                outBuffer[*outBufNum] = CX_REG;
            else if ( inBuffer[*inBufNum] == 'D' )
                outBuffer[*outBufNum] = DX_REG;
            else if ( inBuffer[*inBufNum] == 'E' )
                outBuffer[*outBufNum] = EX_REG;
            (*outBufNum)++;
            (*inBufNum) += 2;
        }
        else if (inBuffer[*inBufNum] == 'S')
        {
            outBuffer[*outBufNum] = ST_CODE;
            (*outBufNum)++;
            (*inBufNum)++;
        }
        else if ( (inBuffer[*inBufNum] == 'R') && (inBuffer[(*inBufNum) + 1] == 'A') && (inBuffer[(*inBufNum) + 2] == 'M') )
        {
            outBuffer[*outBufNum] = RAM_CODE;
            (*inBufNum) += 3;
            (*outBufNum)++;

            while ( isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            outBuffer[*outBufNum] = atoi (&inBuffer[*inBufNum]);
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;
        }
        else return WRONG_CMD_CODE;

        return 0;
    }

    if ( !strcmp (command, "CALL") )
    {
        outBuffer[*outBufNum] = CALL_CODE;
        (*outBufNum)++;

        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;

        if ( (inBuffer[*inBufNum] == ':') && ( isdigit (inBuffer[(*inBufNum) + 1]) ) )
        {
            (*inBufNum)++;
            outBuffer[*outBufNum] = labels[atoi (&inBuffer[*inBufNum])];
            (*outBufNum)++;

            while ( !isspace(inBuffer[*inBufNum]) )
                (*inBufNum)++;

            return 0;
        }
        else return WRONG_CMD_CODE;
    }

    if ( !strcmp (command, "BACK") )
    {
        outBuffer[*outBufNum] = BACK_CODE;
        (*outBufNum)++;
        return 0;
    }

    else return WRONG_CMD_CODE;
}
