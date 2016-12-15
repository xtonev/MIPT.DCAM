#include "5.hash.h"

size_t SizeOfFile (FILE* file);

int main()
{
    struct hash* tmp = HashTabCtor (3);
    FILE* test = fopen ("Onegin.txt", "r");
    size_t oneginSize = SizeOfFile (test);
    char* buffer = (char*) calloc (oneginSize + 1, sizeof(char) );
    fread (buffer, sizeof(char), oneginSize, test);
    fclose (test);
    buffer[oneginSize] = '\0';

    size_t bufCounter = 0;
    char* word = NULL;
    size_t listCounter = 0;

    while (bufCounter < oneginSize)
    {
        while ( (isspace(buffer[bufCounter])) && (bufCounter < oneginSize) )
            bufCounter++;
        if (isalnum(buffer[bufCounter]))
        {
            word = &buffer[bufCounter];
            while ( !isspace(buffer[bufCounter]) )
                bufCounter++;
            buffer[bufCounter] = '\0';
            bufCounter++;
            if ( !FindElemHash(tmp, word, HashFnc5) )
                InsertElem (tmp, word, HashFnc5);
        }
        else if (bufCounter < oneginSize)
            bufCounter++;
    }
    free (buffer);

    FILE* results = fopen ("hashResults.csv", "a");
    fprintf (results, "Sixth function: just funny fnc ");
    for (; listCounter < tmp->hashSize - 1; listCounter++)
        fprintf (results, "%d ", tmp->H[listCounter]->listSize);
    fprintf (results, "%hu\n", tmp->H[listCounter]->listSize);
    fclose (results);
    HashTabDtor (&tmp);
}

size_t SizeOfFile (FILE* file)
{
    fseek (file, 0, SEEK_END);
    size_t fileSize = ftell (file) / sizeof (char);
    rewind (file);
    return fileSize;
}
