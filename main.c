#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_LEN 20

char** read( char* fileName, int* len ) {
  FILE *file;
  file = fopen(fileName, "r");

  fscanf(file, "%d", len);
  char buf[MAX_LEN];
  fgets(buf, MAX_LEN, file);
  
  char** text = (char**)malloc(sizeof(char*) * *len);
  for (int i = 0; i < *len; i++) {
    text[i] = (char*)malloc(sizeof(char) * MAX_LEN);
  }

  int line = 0;
  while ( !feof(file) && !ferror(file) ) { //  keep reading until the end of the file
    if (fgets(text[line], MAX_LEN, file) != NULL) {
      line++;
    }
  }

  fclose(file);

  //  loop through text lines and remove linebreaks
  for (int i = 0; i < len[0]; i++) {
    text[i][strlen(text[i])-1] = '\0';
  }

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

void game(char** spells, int spells_len, struct player* players, int startingPlayer) {
    char** usedSpells = (char**)malloc(sizeof(char*) * spells_len);
    int usedSpells_len = 0;
    char spell_previous[MAX_LEN] = "";  // No previous spell at the start
    int currentPlayer = startingPlayer;

    while (true) {
        printf("\n%s's turn.\n", players[currentPlayer].name);

        // Input spell
        char spell[MAX_LEN];
        printf("Enter a spell: ");
        scanf("%s", spell);

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

                // Shift remaining spells left to overwrite used spell
                for (int j = i; j < spells_len - 1; j++) {
                    strcpy(spells[j], spells[j + 1]);
                }

                spells_len--;  // Decrease spell count
                break;
            }
        }

        // Update spell_previous and switch player
        strcpy(spell_previous, spell);
        currentPlayer = 1 - currentPlayer;
    }

    // Free allocated memory
    for (int i = 0; i < usedSpells_len; i++) {
        free(usedSpells[i]);
    }
    free(usedSpells);
}

int main () {
  int* spells_len;
  char** spells = read("spells.txt", &spells_len);

  struct player {
    char * name; //  first name
    char * type; //  human or bot
  }playerOne, playerTwo;

  printf("Player one, enter your name: ");
  scanf("%s", &playerOne.name);
  playerOne.type = "human";

  printf("Player two, enter your name: ");
  scanf("%s", &playerTwo.name);
  playerTwo.type = "human";

  struct player players[2] = {playerOne, playerTwo};

  displaySpells(spells, spells_len);

  srand(time(NULL));
  printf("\n\n---------------\n");
  printf("Tossing coin...\n");
  printf("---------------\n");
  sleep(2);
  int startingPlayer = coin();
  printf("%s BEGINS.\n", &(players[startingPlayer]).name);
  
  return 0;
}
