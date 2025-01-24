#include "main.h"

bool check_perms(struct stat fichier, char wanted_perm[])
{
    //printf("Perm souhaitée : %s\n", wanted_perm);
    //printf("Perms du fichier : %o\n", fichier.st_mode);
    if (S_ISREG(fichier.st_mode)) // Est un fichier ordinaire
    {
        char file_perms[7];
        sprintf(file_perms, "%o", fichier.st_mode);
        //printf("file_perms : %s\n", file_perms);
        int read_perm = file_perms[3] - '0';
        int write_perm = file_perms[4] - '0';
        int execute_perm = file_perms[5] - '0';
        //printf("%d %d %d\n", read_perm, write_perm, execute_perm);
        //printf("product : %d, atoi : %d\n", 100 * read_perm + 10 * write_perm + execute_perm, atoi(wanted_perm));
        if (100 * read_perm + 10 * write_perm + execute_perm == atoi(wanted_perm))
        {
            return true;
        }
    }
    return false;
}
