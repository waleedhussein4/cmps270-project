#include <stdio.h>
#include <string.h>

#define MAX_LEN 50

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
    if (fgets(text[line], 50, file) != NULL) {
      line++;
    }
  }

  fclose(file);

  // loop through text lines and remove new lines
  for (int i = 0; i < len[0]; i++) {
    text[i][strlen(text[i])-1] = '\0';
  }

  return text;
}

int main () {
  int* spells_len;
  char** spells = read("spells.txt", &spells_len);

  for (int i = 0; i < spells_len; i++) {
    printf("%d %s\n", i, spells[i]);
  }

  return 0;
}