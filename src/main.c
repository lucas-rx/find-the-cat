#include "main.h"

int main(int argc, char *argv[ ])
{
    // Initialisation des paramètres de recherche
    option_t option;
    option.basique = false;
    option.nom = false;
    option.taille = false;
    option.date = false;
    option.mime = false;
    option.ctc = false;
    option.dir = false;
    option.perm = false;
    option.color = false;
    option.ou = false;

    val_t valeur;
    for (int i = 0; i < 1000; i++)
    {
        valeur.nom[i] = '\0';
        valeur.date[i] = '\0';
        valeur.ctc[i] = '\0';
    }

    size_params_t* size_params = calloc(1, sizeof(size_params));
    size_params -> signe = 'o'; // Signe vide
    size_params->taille = -1;
    // Initialiser size_params avec calloc : évite un warning

    mime_params_t* mime_params = calloc(1, sizeof(mime_params));
    mime_params->has_subtype = false;

    //printf("[main.c] %s | %s\n", mime_params->type, mime_params->subtype);
    //printf("%ld | %ld\n", strlen(mime_params -> type), strlen(mime_params -> subtype));
    
    // Affichage en couleur ?
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-color") == 0)
        {
            option.color = true;
        }
    }

    if (argc < 2)
    {
        fprintf(stderr, "Erreur : aucun dossier de départ n'a été précisé.\nLe synopsis de ftc est : ftc starting-point [-option [parametre]].\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 2)
    {
        option.basique = true;
        printf("%s\n", argv[1]);
    }
    else
    {
        for (int i = 2 ; i < argc ; i++)
        {
            // Vérifie si les options saisies sont correctes ou non
            options_legales(argc, argv, option.color);
            
            // Teste si les strings sont égales
            if (strcmp(argv[i], "-test") == 0 && i < argc - 2)
            {
                if (estUneOption(argv[i+1]))
                {
                    printf("La valeur du flag %s est %s\n", argv[i+1], argv[i+2]);
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    fprintf(stderr, "Le flag %s n'est pas correct\n", argv[i + 1]);
                    exit(EXIT_FAILURE);
                }
            }

            // Teste si -test a au moins deux arguments derrière lui
            if (strcmp(argv[i], "-test") == 0 && i >= argc - 2)
            {
                print_error("Erreur : -test.\n", 'r', 1, option.color);
                print_error("L'option -test doit être suivie par une option et son paramètre.\nExemple : ./ftc . -test -name mon_fichier.txt\n", 'r', 0, option.color);
                exit(EXIT_FAILURE);
            }

            // Cas option -name
            if (strcmp(argv[i], "-name") == 0)
            {
                if (i == argc - 1)
                {
                    print_error("Erreur : -name.\n", 'r', 1, option.color);
                    print_error("L'option -name doit être suivie par un paramètre indiquant le nom du fichier à rechercher.\nCe paramètre peut être une chaîne de caractères ou une regex.\nExemples :\n./ftc . -name mon_fichier.txt - Recherche les fichiers nommés mon_fichier.txt\n./ftc . -name \"\\.c$\" - Récupère tous les fichiers en .c\n", 'r', 0, option.color);
                    exit(EXIT_FAILURE);
                }
                else if (i < argc - 1)
                {
                    option.nom = true;
                    strncat(valeur.nom, argv[i+1], 1000);
                }
            }

            // Cas option -size
            if (strcmp(argv[i], "-size") == 0)
            {
                if (i == argc - 1)
                {
                    print_error("Erreur : -size.\n", 'r', 1, option.color);
                    print_error("L'option -size doit être suivie par un paramètre indiquant la taille des fichiers à rechercher.\nCe paramètre est de la forme [\\+\\-]?[0-9]+[ckMG]?.\nExemples :\n./ftc . -size +9k\n./ftc . -size 15\n./ftc . -size -100G\n", 'r', 0, option.color);
                    exit(EXIT_FAILURE);
                }
                else if (i < argc - 1)
                {
                    option.taille = true;
                    parseSizeParameter(size_params, argv[i + 1], option.color);
                    valeur.size_params = *size_params;
                }
            }

            // Cas option -date
            if (strcmp(argv[i], "-date") == 0)
            {
                if (i == argc - 1)
                {
                    print_error("Erreur : -date.\n", 'r', 1, option.color);
                    print_error("L'option -date nécessite un paramètre.\nLe paramètre doit être un nombre entier suivi du caractère 'm' pour minute, 'h' pour heure ou 'j' pour jour.\nExemple : ./ftc . -date 30m\n", 'r', 0, option.color);
                    exit(EXIT_FAILURE);
                }
                else if (i < argc - 1)
                {
                    option.date = true;
                    strncat(valeur.date, argv[i+1], 1000);
                }
            }

            // Cas option -mime
            if (strcmp(argv[i], "-mime") == 0)
            {
                if (i == argc - 1)
                {
                    print_error("Erreur : -mime.\n", 'r', 1, option.color);
                    print_error("L'option -mime doit être suivie par un paramètre : le type MIME des fichiers recherchés.\nExemples :\n./ftc . -mime text - Recherche selon le type\n./ftc . -mime text/html - Recherche selon le type et le sous-type\n", 'r', 0, option.color);
                    exit(EXIT_FAILURE);
                }
                else if (i < argc - 1)
                {
                    option.mime = true;
                    parse_mime_parameter(mime_params, argv[i + 1], option.color);
                    valeur.mime_params = *mime_params;
                }
            }

            // Cas option -ctc
            if (strcmp(argv[i], "-ctc") == 0)
            {
                if (i == argc - 1)
                {
                    print_error("Erreur : -ctc.\n", 'r', 1, option.color);
                    print_error("L'option -ctc doit être suivie par un paramètre.\nCe paramètre est une chaîne de caractères ou une regex correspondant au contenu recherché à l'intérieur des fichiers.\nExemple :\n./ftc . -ctc \"coucou\"\n", 'r', 0, option.color);
                    exit(EXIT_FAILURE);
                }
                else if (i < argc - 1)
                {
                    option.ctc = true;
                    strncat(valeur.ctc, argv[i+1], 1000);
                }
            }

            // Cas option -dir
            if (strcmp(argv[i], "-dir") == 0)
            {
                // Cas où un paramètre est passé à -dir
                if (i < argc - 1)
                {
                    option.nom = true;
                    strncat(valeur.nom, argv[i+1], 1000);
                }
                option.dir = true;
            }

            if (strcmp(argv[i], "-perm") == 0)
            {
                if (i == argc - 1)
                {
                    print_error("Erreur : -perm.\n", 'r', 1, option.color);
                    print_error("L'option -perm doit être suivie par un paramètre.\nCe paramètre est une chaîne de caractères correspondant aux permissions des fichiers recherchés, au format octal.\nExemple :\n./ftc . -perm 775\n", 'r', 0, option.color);
                    exit(EXIT_FAILURE);
                }
                else if (i < argc - 1)
                {
                    option.perm = true;
                    strncat(valeur.perm, argv[i + 1], 3);
                }
            }

            if (strcmp(argv[i], "-ou") == 0)
            {
                option.ou = true;
            }
            
            // Affichage pour test
            //printf("%s\t", argv[i]);
        }
    }

    //Modification starting point
    if (argv[1][strlen(argv[1])-1] == '/')
    {
        argv[1][strlen(argv[1])-1] = '\0';
    }
    
    // Recherche des fichiers vérifiant les critères imposés
    lsRecursif(argv[1], option, valeur);

    free(size_params);
    free(mime_params);

    return EXIT_SUCCESS;
}
