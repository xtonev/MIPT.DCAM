#include <TXLib.h>
#include <ctype.h>

const int MAXLINE = 1024;

void normaliz (char s[]);

int main()
{
    FILE *file1 = fopen ("str1.txt", "r"), *file2 = fopen ("str2.txt", "w");
    assert (file1);
    assert (file2);

    char str[2][MAXLINE] = {0};
    char *str1p = 0, *str2p = 0;
    int i = 0;

    if ( (str1p = fgets (str[i], MAXLINE - 2, file1) ) == NULL )
        {
        if ( !feof (file1) )
            printf ("Error.\n");
        else printf ("Empty file.\n");
        }

    normaliz (str[i]);

    while ( (str2p = fgets (str[(i + 1) % 2], MAXLINE - 2, file1) ) != NULL )
        {
         normaliz (str[(i + 1) % 2]);
         if (strcmp (str[i % 2], str[(i + 1) % 2]) != 0)
            {
            fprintf (file2, "%s", str[i % 2]);
            i++;
            }
        }

    fprintf (file2, "%s", str[i % 2]);

    fclose (file1);
    fclose (file2);
    return 0;
}

void normaliz (char s[])
    {
    int i = 0, j = 0, k = strlen (s);

    for (; isspace (s[j]); j++)
            ;

    while (j < k)
        {
        if ( !isspace (s[j]) )
            s[i] = s[j];
        else if ( !isspace (s[j - 1]) )
                s[i] = s[j];
        i++, j++;
        }

    if (isspace (s[i - 1]))
        {
        s[i - 1] = '\n';
        s[i] = '\0';
        }
    else s[i] = '\n';
    i++;

    while (i < k)
        {
        s[i] = '\0';
        i++;
        }
    }


