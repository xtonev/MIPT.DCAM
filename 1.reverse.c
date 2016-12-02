#include <TXLib.h>
#include <ctype.h>

const int MAXLINE = 1024;

void myreverse (char s[]);
void normaliz (char s[]);

int main()
{
    FILE *file1 = fopen ("str1.txt", "r"), *file2 = fopen ("str2.txt", "w");
    assert (file1);
    assert (file2);

    char s[MAXLINE] = {0}, *pstr = 0;

    while  ( (pstr = fgets (s, MAXLINE - 1, file1)) != NULL )
    {
        normaliz (s);
        myreverse (s);
        fprintf (file2, "%s", s);
    }

    fclose (file1), fclose (file2);
    return 0;
}

void myreverse (char s[])
    {
    int c, i = 0, j = strlen (s) - 1;

    if (s[j] == '\n')
        j--;

    for (; i < j; i++, j--)
        {
        c = s[i];
        s[i] = s[j];
        s[j] = (char) c;
        }
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

