#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 20

char** read(char* fileName, int* len) {
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
  while ( !feof(file) && !ferror(file) ) { // keep reading until the end of the file
    if (fgets(text[line], MAX_LEN, file) != NULL) {
      line++;
    }
  }

  fclose(file);

  // loop through text lines and remove linebreaks
  for (int i = 0; i < len[0]; i++) {
    text[i][strlen(text[i])-1] = '\0';
  }

  return text;
}

int coin () {
  int r = rand() % 2;
  return r;
}

int main () {
  int* spells_len;
  char** spells = read("spells.txt", &spells_len);

  struct player {
    char * name; // first name
    char * type; // human or bot
  }playerOne, playerTwo;

  printf("Player one, enter your name: ");
  scanf("%s", &playerOne.name);
  playerOne.type = "human";

  printf("Player two, enter your name: ");
  scanf("%s", &playerTwo.name);
  playerTwo.type = "human";

  struct player players[2] = {playerOne, playerTwo};

  // display table of spells
  printf("--------------------\n");
  printf("|%20s", "|\n");
  printf("%14s", "SPELLS\n");
  printf("|%20s", "|\n");
  printf("--------------------\n");
  for (int i = 0; i < spells_len; i++) {
    if (i % 5 == 0 && i != 0) {
      printf("\n");
    }
    printf("%d| %-*s\t", i+1, MAX_LEN+2, spells[i]);
  }

  srand(time(NULL));
  printf("\n\n---------------\n");
  printf("Tossing coin...\n");
  printf("---------------\n");
  sleep(2);
  int startingPlayer = coin();
  printf("%s BEGINS.\n", &(players[startingPlayer]).name);
  
  return 0;
}