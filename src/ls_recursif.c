#include "main.h"

void lsRecursif(char* startingPoint, option_t option, val_t valeur)
{
    //printf("Dossier courant : %s\n", startingPoint);
    DIR *dirp;
    struct dirent *dp;  
    struct stat fichier;
    dirp = opendir(startingPoint);
    

    // Si le starting point donné n'est pas un dossier
    if (dirp == NULL)
    {
        print_error("Erreur : starting-point incorrect.\n", 'r', 1, option.color);
        print_error("Le dossier spécifié comme point de départ de la commande n'existe pas, ou son ouverture a échoué.\n", 'r', 0, option.color);
        exit(EXIT_FAILURE);
    }
    
    while ((dp = readdir(dirp)) != NULL)
    {
        //printf("Dans le while : %s\n", dp->d_name);
        if (strcmp(dp->d_name, "..") != 0 && strcmp(dp->d_name, ".") != 0)
        {
            // Définition du path
            char realpathBuf[BUFFER_SIZE];
            char newestStartingPoint[BUFFER_SIZE];
            strncpy(newestStartingPoint, startingPoint, BUFFER_SIZE);
            strcat(newestStartingPoint,"/");
            strcat(newestStartingPoint, dp->d_name);
            realpath(newestStartingPoint, realpathBuf);

            // Initialisation stats du fichier
            stat(realpathBuf, &fichier);

            if (option.basique == false)
            {
                verif_params_t verif_param = verificationFichier(dp, option, valeur, fichier, realpathBuf);
                
                if (option.dir == true && dp->d_type == DT_DIR)
                {
                    if (verif_param.etat)
                    {
                        //printf("date color : %c\n", verif_param.color);
                        if (option.date)
                        {
                            print_file_date(newestStartingPoint, option.color, verif_param.color);
                        }
                        else
                        {
                            char* mime_type = get_mime_type(realpathBuf);
                            if (mime_type != NULL)
                            {
                                colorPrintf(newestStartingPoint, verif_param.color, 0);
                                free(mime_type);
                            }
                            else
                            {
                                printf("%s\n", newestStartingPoint);
                            }
                        }
                    }
                }
                else if (option.dir == false && dp->d_type != DT_DIR)
                {
                    if (verif_param.etat)
                    {
                        //printf("date color : %c\n", verif_param.color);
                        if (option.date)
                        {
                            print_file_date(newestStartingPoint, option.color, verif_param.color);
                        }
                        else
                        {
                            char* mime_type = get_mime_type(realpathBuf);
                            if (mime_type != NULL)
                            {
                                print_file(newestStartingPoint, option.color, mime_type);
                                free(mime_type);
                            }
                            else
                            {
                                printf("%s\n", newestStartingPoint);
                            }
                        }
                    }
                }
            }
            else
            {
                char* mime_type = get_mime_type(realpathBuf);
                if (mime_type != NULL)
                {
                    print_file(newestStartingPoint, option.color, mime_type);
                    free(mime_type);
                }
                else
                {
                    printf("%s\n", newestStartingPoint);
                }
            }            
        }

        if (strcmp(dp->d_name, "..") != 0 && strcmp(dp->d_name, ".") != 0)
        {
            if (dp->d_type == DT_DIR)
            {
                char newStartingPoint[BUFFER_SIZE];
                strncpy(newStartingPoint, startingPoint, BUFFER_SIZE);
                strcat(newStartingPoint,"/");
                strcat(newStartingPoint, dp->d_name);
                lsRecursif(newStartingPoint, option, valeur);
            }
        }
    }
    closedir(dirp);
}

verif_params_t verificationFichier(struct dirent *dp, option_t option, val_t valeur, struct stat fichier, char* path)
{
    verif_params_t verif_param;

    // Initialisation état
    if (option.ou == false)
    {
        verif_param.etat = true;
    }
    else
    {
        verif_param.etat = false;
    }
    
    // Vérification nom
    if (option.nom)
    {
        if (option.ou == false)
        {
            verif_param.etat = regexMatch(dp->d_name, valeur.nom);
        }
        else
        {
            verif_param.etat = (verif_param.etat || regexMatch(dp->d_name, valeur.nom));
        }
    }

    // Vérification taille (faire une fonction pour les options '+' et '-')
    if (option.taille)
    {
        long bytes_multiplicator = 1;

        switch(valeur.size_params.symbole_taille)
        {
            case 'k':
                bytes_multiplicator *= 1024;
                break;
            case 'M':
                bytes_multiplicator *= 1024 * 1024;
                break;
            case 'G':
                bytes_multiplicator *= 1024 * 1024 * 1024;
                break;
            default:
                break;
        }

        // Taille critique : avec un test, permet de savoir si le fichier est affiché sur la stdout ou non.
        long taille_fichier_critique = valeur.size_params.taille * bytes_multiplicator;
        //printf("Taille critique : %ld %ld %ld\n", valeur.size_params.taille, bytes_multiplicator, taille_fichier_critique);
        //printf("Taille du fichier : %d, taille critique : %d\n", fichier.st_size, taille_fichier_critique);

        if ((valeur.size_params.signe == '+' && fichier.st_size <= taille_fichier_critique) || (valeur.size_params.signe == '-' && fichier.st_size >= taille_fichier_critique) || (valeur.size_params.signe == 'o' && fichier.st_size != taille_fichier_critique))
        // Signe +, taille du fichier trop petite
        // Signe -, taille du fichier trop grande
        // --> on ne retient pas ce fichier
        {
            if (option.ou == false)
            {
                verif_param.etat = false;
            }
            else
            {
                verif_param.etat = (verif_param.etat || false);
            }
        }
        else
        {
            if (option.ou == true)
            {
                verif_param.etat = (verif_param.etat || true);
            }
        }
        /*
        else // Test : affiche la taille du fichier
        {
            printf("Taille du fichier : %ld, taille critique : %ld\n", fichier.st_size, taille_fichier_critique);
        }
        */
    }

    // Vérification date
    if (option.date)
    {
        date_params_t date_param = checkDate(fichier, valeur.date, option.color);
        if (option.ou == false)
        {
            verif_param.etat = date_param.etat;
        }
        else
        {
            verif_param.etat = (verif_param.etat || date_param.etat);
        }

        if (option.color == true)
        {
            verif_param.color = date_param.color;
        }
    }

    // Vérification mime
    if (option.mime)
    {
        //printf("Path : %s\n", path);
        if (option.ou == false)
        {
            verif_param.etat = check_mime_type(path, valeur.mime_params);
        }
        else
        {
            verif_param.etat = (verif_param.etat || check_mime_type(path, valeur.mime_params));
        }
    }

    // Vérification ctc
    if (option.ctc)
    {
        if (option.ou == false)
        {
            verif_param.etat = ctc(path, valeur.ctc);
        }
        else
        {
            verif_param.etat = (verif_param.etat || ctc(path, valeur.ctc));
        }
    }

    // Vérification perm
    if (option.perm)
    {
        //printf("valeur.perm : %s\n", valeur.perm);
        if (option.ou == false)
        {
            verif_param.etat = check_perms(fichier, valeur.perm);
        }
        else
        {
            verif_param.etat = (verif_param.etat || check_perms(fichier, valeur.perm));
        }
    }

    return verif_param;
}