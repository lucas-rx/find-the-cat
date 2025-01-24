#include "main.h"

bool ctc(char* pathOfFile, char* token)
{
    char content[BUFFER_FILE];
    bool etat = true;

    for (int i = 0; i < BUFFER_FILE; i++)
    {
        content[i] = '\0';
    }
    
    char c;
    int i = 0;
    FILE *file;
    file = fopen(pathOfFile, "r");
    if (file)
    {
        while ((c = getc(file)) != EOF)
        {
            content[i] = c;
            i++;
        }
        fclose(file);
    }
    //printf("Content: %s, Token: %s, Result: %d\n", content, token, regexMatch(content, token));
    if (regexMatch(content, token) == false)
    {
        etat = false;
    }

    return etat;
}