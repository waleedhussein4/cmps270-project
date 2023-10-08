#include <stdio.h>

int main () {
  String p1input1;
  String p1input2;
  String p2input1;
  String p2input2;
  printf("Player 1 chooses: %s", p1input1);
  scanf("%s" , p1input1)
  printf("Player 1 chooses: %s", p2input1);
  scanf("%s" , p2input1)
  while(true){
    printf("Player 1 chooses: %s",p1input2);
    if(p1input2.equalsIgnorecase(p1input1)){
      printf("Player 2 wins (Player 1 repeated a previously cast spell)");
      break;
    else if(p1input2.charAt(0) != p1input1.charAt(length-1)){
      printf("Player 2 wins (no more spells in the list that satisfy the character matching condition)");
      break;
    printf("Player 2 chooses: %s",p2input2);
    if(p2input2.equalsIgnorecase(p2input1)){
      printf("Player 1 wins (Player 2 repeated a previously cast spell)");
      break;
    else if(p2input2.charAt(0) != p2input1.charAt(length-1)){
      printf("Player 1 wins (no more spells in the list that satisfy the character matching condition)");
      break;
}
