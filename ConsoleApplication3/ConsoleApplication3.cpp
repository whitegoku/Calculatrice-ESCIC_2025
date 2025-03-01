// Calculatrice_2025.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Fonction pour vérifier si un caractère est un opérateur
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Fonction pour donner la priorité aux opérateurs
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Fonction pour effectuer une opération
int applyOp(int a, int b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    }
    return 0;
}

// Fonction pour convertir un caractère de base 26 en entier de base 10
int charToBase26(char c) {
    if (isdigit(c)) return c - '0';
    if (isalpha(c)) return toupper(c) - 'A' + 10;
    return 0;
}

// Fonction pour convertir une chaîne en entier de base 10
int stringToBase10(const char* str) {
    int num = 0;
    while (*str) {
        num = num * 10 + charToBase26(*str);
        str++;
    }
    return num;
}

// Fonction pour convertir un nombre de base 10 en base 26
char* toBase26(int num) {
    static char result[100];
    char chars[] = "0123456789ABCDEFGHIJKLMNOP";
    int index = 0;

    do {
        result[index++] = chars[num % 26];
        num /= 26;
    } while (num > 0);

    result[index] = '\0';

    // Inverser la chaîne de caractères
    for (int i = 0; i < index / 2; i++) {
        char temp = result[i];
        result[i] = result[index - 1 - i];
        result[index - 1 - i] = temp;
    }

    return result;
}

// Fonction pour évaluer une expression en base 26
int evaluateBase26(const char* tokens) {
    int i;
    int values[100];
    char ops[100];
    int valIndex = -1;
    int opIndex = -1;

    for (i = 0; i < strlen(tokens); i++) {
        if (tokens[i] == ' ') continue;
        if (isalnum(tokens[i])) {
            char temp[100];
            int j = 0;
            while (i < strlen(tokens) && isalnum(tokens[i])) {
                temp[j++] = tokens[i++];
            }
            temp[j] = '\0';
            values[++valIndex] = stringToBase10(temp);
            i--;
        }
        else if (tokens[i] == '(') {
            ops[++opIndex] = tokens[i];
        }
        else if (tokens[i] == ')') {
            while (opIndex != -1 && ops[opIndex] != '(') {
                int val2 = values[valIndex--];
                int val1 = values[valIndex--];
                char op = ops[opIndex--];
                values[++valIndex] = applyOp(val1, val2, op);
            }
            opIndex--;  // remove '('
        }
        else if (isOperator(tokens[i])) {
            while (opIndex != -1 && precedence(ops[opIndex]) >= precedence(tokens[i])) {
                int val2 = values[valIndex--];
                int val1 = values[valIndex--];
                char op = ops[opIndex--];
                values[++valIndex] = applyOp(val1, val2, op);
            }
            ops[++opIndex] = tokens[i];
        }
    }

    while (opIndex != -1) {
        int val2 = values[valIndex--];
        int val1 = values[valIndex--];
        char op = ops[opIndex--];
        values[++valIndex] = applyOp(val1, val2, op);
    }

    return values[valIndex];
}

// Fonction pour lire un fichier texte et évaluer les expressions en base 26
void lireFichierEtCalculer(const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", nomFichier);
        return;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        // Supprimer le '\n' à la fin de l'entrée
        ligne[strcspn(ligne, "\n")] = '\0';

        int resultBase10 = evaluateBase26(ligne);
        printf("Expression : %s\n", ligne);
        printf("Résultat en base 10: %d\n", resultBase10);
        printf("Résultat en base 26: %s\n\n", toBase26(resultBase10));
    }

    fclose(fichier);
}

// Fonction principale
int main() {
    char choix[3];
    char expression[100];
    const char* nomFichier = "fichier.txt";

    while (1) {
        printf("Choisissez une option :\n");
        printf("1. Entrer une expression en base 26\n");
        printf("2. Lire les expressions d'un fichier\n");
        printf("3. Quitter\n");
        printf("Votre choix : ");
        fgets(choix, sizeof(choix), stdin);

        if (choix[0] == '1') {
            printf("Entrez l'expression en base 26 : ");
            fgets(expression, sizeof(expression), stdin);

            // Supprimer le '\n' à la fin de l'entrée
            expression[strcspn(expression, "\n")] = '\0';

            int resultBase10 = evaluateBase26(expression);
            printf("Résultat en base 10: %d\n", resultBase10);
            printf("Résultat en base 26: %s\n\n", toBase26(resultBase10));
        }
        else if (choix[0] == '2') {
            lireFichierEtCalculer(nomFichier);
        }
        else if (choix[0] == '3') {
            break;
        }
        else {
            printf("Choix invalide. Veuillez réessayer.\n\n");
        }
    }

    return 0;
}
