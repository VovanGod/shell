#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void save_history(char*command) {
  FILE *file = fopen("history-txt", "a");
  if(file == NULL){
    perror("Failed to open");
    return;
  }
  fprintf(file, "%s\n", command);
  fclose(file);
}

int main() {
  char input[200];
  printf("Enter string> ");
  fgets(input, sizeof(input), stdin);
  input[strcspn(input, "\n")] == 0;

  printf("Your string> %s\n", input);
  
  while(1) {
    save_history(input);
    printf("Enter string> ");
    if(fgets(input, sizeof(input), stdin) == NULL) {
      printf("Exit\n");
      break;
    }

    input[strcspn(input, "\n")] = 0;

    if(strcmp(input, "exit") == 0 || strcmp(input, "\\q") == 0) {
      printf("exit\n");
      break;
    }

    if (strncmp(input, "echo ", 5) == 0) {
      printf("%s\n", input+5);
      continue;
    }

    if(strcmp(input, "") == 0){
      printf("enter correct command dibil\n");
      continue;
    }

    printf("Your string> %s\n", input);

    if(strncmp(input, "\\e ", 3) == 0) {
      char* varName = input+3;
      varName[strcmp(varName, "\n")] = 0;
      char* varValue = getenv(varName);
      if(varValue == NULL) {
        printf("Var not found: %s\n", varName);
      }
      else {
        printf("%s\n", varValue);
      }
    }
  }


  return 0;
}

