#include <stdio.h>

#define MAX_LINES 100
#define MAX_LEN 50

char** read(char* fileName) {
  FILE *file;
  file = fopen(fileName, "r");
  
  char** text = (char**)malloc(sizeof(char*) * MAX_LINES);
  for (int i = 0; i < MAX_LINES; i++) {
    text[i] = (char*)malloc(sizeof(char) * MAX_LEN);
  }

  int line = 0;
  while ( !feof(file) && !ferror(file) ) { // keep reading until the end of the file
    if (fgets(text[line], 50, file) != NULL) {
      line++;
    }
  }

  fclose(file);

  return text;
}

int main () {
  char** spells = read("spells.txt");

  return 0;
}