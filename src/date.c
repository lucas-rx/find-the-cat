#include "main.h"

date_params_t checkDate(struct stat fichier, char* date, bool is_flag_color)
{
    date_params_t date_param;
    time_t now = time(NULL);
    double diff_d;
    diff_d = difftime(now, fichier.st_atim.tv_sec);
    int diff = (int) diff_d;
    date_param.etat = true;
    //printf("Valeur date: %s\n", date);
    //printf("Last access: %d\t", diff/60);

    bool inverse = false;
    int sec = -1;

    if (strchr(date, '+') != NULL)
    {
        inverse = true;
        strcpy(date, date+1);
    } 

    if (strchr(date, 'm') != NULL && strchr(date, 'o') == NULL)
    {
        date[strlen(date) - 1] = '\0';
        // Test vérifiant si l'argument entré est bien un nombre entier
        if (!estNombreEntier(date))
        {
            print_error("Erreur : -date.\n", 'r', 1, is_flag_color);
            print_error("Le paramètre donné à -date doit être un nombre entier.\nExemple : ftc . -date 30m\n", 'r', 0, is_flag_color);
            exit(EXIT_FAILURE);
        }
        int min = atoi(date);
        sec = 60 * min;
    }
    else if (strchr(date, 'h') != NULL && strchr(date, 'o') == NULL)
    {
        date[strlen(date) - 1] = '\0';
        // Test vérifiant si l'argument entré est bien un nombre entier
        if (!estNombreEntier(date))
        {
            print_error("Erreur : -date.\n", 'r', 1, is_flag_color);
            print_error("Le paramètre donné à -date doit être un nombre entier.\nExemple : ftc . -date 30m\n", 'r', 0, is_flag_color);
            exit(EXIT_FAILURE);
        }
        int hour = atoi(date);
        sec = 60 * 60 * hour;
    }
    else if (strchr(date, 'j') != NULL)
    {
        date[strlen(date) - 1] = '\0';
        // Test vérifiant si l'argument entré est bien un nombre entier
        if (!estNombreEntier(date))
        {
            print_error("Erreur : -date.\n", 'r', 1, is_flag_color);
            print_error("Le paramètre donné à -date doit être un nombre entier.\nExemple : ftc . -date 30m\n", 'r', 0, is_flag_color);
            exit(EXIT_FAILURE);
        }
        int day = atoi(date);
        sec = 24 * 60 * 60 * day;
    }
    else
    {
        if (strstr(date, "now") != NULL)
        {
            // Recherche 1 min
            sec = 60;
        }
        else if (strstr(date, "today") != NULL)
        {
            // Recherche 1 jour
            sec = 60 * 60 * 24;
        }
        else if (strstr(date, "yesterday") != NULL)
        {
            // Recherche 2 jours
            sec = 60 * 60 * 24 * 2;
        }
        else if (strstr(date, "this\0month") != NULL)
        {
            // Recherche 30 jours
            sec = 60 * 60 * 24 * 30;
        }
        else
        {
            print_error("Erreur : -date.\n", 'r', 1, is_flag_color);
            print_error("Le paramètre donné à -date n'est pas accepté.\nLe paramètre doit être un nombre entier suivi du caractère 'm' pour minute, 'h' pour heure, ou 'j' pour jour.\nOn peut optionnellement mettre un + avant le nombre entier pour inverser le fonctionnement de la commande.\nExemples :\n./ftc . -date 30m\n./ftc . -date +5j\n", 'r', 0, is_flag_color);
            exit(EXIT_FAILURE);
        }
    }

    if (sec != -1)
    {
        if (sec > diff && inverse)
        {
            date_param.etat = false;
        }
        else if (sec < diff && !inverse)
        {
            date_param.etat = false;
        }
    }

    if (sec != -1)
    {
        // Temps inférieur à 1m
        if (diff < 60)
        {
            date_param.color = 'c'; // Cyan
        }
        // Temps compris entre 1m et 10m
        else if (diff >= 60 && diff < 10 * 60)
        {
            date_param.color = 'b'; // Bleu
        }
        // Temps compris entre 10m et 1h
        else if (diff >= 10 * 60 && diff < 60 * 60)
        {
            date_param.color = 'p'; // Violet
        }
        // Temps compris entre 1h et 24h
        else if (diff >= 60 * 60 && diff < 24 * 60 * 60)
        {
            date_param.color = 'g'; // Vert
        }
        // Temps compris entre 1j et 7j
        else if (diff >= 24 * 60 * 60 && diff < 7 * 24 * 60 * 60)
        {
            date_param.color = 'y'; // Jaune
        }
        // Temps supérieur à 7j
        else
        {
            date_param.color = 'r'; // Rouge
        }
    }
    //printf("date_param.color : %c\n", date_param.color);
    return date_param;
}