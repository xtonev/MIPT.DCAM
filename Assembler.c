#include <TXLib.h>

enum Warnings
{
    LABELS_OVERFLOW = 100,
};

const int WRONG_CMD_CODE = -10,
    PROGRAMM_END_CODE = 2,
    PUSH_CODE = 31,
    PUSHREG_CODE = 32,
    POP_CODE = 41,
    POPREG_CODE = 42,
    ADD_CODE = 5,
    SUB_CODE = 6,
    MUL_CODE = 7,
    DIV_CODE = 8,
    JE_CODE = 10,
    JNE_CODE = 11,
    JB_CODE = 12,
    JG_CODE = 13,
    AX_REG = 124,
    BX_REG = 224,
    CX_REG = 324,
    DX_REG = 424;

const int LABELSSIZE = 10;

size_t SizeOfFile (FILE* file);
char* FirstSpace (char* strPointer);
int* ReadLabels (char* inBuffer, double* outBuffer, size_t* outBufNum);
int ReadCmd (char* inBuffer, size_t* inBufNum, double* outBuffer, size_t* outBufNum, int* labels);

int main()
{
    FILE* Input = fopen ("Input.txt", "r");
    size_t fileSize = SizeOfFile (Input);
    char* inBuffer = (char*) calloc (fileSize, sizeof (char) );
    fread (inBuffer, sizeof (char), fileSize, Input);
    fclose (Input);

    double* outBuffer = (double*) calloc (fileSize, sizeof (double) );

    size_t inBufNum = 0;
    size_t outBufNum = 0;

    int* labels = ReadLabels (inBuffer, outBuffer, &outBufNum);

    int readingStatus = ReadCmd (inBuffer, &inBufNum, outBuffer, &outBufNum, labels);
    while (readingStatus != PROGRAMM_END_CODE)
    {
        readingStatus = ReadCmd (inBuffer, &inBufNum, outBuffer, &outBufNum, labels);
    }

    FILE* Output = fopen ("Output.txt", "bw");

    for (size_t dblCount = 0; dblCount < outBufNum; dblCount++)
    {
        fprintf (Output, "%lg ", outBuffer[dblCount] );
    }

    free (labels);
    free (inBuffer);
    free (outBuffer);
    fclose (Output);

    return 0;
}

size_t SizeOfFile (FILE* file)
{
    fseek (file, 0, SEEK_END);
    size_t fileSize = ftell (file);
    rewind (file);
    return fileSize;
}

char* FirstSpace (char* strPointer)
{
    char* tab = strchr (strPointer, '\t');
    char* newLine = strchr (strPointer, '\n');
    char* space = strchr (strPointer, ' ');
    if (!tab)
    {
        if (!newLine)
            return space;
        if (!space)
            return newLine;
        else if (space < newLine)
                return space;
                else return newLine;
    }
    if (!newLine)
    {
        if (!tab)
            return space;
        if (!space)
            return tab;
        else if (space < tab)
                return space;
                else return tab;
    }
    if (!space)
    {
        if (!newLine)
            return tab;
        if (!tab)
            return newLine;
        else if (tab < newLine)
                return tab;
            else return newLine;
    }
    if ( (space < newLine) && (space < newLine))
        return space;
    if ( (tab < space) && (newLine < tab))
        return newLine;
    else return tab;
}

int* ReadLabels (char* inBuffer, double* outBuffer, size_t* outBufNum)
{
    int inBufNum = 0;
    int* labels = (int*) calloc (10, sizeof(int) );
    int labelNum = 0;

    while ( (inBuffer[inBufNum] != '.') || (inBuffer[inBufNum - 1] != '\n') )
    {
        while (isspace(inBuffer[inBufNum]))
            inBufNum++;
        (*outBufNum)++;
        if ( (inBuffer[inBufNum] == ':') && (inBuffer[inBufNum - 1] == '\n') )
        {
            inBuffer[inBufNum] = ' ';
            if ( inBuffer[inBufNum + 1] != EOF )
            {
                inBufNum++;
                if ( !isspace(inBuffer[inBufNum + 1]) )
                {
                    free (labels);
                    assert (!"WRONG_LABEL");
                }
                labelNum = atoi ( &inBuffer[inBufNum] );
                if ( labels[labelNum] )
                {
                    free (labels);
                    assert (!"LABELS_OVERFLOW");
                }
                labels[labelNum] = *outBufNum;
                (*outBufNum)--;
                inBuffer[inBufNum] = ' ';
                inBufNum++;
            }
        }
        if ( (inBuffer[inBufNum] == '.') && (inBuffer[inBufNum - 1] == '\n') )
        {
            outBuffer[0] = *outBufNum - 1;
            *outBufNum = 1;
            return labels;
        }
        else
        {
            while ( !isspace(inBuffer[inBufNum]) )
                inBufNum++;
        }
    }
    outBuffer[0] = *outBufNum;
    outBuffer[1] = ' ';
    *outBufNum = 2;
    return labels;
}

int ReadCmd (char* inBuffer, size_t* inBufNum, double* outBuffer, size_t* outBufNum, int* labels)
{
    char command[50] = {0};
    int cmdPtr = 0;
    int labelNum = 0;

    while ( isspace(inBuffer[*inBufNum]) )
    {
        (*inBufNum)++;
    }

    while ( !isspace(inBuffer[*inBufNum]) && (inBuffer[*inBufNum] != '.') )
    {
        command[cmdPtr] = inBuffer[*inBufNum];
        cmdPtr++;
        (*inBufNum)++;
    }
    if (inBuffer[*inBufNum] != EOF)
        (*inBufNum)++;

    if ( !strcmp (command, "PUSH") )
    {
        outBuffer [*outBufNum] = PUSH_CODE;
        (*outBufNum)++;
        outBuffer [*outBufNum] = atof (&inBuffer[*inBufNum]);
        (*outBufNum)++;
        *inBufNum = ( FirstSpace(&inBuffer[*inBufNum]) - inBuffer) / sizeof (char) + 1;
        return 0;
    }

    if ( !strcmp (command, "PUSHREG") )
    {
        outBuffer [*outBufNum] = PUSHREG_CODE;
        (*outBufNum)++;
        (*inBufNum)++;
        if ( (inBuffer[(*inBufNum) + 1] == 'x') && (isspace(inBuffer[(*inBufNum) + 2])) )
        {
            if ( inBuffer[*inBufNum] == 'A' )
                outBuffer [*outBufNum] = AX_REG;
            if ( inBuffer[*inBufNum] == 'B' )
                outBuffer [*outBufNum] = BX_REG;
            if ( inBuffer[*inBufNum] == 'C' )
                outBuffer [*outBufNum] = CX_REG;
            if ( inBuffer[*inBufNum] == 'D' )
                outBuffer [*outBufNum] = DX_REG;
            else return WRONG_CMD_CODE;
        }
        (*inBufNum) += 2;
        return 0;
    }

    if ( !strcmp (command, "POP") )
    {
        outBuffer [*outBufNum] = POP_CODE;
        (*outBufNum)++;
        return 0;
    }

    if ( !strcmp (command, "POPREG") )
    {
        outBuffer [*outBufNum] = POPREG_CODE;
        (*outBufNum)++;
        (*inBufNum)++;
        if ( (inBuffer[(*inBufNum) + 1] == 'x') && isspace(inBuffer[(*inBufNum) + 2]) )
        {
            if ( inBuffer[*inBufNum] == 'A' )
                outBuffer [*outBufNum] = AX_REG;
            if ( inBuffer[*inBufNum] == 'B' )
                outBuffer [*outBufNum] = BX_REG;
            if ( inBuffer[*inBufNum] == 'C' )
                outBuffer [*outBufNum] = CX_REG;
            if ( inBuffer[*inBufNum] == 'D' )
                outBuffer [*outBufNum] = DX_REG;
            else return WRONG_CMD_CODE;
        }
        (*inBufNum) += 2;
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

    if ( !strcmp (command, "PROGRAMM_END") )
        return PROGRAMM_END_CODE;

    if ( !strcmp (command, "JE") )
    {
        outBuffer[*outBufNum] = JE_CODE;
        (*outBufNum)++;
        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;
        if ( (inBuffer[*inBufNum] == ':') && isspace(inBuffer[(*inBufNum) + 2]) )
        {
            (*inBufNum)++;
            labelNum = atoi (&inBuffer[*inBufNum]);
            outBuffer[*outBufNum] = labels[labelNum];
            (*inBufNum)++;
            (*outBufNum)++;
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
        labelNum = atoi (&inBuffer[*inBufNum]);
        outBuffer[*outBufNum] = labels[labelNum];
        (*inBufNum)++;
        (*outBufNum)++;
        return 0;
    }

    if ( !strcmp (command, "JB") )
    {
        outBuffer[*outBufNum] = JB_CODE;
        (*outBufNum)++;
        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;
        labelNum = atoi (&inBuffer[*inBufNum]);
        outBuffer[*outBufNum] = labels[labelNum];
        (*inBufNum)++;
        (*outBufNum)++;
        return 0;
    }

    if ( !strcmp (command, "JG") )
    {
        outBuffer[*outBufNum] = JG_CODE;
        (*outBufNum)++;
        while ( isspace(inBuffer[*inBufNum]) )
            (*inBufNum)++;
        labelNum = atoi (&inBuffer[*inBufNum]);
        outBuffer[*outBufNum] = labels[labelNum];
        (*inBufNum)++;
        (*outBufNum)++;
        return 0;
    }

    else return WRONG_CMD_CODE;
}
