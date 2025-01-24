#include "main.h"

void parseSizeParameter(size_params_t* size_params, char* parameter, bool is_flag_color)
{
    //printf("Paramètre : %s\n", parameter);
    int length = strlen(parameter);

    bool has_symbole_taille = true;
    // Permet de savoir si le paramètre se termine par une lettre.
    // Utile pour déterminer la taille

    bool has_signe = true;
    // Permet de savoir si le paramètre commence par un signe.
    // Utile pour déterminer la taille

    // Signe

    if (parameter[0] == '-')
    {
        size_params -> signe = '-';
    }
    else if (parameter[0] == '+')
    {
        size_params -> signe = '+';
    }
    else if (parameter[0] >= '0' && parameter[0] <= '9')
    {
        size_params -> signe = 'o'; // Signe vide
        has_signe = false;
    }
    else
    {
        print_error("Erreur : -size.\n", 'r', 1, is_flag_color);
        print_error("Le signe saisi est incorrect. Le signe peut être :\n+ : taille strictement supérieure à la borne indiquée\n- : taille strictement inférieure à la borne indiquée\n(aucun) : taille égale à la borne indiquée\n", 'r', 0, is_flag_color);
        exit(EXIT_FAILURE);
    }

    // Symbole

    if (parameter[length - 1] == 'G') // 71 : code ASCII de G (gibibytes)
    {
        size_params -> symbole_taille = 'G';
    }

    else if (parameter[length - 1] == 'M')  // 77 : code ASCII de M (mebibytes)
    {
        size_params -> symbole_taille = 'M';
    }

    else if (parameter[length - 1] == 'k')  // 107 : code ASCII de k (kibibytes)
    {
        size_params -> symbole_taille = 'k';
    }

    else if (parameter[length - 1] == 'c') // 99 : code ASCII de c (bytes)
    {
        size_params -> symbole_taille = 'c';
    }

    else if (parameter[length - 1] >= '0' && parameter[length - 1] <= '9')
    // Est un chiffre entre 0 et 9 (48 : code ASCII de 0, 57 : code ASCII de 9)
    {
        size_params -> symbole_taille = 'c'; // 'c' est le symbole par défaut
        has_symbole_taille = false;
    }

    else // Erreur
    {
        print_error("Erreur : -size.\n", 'r', 1, is_flag_color);
        print_error("Le symbole de taille saisi est incorrect. Le symbole de taille peut être :\nc ou (aucun) : 1 byte\nk : 1.024 bytes\nM : 1.048.576 bytes\nG : 1.073.741.824 bytes\n", 'r', 0, is_flag_color);
        exit(EXIT_FAILURE);
    }

    // Taille

    int debut_recherche = 0;
    if (has_signe)
    {
        debut_recherche++; // Le paramètre a un signe au début : on l'ignore
    }

    int fin_recherche = length;
    if (has_symbole_taille)
    {
        fin_recherche--; // Le paramètre a un symbole à la fin : on le parcourt alors pour i = 1 à length - 2
    }
    
    if (debut_recherche == fin_recherche) // Pas de taille saisie
    {
        print_error("Erreur : -size.\n", 'r', 1, is_flag_color);
        print_error("La taille saisie doit être un entier positif ou nul.\n", 'r', 0, is_flag_color);
        exit(EXIT_FAILURE);
    }

    char* parameterBuffer = calloc(1, sizeof(fin_recherche - debut_recherche + 1));
    //printf("Buffer avant : %s\n", parameterBuffer);

    for (int i = debut_recherche; i < fin_recherche; i++)
    {
        if (!(parameter[i] >= '0' && parameter[i] <= '9'))
        {
            print_error("Erreur : -size.\n", 'r', 1, is_flag_color);
            print_error("La taille saisie doit être un entier positif ou nul.\n", 'r', 0, is_flag_color);
            exit(EXIT_FAILURE);
        }
        strncat(parameterBuffer, &parameter[i], 1);
    }
    //printf("Buffer après : %s\n", parameterBuffer);
    size_params -> taille = atol(parameterBuffer);
    //printf("Mémoire allouée : %d\n", fin_recherche - debut_recherche);
    //printf("Taille : %ld\n", size_params -> taille);

    free(parameterBuffer);
}