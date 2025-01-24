#include "main.h"
#include "../MegaMimes/src/MegaMimes.h"

bool estUneOption(char* option)
{
    char* optionsPossibles[NB_OPTIONS] = {"-test", "-name", "-size", "-date", "-mime", "-ctc", "-dir", "-color", "-perm", "-link", "-threads", "-ou"};
    for (int i = 0; i < NB_OPTIONS; i++) 
    {
        if (strcmp(option, optionsPossibles[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

void clear_buffer(char* buffer)
{
    for (int i = 0; i < strlen(buffer); i++)
    {
        buffer[i] = '\0';
    }
}

void options_legales(int length_tab, char* tab[], bool is_flag_color)
{
    for (int i = 2; i < length_tab; i++)
    {
        if (strcmp(tab[i], "-test") == 0)
        {
            i += 2;
        }
        else if (strcmp(tab[i], "-name") == 0 || strcmp(tab[i], "-size") == 0 || strcmp(tab[i], "-date") == 0 || strcmp(tab[i], "-mime") == 0 || strcmp(tab[i], "-ctc") == 0 || strcmp(tab[i], "-perm") == 0)
        {
            i += 1;
        }
        else if (strcmp(tab[i], "-dir") == 0)
        {
            if (i+1 < length_tab)
            {
                if (estUneOption(tab[i+1]))
                {
                    i += 0;
                }
                else
                {
                    i += 1;
                }
            }
            else
            {
                i += 0;
            }
        }
        else if (strcmp(tab[i], "-color") == 0)
        {
            i += 0;
        }
        else if (strcmp(tab[i], "-ou") == 0)
        {
            i += 0;
        }
        else
        {
            //printf("Erreur : l'option %s est inexistante.\n", tab[i]);
            char error_nonexistant_option[BUFFER_SIZE];
            sprintf(error_nonexistant_option, "Erreur : option.\nUne option inexistante a été saisie : %s.\nLes options existantes sont : -test, -name, -size, -date, -mime, -ctc, -dir, -color et -perm.\n", tab[i]);
            print_error(error_nonexistant_option, 'r', 1, is_flag_color);
            exit(EXIT_FAILURE);
        }
    }
}

bool estNombreEntier(char* token)
{
    // Regarde si on dans le cas particulier où le nombre entré est zéro
    if (!(strlen(token) == 1 && token[0] == '0'))
    {
        char cop[1000];
        strcpy(cop, token);
        int result = atoi(cop);
        if (result == 0)
        {
            return false;
        }
    }
    return true;
}

char* begin_color(char color, bool bold)
{
    char* begin_color_string = calloc(1, 10);
    strcat(begin_color_string, "\033[");
    
    if (bold)
    {
        strcat(begin_color_string, "1");
    }
    else
    {
        strcat(begin_color_string, "0");
    }

    strcat(begin_color_string, ";3");

    switch(color)
    {
        case 'k': // Black
            strcat(begin_color_string, "0");
            break;
        case 'r': // Red
            strcat(begin_color_string, "1");
            break;
        case 'g': // Green
            strcat(begin_color_string, "2");
            break;
        case 'y': // Yellow
            strcat(begin_color_string, "3");
            break;
        case 'b': // Blue
            strcat(begin_color_string, "4");
            break;
        case 'p': // Purple
            strcat(begin_color_string, "5");
            break;
        case 'c': // Cyan
            strcat(begin_color_string, "6");
            break;
        case 'w': // White
            strcat(begin_color_string, "7");
            break;
        default: // White (par défaut)
            strcat(begin_color_string, "7");
            break;
    }

    strcat(begin_color_string, "m");

    return begin_color_string;
}

char* end_color()
{
    char* end_color_string = calloc(1, 10);
    strcat(end_color_string, "\033[0m");
    return end_color_string;
}

void colorPrintf(char* token, char color, bool bold)
{
    char* begin_color_string = begin_color(color, bold);
    char* end_color_string = end_color();

    printf("%s%s%s", begin_color_string, token, end_color_string);

    free(begin_color_string);
    free(end_color_string);
}

void print_error(char* token, char color, bool bold, bool is_flag_color)
{
    if (is_flag_color) // -color est dans la commande
    {
        char* begin_color_string = begin_color(color, bold);
        char* end_color_string = end_color();

        fprintf(stderr, "%s%s%s", begin_color_string, token, end_color_string);

        free(begin_color_string);
        free(end_color_string);
    }
    else
    {
        fprintf(stderr, "%s", token);
    }
}

char* get_mime_type(char* path)
{
    char* mime_type = (char*) getMegaMimeType(path);
    if (mime_type == NULL) {
        return NULL;
    }
    char* mime_type_token = calloc(1, BUFFER_SIZE);
    clear_buffer(mime_type_token);
    //printf("MIME : %s\n", mime_type);

    int position = 0;
    while (mime_type[position] != '/' && position < 20)
    {
        position++;
    }
    strncat(mime_type_token, mime_type, position);
    //printf("MIME token : %s %ld\n", mime_type_token, strlen(mime_type_token));
    return mime_type_token;
}

void print_file(char* token, bool is_flag_color, char* mime_type)
{
    if (is_flag_color) // -color est dans la commande
    {
        char* mime_types[NB_MIME_TYPES] = {"application", "audio", "image", "text", "video"};
        char colors[NB_MIME_TYPES] = {'g', 'p', 'y', 'b', 'p'};
        char color = 'w';
        for (int i = 0; i < NB_MIME_TYPES; i++)
        {
            if (strcmp(mime_type, mime_types[i]) == 0)
            {
                color = colors[i];
            }
        }
        
        char* begin_color_string = begin_color(color, 0);
        char* end_color_string = end_color();

        printf("%s%s%s\n", begin_color_string, token, end_color_string);

        free(begin_color_string);
        free(end_color_string);
    }
    else
    {
        printf("%s\n", token);
    }
}

void print_file_date(char* token, bool is_flag_color, char color)
{
    if (is_flag_color) // -color est dans la commande
    {     
        char* begin_color_string = begin_color(color, 0);
        char* end_color_string = end_color();

        printf("%s%s%s\n", begin_color_string, token, end_color_string);

        free(begin_color_string);
        free(end_color_string);
    }
    else
    {
        printf("%s\n", token);
    }
}


