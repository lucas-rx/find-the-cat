#ifndef __MAIN__
#define __MAIN__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> // perror
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <regex.h>
#include <time.h>

#define NB_OPTIONS 12
#define NB_MIME_TYPES 5
#define BUFFER_SIZE 2000
#define BUFFER_FILE 100000

/**
 * Structure permettant de connaître si telle option est utilisée dans la commande ftc ou non.
 * @param basique booléen : recherche sans options ;
 * @param nom booléen : recherche sur le nom;
 * @param taille booléen : recherche sur la taille ;
 * @param date booléen : recherche sur la date ;
 * @param mime booléen : recherche sur l'extension ;
 * @param ctc booléen : recherche sur le contenu ;
 * @param dir booléen : recherche sur les dossiers ;
 * @param perm booléen : recherche sur les permissions ;
 * @param color booléen : (optionnel) affichage en couleurs ;
 * @param ou booléen : recherche avec plusieures options.
 */
typedef struct option_t option_t;
struct option_t
{
    bool basique; // Recherche sans options
    bool nom;
    bool taille;
    bool date;
    bool mime;
    bool ctc;
    bool dir;
    bool perm;
    bool color;
    bool ou;
};

/**
 * Structure stockant les informations relatives au paramètre de l'option -size.
 * @param signe Un caractère : le signe de la commande : +, - ou o (signe vide) ;
 * @param taille Un entier long : la taille ;
 * @param symbole_taille Un caractère : c, k, M ou G.
 */
typedef struct size_params_t size_params_t;
struct size_params_t
{
    char signe;
    long taille;
    char symbole_taille;
};

/**
 * Structure stockant les informations relatives au paramètre de l'option -mime.
 * @param type Une chaîne de caractères : le type MIME d'un fichier. Exemple : text/html --> text.
 * @param subtype Une chaîne de caractères : le sous-type MIME d'un fichier. Exemple : text/html --> html.
 * @param has_subtype Un booléen valant true si un sous-type est stocké, il vaut false sinon.
 */
typedef struct mime_params_t mime_params_t;
struct mime_params_t
{
    char type[50];
    char subtype[50];
    bool has_subtype;
};

/**
 * Répertorie les paramètres de toutes les options de la commande exécutée.
 * @param nom Une chaîne de caractères : le nom d'un fichier;
 * @param size_params Une structure : informations concernant la taille;
 * @param date Une chaîne de caractères : informations concernant la date;
 * @param mime_params Une structure : informations concernant le type d'extension;
 * @param ctc Une chaîne de caractères : le contenu du fichier;
 * @param perm Une chaîne de caractères : les permissions du fichier.
 */
typedef struct val_t val_t;
struct val_t
{
    char nom[1000];
    size_params_t size_params;
    char date[100];
    mime_params_t mime_params;
    char ctc[1000];
    char perm[4];
};

/**
 * Structure de données spécifique à l'option -date.
 * @param color Un caractère : désigne la couleur;
 * @param etat Un booléen : désigne le résultat de la vérification.
 */
typedef struct date_params_t date_params_t;
struct date_params_t
{
    char color;
    bool etat;
};

/**
 * Structure de données spécifique à la fonction de vérification des fichiers.
 * @param color Un caractère : désigne la couleur;
 * @param etat Un booléen : désigne le résultat de la vérification.
 */
typedef struct verif_params_t verif_params_t;
struct verif_params_t
{
    char color;
    bool etat;
};

/**
 * Vérifie si une chaîne de caractères est une option valide.
 * @param option Une chaîne de caractères.
 * @return true si cette chaîne de caractères est une option valide, false sinon.
 */
bool estUneOption(char* option);

/**
 * Vide un buffer.
 * @param buffer Un buffer.
 */
void clear_buffer(char* buffer);

/**
 * Affiche dans le terminal les chemins de tous les dossiers et fichiers à partir du dossier de départ startingPoint.
 * @param startingPoint Le dossier à partir duquel commence le parcours de l'arborescence des fichiers ;
 * @param option Une structure comportant des champs booléens, permettant de savoir si tel flag est passé en argument de ftc ;
 * @param valeur Une structure contenant les paramètres des options utilisées.
 */
void lsRecursif(char* startingPoint, option_t option, val_t valeur);

/**
 * Permet de savoir si un fichier respecte les critères passés en paramètres des options de la commande ftc.
 * @param dp Un dossier ;
 * @param option Une structure comportant des champs booléens, permettant de savoir si tel flag est passé en argument de ftc ;
 * @param valeur Une structure contenant les paramètres des options utilisées ;
 * @param fichier Un fichier ;
 * @param path Un chemin relatif vers le fichier passé en paramètre ci-dessus.
 * @return Une structure verif_params_t.
 */
verif_params_t verificationFichier(struct dirent *dp, option_t option, val_t valeur, struct stat fichier, char* path);

/**
 * Parse le paramètre de l'option -size.
 * @param size_params Une structure contenant un champ pour le signe, un champ pour la taille et un champ pour le symbole de la taille du paramètre ;
 * @param parameter Le paramètre de l'option -size.
 */
void parseSizeParameter(size_params_t* size_params, char* parameter, bool is_flag_color);

/**
 * Check si une chaîne de caractère name matche avec un token regex.
 * @param name Une chaîne de caractères : désigne le nom du fichier parcouru ;
 * @param token Une chaîne de caractères : désigne le nom (ou expression regex) qu'on cherche.
 * @return Un booléen.
 */
bool regexMatch(char* name, char* token);

/**
 * Check si un fichier texte contient une chaîne de caractère.
 * @param name Une chaîne de caractères : désigne le nom du fichier parcouru ;
 * @param token Une chaîne de caractères : désigne le nom (ou expression regex) qu'on cherche.
 * @return Un booléen.
 */
bool ctc(char* pathOfFile, char* token);

/**
 * Check si un fichier correspond a une date donnée.
 * @param fichier Une structure de type stat : contient les infos du fichier parcouru ;
 * @param date Une chaîne de caractères : désigne la date qu'on cherche.
 * @param is_flag_color Un booléen : true si l'erreur sera affichée en couleur, false sinon.
 * @return Une structure de type date_params_t.
 */
date_params_t checkDate(struct stat fichier, char* date, bool is_flag_color);

/**
 * Vérifie si les options passées en paramètre de la commande ftc existent.
 * @param length_tab argc ;
 * @param tab argv.
 * @param is_flag_color Un booléen : true si l'erreur sera affichée en couleur, false sinon.
 */
void options_legales(int length_tab, char* tab[], bool is_flag_color);

/**
 * Vérifie si la chaîne de caractère entrée est équivalente à un nombre entier.
 * @param token Une chaîne de caractère : contient un nombre.
 * @return Un booléen.
 */
bool estNombreEntier(char* token);

/**
 * Ajoute du style (couleur et gras) à une chaîne de caractères.
 * @param token Une chaîne de caractères ;
 * @param color Un caractère : la couleur ;
 * @param bold Un booléen : true si la chaîne est en gras, false sinon.
 */
void colorPrintf(char* token, char color, bool bold);

/**
 * Retourne la chaîne de caractères symbolisant le début d'écriture avec un certaine couleur, et si elle est en gras ou non.
 * @param color Un caractère : la couleur ;
 * @param bold Un booléen : true si la chaîne est en gras, false sinon.
 * @return La chaîne de caractères en question (décrite plus haut).
 */
char* begin_color(char color, bool bold);

/**
 * Retourne la chaîne de caractères symbolisant la fin d'écriture avec un certaine couleur, et si elle est en gras ou non.
 * @return La chaîne de caractères en question (décrite plus haut).
 */
char* end_color();

/**
 * Affiche une chaîne de caractères en couleur sur stderr (la sortie d'erreur standard).
 * @param token La chaîne de caractères à afficher ;
 * @param color Un caractère : la couleur du texte ;
 * @param bold Un booléen : true pour afficher la chaîne de caractères en gars, false sinon ;
 * @param is_flag_color Un booléen : true si l'erreur sera affichée en couleur, false sinon.
 */
void print_error(char* token, char color, bool bold, bool is_flag_color);

/**
 * Affiche un fichier en couleur. Cet affichage est utilisé par défaut.
 * @param token La chaîne de caractères à afficher ;
 * @param is_flag_color Un booléen : true si l'erreur sera affichée en couleur, false sinon ;
 * @param mime_type Une chaîne de caractères : le type MIME du fichier, mais pas son sous-type (exemple : text/html ou text --> text).
 */
void print_file(char* token, bool is_flag_color, char* mime_type);

/**
 * Affiche un fichier en couleur. Cet affichage est utilisé si -date est passé en option de la commande.
 * @param token La chaîne de caractères à afficher ;
 * @param is_flag_color Un booléen : true si l'erreur sera affichée en couleur, false sinon ;
 * @param color Un caractère : la couleur du texte.
 */
void print_file_date(char* token, bool is_flag_color, char color);

/**
 * Retourne le type MIME d'un fichier, mais pas son sous-type (exemple : text/html ou text --> text).
 * @param path Un chemin relatif vers le fichier passé en paramètre ci-dessus.
 * @return Une chaîne de caractères : le type MIME du fichier.
 */
char* get_mime_type(char* path);

/**
 * Parse le paramètre de l'option -mime.
 * @param mime_params Une structure contenant le type, le sous-type MIME du fichier et un entier indiquant si la recherche s'effectue sur le type, 
 * ou bien sur le type et le sous-type. Exemple : text/html --> text est appelé type, html est appelé sous-type ;
 * @param parameter Le paramètre de l'option -mime ;
 * @param is_flag_color Un booléen : true si l'erreur sera affichée en couleur, false sinon.
 */
void parse_mime_parameter(mime_params_t* mime_params, char* parameter, bool is_flag_color);

/**
 * Vérifie si le type MIME d'un fichier correspond au type MIME recherché.
 * @param path Le chemin relatif du fichier depuis startingPoint ;
 * @param mime_params Une structure contenant le type, le sous-type MIME du fichier et un booléen indiquant si la recherche s'effectue sur le type, 
 * ou bien sur le type et le sous-type. Exemple : text/html --> text est appelé type, html est appelé sous-type ;
 * @return true si le type MIME du fichier correspond au type MIME recherché, false sinon.
 */
bool check_mime_type(char* path, mime_params_t mime_params);

/**
 * Vérifie si les permissions d'un fichier correspondent aux permissions recherchées.
 * @param fichier Un fichier ;
 * @param wanted_perm Une chaîne de caractères : les permissions recherchées au format octal.
 * @return true si les permissions du fichier correspond aux permissions recherchées, false sinon.
 */
bool check_perms(struct stat fichier, char wanted_perm[]);

#endif
