#include <TXLib.h>

unsigned long myfsize (FILE *file);
unsigned long changeNGiveLinesQuant (char *buffer, unsigned long fsize);
int revstrcmpp (char* ptr1, char* ptr2);

int main()
    {
    FILE *file1 = fopen ("str1.txt", "r");
    FILE *file2 = fopen ("str3.txt", "w");
    assert (file1), assert (file2), assert (file1 != file2);

    unsigned long fsize = myfsize (file1);
    assert (fsize);

    char *buffer = (char*) calloc (fsize + 1, sizeof (char) );
    if (buffer == NULL)
        printf ("Memory error.\n");

    fread (buffer, sizeof (char), fsize, file1);
    unsigned long k = changeNGiveLinesQuant (buffer, fsize);

    char *lnptr[k];
    unsigned long i = 0, j = 0;
    for (i = 0; i < fsize; i++)
        {
        if (buffer[i] == '\0')
            lnptr[j++] = &buffer[i - 1];
        }

    qsort (lnptr, k, fsize, (int (*) (const void*, const void*))revstrcmpp);

    for (i = 0; i <= k; i++)
        fprintf (file2, "%s\n", lnptr[i]);

    fclose (file1); fclose (file2);
    free (buffer);
    return 0;
    }

unsigned long changeNGiveLinesQuant (char *buffer, unsigned long fsize)
    {
    unsigned long i = 0;
    unsigned int k = 0;

    for (; i < fsize; i++)
        {
        if (buffer[i] == '\n')
            {
            buffer[i] = '\0';
            k++;
            }
        }

    if (buffer[fsize] != '\0')
        buffer[fsize] = '\0';

    return k;
    }

unsigned long myfsize (FILE *file1)
    {
    fseek (file1, 0, SEEK_END);
    unsigned long fsize = ftell (file1);
    rewind (file1);

    return fsize;
    }

int revstrcmpp (char* ptr1, char* ptr2)
    {
    for (; ( tolower(*ptr1) == tolower (*ptr2) ) && (*ptr1 != '\0') && (*ptr2 != '\0'); ptr1--, ptr2--)
        {
        if ( !isalnum (*ptr1) )
            ptr2++;
        if ( !isalnum (*ptr2) )
            ptr1++;
        }
    return *ptr1 - *ptr2;
    }

