#include <TXLib.h>

unsigned long changeNAndGiveSize (char *buffer, unsigned long fsize);
unsigned long myFsize (FILE *file);
int strcmpp (const void* ptr1, const void* ptr2);
int strcmppp (const void* a, const void* b);
int strcmppp (const void* ptr1, const void* ptr2);

int main()
    {
    FILE *rfile = fopen ("str1.txt", "r");
    FILE *wfile = fopen ("str2.txt", "w");
    assert (rfile), assert (wfile), assert (rfile != wfile);

    unsigned long fsize = myFsize (rfile);

    char *buffer = (char*) calloc (fsize + 1, sizeof (char) );
    if (buffer == NULL)
        assert(!"Calloc can not allocate memory for me (");

    fread (buffer, sizeof (char), fsize, rfile);

    unsigned long linenum = changeNAndGiveSize (buffer, fsize);

    char *lineptr[linenum];
    unsigned long i = 0, j = 0;
    lineptr[j++] =  &buffer[0];
    for (i = 0; i < fsize; i++)
        {
        if ( (buffer[i] == '\0') && ( (i + 1) < fsize ) )
            lineptr[j++] = &buffer[i + 1];
        }
    for (i = 0; i <= linenum; i++)
        printf ("%s\n", lineptr[i]);

    qsort (lineptr, linenum, sizeof (lineptr[0]), (int (*)(const void*, const void*))strcmp);

    for (i = 0; i <= linenum; i++)
        printf ("%s\n", lineptr[i]);
//        fprintf (file2, "%s\n", lnptr[i]);
// realloc
    fclose (wfile);
    fclose (rfile);
    free (buffer);
    return 0;
    }

unsigned long changeNAndGiveSize (char *buffer, unsigned long fsize)
                              // nPtr = strchr (buffer, '\n')
    {                         // if (!nPtr) then all '\n' are found
    unsigned long i = 0;      // *nPtr = '\0';
    unsigned int k = 0;       // linePtr[i++] = nPtr + 1
                              // lineNum =
    for (; i < fsize; i++)
        {
        if (buffer[i] == '\n')
            {
            buffer[i] = '\0';
            k++;
            }
        }

    if (buffer[fsize - 1] != '\0')
        buffer[fsize] = '\0';

    return k;
    }

unsigned long myFsize (FILE *file1)
    {
    fseek (file1, 0, SEEK_END);
    unsigned long fsize = ftell (file1);
    rewind (file1);
    assert (fsize);
    return fsize;
    }

int strcmppp (const void* ptr1, const void* ptr2)
{
    return strcmp ((char*)ptr1, (char*)ptr2);
}

int strcmpp (const void* ptr1, const void* ptr2)
    {
    unsigned long *p1 = (unsigned long*) ptr1;
    unsigned long *p2 = (unsigned long*) ptr2;
    for (; ( tolower((char)*p1) == tolower ((char)*p2) ) && ((char)*p1 != '\0') && ((char)*p2 != '\0'); p1++, p2++)
        {
        if ( !isalnum ((char)*p1) ^ !isalnum ((char)*p2) )
            if ( !isalnum ((char)*p1) )
                p2--;
            if ( !isalnum ((char)*p2) )
                p1--;
        }
    return (char)*p1 - (char)*p2;
    }





    // isalpha     ctype ñâÿçàí ñ àñêè  ìëàäøèé áèò - óèôðà  ñëåäóþùèé - áóêâà


   //óêàçàòåëü íà ôóíêöèè ñòðóêòóðû äàííûõ ñòåê
