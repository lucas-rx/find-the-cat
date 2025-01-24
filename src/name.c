#include "main.h"

bool regexMatch(char* name, char* token)
{
    regex_t regex;
    int res;
    bool etat = true;

    char strToMatch[1000] = "\0";
    //strcat(strToMatch, "^");
    strcat(strToMatch, token);
    //strcat(strToMatch, "$");

    // Compilation REGEX
    res = regcomp(&regex, strToMatch, 0);
    if (res)
    {
        fprintf(stderr, "Could not compile regex\n");
        exit(EXIT_FAILURE);
    }

    // Execution REGEX
    res = regexec(&regex, name, 0, NULL, 0);
    if (res == REG_NOMATCH)
    {
        etat = false;
    }

    return etat;
}