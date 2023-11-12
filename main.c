#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h> 

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h> 
#endif

#define MAX_LEN 100

struct player {
    char *name;
    char *type; // human or bot
};

char** readSpells(char* fileName, int* len) {
    FILE *file = fopen(fileName, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", len);
    char buf[MAX_LEN];
    fgets(buf, MAX_LEN, file);  

    char** text = (char**)malloc(sizeof(char*) * *len);
    if (!text) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *len; i++) {
        text[i] = (char*)malloc(sizeof(char) * MAX_LEN);
        if (!text[i]) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        if (fgets(text[i], MAX_LEN, file) == NULL) {
            perror("Error reading file");
            exit(EXIT_FAILURE);
        }
        text[i][strlen(text[i])-1] = '\0'; 
    }

    fclose(file);
    return text;
}

void displaySpells ( char** spells, int spells_len ) {
  printf("------------------------\n");
  printf("|%24s", "|\n");
  printf("%21s", "AVAILABLE SPELLS\n");
  printf("|%24s", "|\n");
  printf("------------------------\n");
  for (int i = 0; i < spells_len; i++) {
    if (i % 5 == 0 && i != 0) {
      printf("\n");
    }
    printf("%d| %-*s\t", i+1, MAX_LEN+2, spells[i]);
  }
}

int coin () {
  int r = rand() % 2;
  return r;
}

bool condition_isInList ( char** spells, int spells_len, char* spell ) {
  for (int i = 0; i < spells_len; i++) {
        if (strcmp(spells[i], spell) == 0) {
            return true;
        }
    }
    return false;
}

bool condition_notAlreadyCast ( char** usedSpells, int usedSpells_len, char* spell ) {
    for (int i = 0; i < usedSpells_len; i++) {
        if (strcmp(usedSpells[i], spell) == 0) {
            return false;
        }
    }
    return true;
}

bool condition_charMatch ( char* spell, char* spell_previous ) {
   if (spell[0] == spell_previous[strlen(spell_previous) - 1]) {
        return true;
    }
    return false;
}

bool condition_validSpellExists ( char** spells, int spells_len, char* spell_previous ) {
    for (int i = 0; i < spells_len; i++) {
        if (condition_charMatch(spells[i], spell_previous)) {
            return true;
        }
    }
    return false;
}

char* bot_easy (char** spells, int spells_len, char** usedSpells, int usedSpells_len, char* spell_previous) {

    if (strlen(spell_previous) == 0) {
        int randomIndex = rand() % spells_len;
        return spells[randomIndex];
    }
    
    for (int i = 0; i < spells_len; i++) {
        if (condition_notAlreadyCast(usedSpells, usedSpells_len, spells[i]) &&
            condition_charMatch(spells[i], spell_previous)) {
            return spells[i];
        }
    }

    if (spells_len > 0) {
        int randomIndex = rand() % spells_len;
        return spells[randomIndex];
    }
    
    return "";
}

char* bot_medium (char** spells, int spells_len, char** usedSpells, int usedSpells_len, char* spell_previous) {
    int minResponses = INT_MAX;
    char* chosenSpell = NULL;

    for (int i = 0; i < spells_len; i++) {
        if (condition_notAlreadyCast(usedSpells, usedSpells_len, spells[i]) &&
            (strlen(spell_previous) == 0 || condition_charMatch(spells[i], spell_previous))) {

            int responses = 0;
            for (int j = 0; j < spells_len; j++) {

                if (i != j && condition_notAlreadyCast(usedSpells, usedSpells_len, spells[j]) && 
                    condition_charMatch(spells[j], spells[i])) {
                    responses++;
                }
            }

            if (responses == 0) {
                return spells[i];
            }

            if (responses < minResponses) {
                minResponses = responses;
                chosenSpell = spells[i];
            }
        }
    }

    if (chosenSpell == NULL) {
        int randomIndex = rand() % spells_len;
        return spells[randomIndex];
    }

    return chosenSpell;
}

char* bot_hard (char** spells, int spells_len, char** usedSpells, int usedSpells_len, char* spell_previous) {
    int minResponses = INT_MAX;
    char* chosenSpell = NULL;

    for (int i = 0; i < spells_len; i++) {
        if (condition_notAlreadyCast(usedSpells, usedSpells_len, spells[i]) &&
            (strlen(spell_previous) == 0 || condition_charMatch(spells[i], spell_previous))) {

            int responses = 0;
            int minOpponentResponses = INT_MAX;

            for (int j = 0; j < spells_len; j++) {
                if (i != j && condition_notAlreadyCast(usedSpells, usedSpells_len, spells[j]) &&
                    condition_charMatch(spells[j], spells[i])) {
                    responses++;
                    int opponentResponses = 0;
                    for (int k = 0; k < spells_len; k++) {
                        if (k != j && condition_notAlreadyCast(usedSpells, usedSpells_len, spells[k]) &&
                            condition_charMatch(spells[k], spells[j])) {
                            opponentResponses++;
                        }
                    }
                    if (opponentResponses < minOpponentResponses) {
                        minOpponentResponses = opponentResponses;
                    }
                }
            }

            if (responses == 0) {
                return spells[i];
            }

            if (minOpponentResponses < minResponses) {
                minResponses = minOpponentResponses;
                chosenSpell = spells[i];
            }
        }
    }

    if (chosenSpell != NULL) {
        return chosenSpell;
    }

    while (spells_len > 0) {
        int randomIndex = rand() % spells_len;
        if (condition_notAlreadyCast(usedSpells, usedSpells_len, spells[randomIndex]) &&
            (strlen(spell_previous) == 0 || condition_charMatch(spells[randomIndex], spell_previous))) {
            return spells[randomIndex];
        }
    }

    return "";
}

char* bot (char* difficulty, char** spells, int spells_len, char** usedSpells, int usedSpells_len, char* spell_previous) {
    if (strcmp(difficulty, "easy") == 0) {
        return bot_easy(spells, spells_len, usedSpells, usedSpells_len, spell_previous);
    }
    if (strcmp(difficulty, "medium") == 0) {
        return bot_medium(spells, spells_len, usedSpells, usedSpells_len, spell_previous);
    }
    if (strcmp(difficulty, "hard") == 0) {
        return bot_hard(spells, spells_len, usedSpells, usedSpells_len, spell_previous);
    }

    return NULL;
}


void game (char** spells, int spells_len, struct player* players, int startingPlayer, char* difficulty) {
    char** usedSpells = (char**)malloc(sizeof(char*) * spells_len);
    int usedSpells_len = 0;
    char spell_previous[MAX_LEN] = "";  // No previous spell at the start
    int currentPlayer = startingPlayer;

    while (true) {
        printf("\n%s's turn.\n", players[currentPlayer].name);

        // Input spell
        char spell[MAX_LEN];
        if (strcmp(players[currentPlayer].type, "human") == 0) {
        printf("Enter a spell: ");
        scanf("%s", spell);
        }
        else if (strcmp(players[currentPlayer].type, "bot") == 0) {
            char* choice = bot(difficulty, spells, spells_len, usedSpells, usedSpells_len, spell_previous);
            strcpy(spell, choice);
            printf("%s chose %s.\n", players[currentPlayer].name, choice);
        }

        // Check conditions
        if (!condition_isInList(spells, spells_len, spell) ||
            !condition_notAlreadyCast(usedSpells, usedSpells_len, spell) ||
            (strlen(spell_previous) > 0 && !condition_charMatch(spell, spell_previous))) {

            printf("Invalid or repeated spell. %s wins!\n", players[1 - currentPlayer].name);
            break;
        }

        // Check if a valid spell exists for the next round
        if (!condition_validSpellExists(spells, spells_len, spell)) {
            printf("No valid spell exists for the next round. %s wins!\n", players[currentPlayer].name);
            break;
        }

        // Remove spell from spells array and add to usedSpells array
        for (int i = 0; i < spells_len; i++) {
            if (strcmp(spells[i], spell) == 0) {
                usedSpells[usedSpells_len] = (char*)malloc(sizeof(char) * MAX_LEN);
                strcpy(usedSpells[usedSpells_len], spell);
                usedSpells_len++;

            
                for (int j = i; j < spells_len - 1; j++) {
                    strcpy(spells[j], spells[j + 1]);
                }

                spells_len--;  
                break;
            }
        }

        // Update spell_previous and switch player
        strcpy(spell_previous, spell);
        currentPlayer = 1 - currentPlayer;
    }

    
    for (int i = 0; i < usedSpells_len; i++) {
        free(usedSpells[i]);
    }
    free(usedSpells);
}

int main() {
    int spells_len;
    char** spells = readSpells("spells.txt", &spells_len);

    struct player playerOne, playerTwo;

    playerOne.name = (char*)malloc(sizeof(char) * 20);
    playerOne.type = (char*)malloc(sizeof(char) * 5);
    printf("Enter your name: ");
    scanf("%19s", playerOne.name);
    fflush(stdin); 
    playerOne.type = "human";

    playerTwo.name = (char*)malloc(sizeof(char) * 20);
    playerTwo.type = (char*)malloc(sizeof(char) * 5);
    playerTwo.name = "BOT";
    playerTwo.type = "bot";

    struct player players[2] = {playerOne, playerTwo};

    char* difficulty = (char*)malloc(sizeof(char) * 6);
    printf("Choose bot difficulty (easy/medium/hard): ");
    scanf("%s", difficulty);

    displaySpells(spells, spells_len);

    srand(time(NULL));
    printf("\n\n---------------\n");
    printf("Tossing coin...\n");
    printf("---------------\n");
    #ifdef _WIN32
    Sleep(2000);
    #else
    sleep(2);
    #endif

    int startingPlayer = coin();
    printf("%s BEGINS.\n", players[startingPlayer].name);
    game(spells, spells_len, players, startingPlayer, difficulty);

    for (int i = 0; i < spells_len; i++) {
        free(spells[i]);
    }
    free(spells);

    free(playerOne.name);
    free(playerOne.type);
    free(playerTwo.name);
    free(playerTwo.type);
    free(difficulty);

    return 0;
}
