#include "utils.h"
#include <stdio.h>
#include <string.h>
#include "lang.h"

/* lecture simple d'un entier */
int read_int(void)
{
    int n;

    if (scanf("%d", &n) == 1)
        return n;

    /* vide le buffer en cas d'erreur */
    while (getchar() != '\n');
    return -1;
}

// verifie le login admin a partir d'un fichier
int verifier_admin(void)
{
    FILE *f;
    char login_fichier[50];
    char pass_fichier[50];
    char login_user[50];
    char pass_user[50];

    f = fopen("admin.conf", "r");
    if (f == NULL)
    {
        printf("Erreur fichier admin\n");
        return 0;
    }

    fgets(login_fichier, 50, f);
    fgets(pass_fichier, 50, f);
    fclose(f);

    login_fichier[strcspn(login_fichier, "\n")] = 0;
    pass_fichier[strcspn(pass_fichier, "\n")] = 0;

    printf("%s", lang("ADMIN_LOGIN"));
    scanf("%s", login_user);

    printf("%s", lang("ADMIN_PASS"));
    scanf("%s", pass_user);

    if (strcmp(login_user, login_fichier) == 0 &&
        strcmp(pass_user, pass_fichier) == 0)
    {
        printf("%s\n", lang("ADMIN_OK"));
        return 1;
    }

    printf("%s\n", lang("ADMIN_KO"));
    return 0;
}

