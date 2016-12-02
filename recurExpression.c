#include <TXLib.h>

int GetN (char* buffer, size_t* currentP);
int GetP (char* buffer, size_t* currentP);
int GetT (char* buffer, size_t* currentP);
int GetE (char* buffer, size_t* currentP);
int G0 (char* buffer, size_t* currentP);

int main()
{
    char buffer[20] = "42/(2+4)/7";
    size_t current = 0;
    printf ("%d", GetE (buffer, &current)); //GetN (buffer, current)
    return 0;
}

int GetN (char* buffer, size_t* currentP)
{
    int result = 0;
    while ( isdigit(buffer[*currentP]) )
        result = result*10 + buffer[(*currentP)++] - 48;
    return result;
}

int GetP (char* buffer, size_t* currentP)
{
    int result = 0;
    if ( buffer[*currentP] == '(' )
    {
        (*currentP)++;
        while ( buffer[*currentP] != ')' )
            result = GetE (buffer, currentP);
        (*currentP)++;
    }
    else result = GetN (buffer, currentP);

    return result;
}

int GetT (char* buffer, size_t* currentP)
{
    int result = GetP (buffer, currentP);
    while ( (buffer[*currentP] == '*') || (buffer[*currentP] == '/') )
    {
        if (buffer[*currentP] == '*')
        {
            (*currentP)++;
            result *= GetP (buffer, currentP);
        }
        if (buffer[*currentP] == '/')
        {
            (*currentP)++;
            result /= GetP (buffer, currentP);
        }
    }
    return result;
}

int GetE (char* buffer, size_t* currentP)
{
    int result = GetT (buffer, currentP);
    while ( (buffer[*currentP] == '+') || (buffer[*currentP] == '-') )
    {
        if (buffer[*currentP] == '+')
        {
            (*currentP)++;
            result += GetN (buffer, currentP);
        }
        if (buffer[*currentP] == '-')
        {
            (*currentP)++;
            result -= GetN (buffer, currentP);
        }
    }
    return result;
}

int G0 (char* buffer, size_t* currentP)
{
    int result = GetE (buffer, currentP);
    return result;
}
