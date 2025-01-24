#include "main.h"
#include "../MegaMimes/src/MegaMimes.h"

void parse_mime_parameter(mime_params_t* mime_params, char* parameter, bool is_flag_color)
{
    /*
    printf("Type : %s\n", mime_params->type);
    printf("Sous-type : %s\n", mime_params->subtype);
    printf("Sous-type ? : %d\n", mime_params->has_subtype);
    printf("Longueurs : %ld, %ld\n", strlen(mime_params->type), strlen(mime_params->subtype));
    */

    if (parameter[0] == '/')
    {
        print_error("Erreur : -mime.\n", 'r', 1, is_flag_color);
        print_error("Type MIME incorrect saisi. Le format demandé est :\n- type/sous-type (exemple : text/html)\n- type (exemple : text)\n", 'r', 0, is_flag_color);
        exit(EXIT_FAILURE);
    }

    //const char** extensions = getMegaMimeExtensions("*/*");
    /*for (int i = 0; i < strlen(extensions); i++) {
        printf("%s\n", extensions[i]);
    }*/

    char* actual_token = strtok(parameter, "/");
    int nb_tokens = 1;

    while (actual_token != NULL)
    {
        //printf("Token %d : %s\n", nb_tokens, actual_token);
        switch(nb_tokens)
        {
            case 1:
                strncat(mime_params->type, actual_token, 50);
                break;

            case 2:
                strncat(mime_params->subtype, actual_token, 50);
                mime_params->has_subtype = true;
                break;

            default:
                print_error("Erreur : -mime.\n", 'r', 1, is_flag_color);
                print_error("Type MIME incorrect saisi. Le format demandé est :\n- type/sous-type (exemple : text/html)\n- type (exemple : text)\n", 'r', 0, is_flag_color);
                exit(EXIT_FAILURE);
                break;
        }
        actual_token = strtok(NULL, "/");
        nb_tokens++;
    }
    /*
    printf("Type : %s\n", mime_params->type);
    printf("Sous-type : %s\n", mime_params->subtype);
    printf("Sous-type ? : %d\n", mime_params->has_subtype);
    printf("Longueurs : %ld, %ld\n", strlen(mime_params->type), strlen(mime_params->subtype));
    */
}

bool check_mime_type(char* path, mime_params_t mime_params)
{
    bool etat = false;
    char* file_mime_type = (char*) getMegaMimeType(path);

    if (file_mime_type == NULL) {
        return etat;
    }

    char wanted_mime_type[BUFFER_SIZE];
    clear_buffer(wanted_mime_type);
    strncat(wanted_mime_type, mime_params.type, 50);

    if (mime_params.has_subtype)
    {
        strcat(wanted_mime_type, "/");
        //sprintf(wanted_mime_type, "/");
        strncat(wanted_mime_type, mime_params.subtype, 50);
        //printf("File : %s\nMIME type : %s\n", path, file_mime_type);
        //printf("Wanted MIME type : %s + length : %ld\n\n", wanted_mime_type, strlen(wanted_mime_type));
        //printf("\nfile_mime_type : %s | wanted_mime_type : %s\n", file_mime_type, wanted_mime_type);
        if (strcmp(file_mime_type, wanted_mime_type) == 0)
        {
            etat = true;
        } 
    }

    else if (!mime_params.has_subtype)
    {
        //printf("File : %s\nMIME type : %s + length : %ld\n", path, file_mime_type, strlen(file_mime_type));
        //printf("Wanted MIME type : %s + length : %ld\n\n", wanted_mime_type, strlen(wanted_mime_type));
        char file_mime_type_token[BUFFER_SIZE];
        clear_buffer(file_mime_type_token);
        int nb_char_to_cat = 0;
        while (file_mime_type[nb_char_to_cat] != '/')
        {
            nb_char_to_cat++;
        }
        strncat(file_mime_type_token, file_mime_type, nb_char_to_cat);
        //printf("\nfile_mime_type_token : %s (length : %d) | wanted_mime_type : %s\n", file_mime_type_token, nb_char_to_cat, wanted_mime_type);
        if (strcmp(file_mime_type_token, wanted_mime_type) == 0)
        {
            etat = true;
        }
    }
    return etat;
}
