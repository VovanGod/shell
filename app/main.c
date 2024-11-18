#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>

void save_history(char*command) {
  FILE *file = fopen("history-txt", "a");
  if(file == NULL){
    perror("Failed to open");
    return;
  }
  fprintf(file, "%s\n", command);
  fclose(file);
}

int isNotEmpty(char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        // Прохожу по всей строке и проверяю каждый символ на то, является ли он пробелом с помощью функции isspace()
        if (!isspace(str[i])) {
            return 1;
        }
    }
    return 0;
}

void executeCommand(const char* command) { 
    pid_t pid = fork();
    if (pid == 0) {  // Дочерний процесс
        execlp(command, command, (char*)NULL);
        perror("execlp failed");
        exit(1);
    } else if (pid < 0) {
        perror("fork failed");
    } else {
        wait(NULL); // Ожидание завершения дочернего процесса
    }

    // gcc main.c -o main - компиляция кода
    // ./main - запуск бинарника
 }


 void handle_sighup() {
      printf("Configuration reloaded\n");
      fflush(stdout); // Необходимо для немедленного вывода на консоль
  }

int main() {
  char input[300];
  
  while(1) {
    save_history(input);
    printf("Enter string> ");
    // Этот фрагмент кода предназначен для безопасного считывания строк из стандартного ввода. Он проверяет на ошибку чтения (fgets вернул NULL) и, если ошибка произошла, корректно завершает работу (или выходит из цикла) с сообщением "Exit". Использование sizeof(input) защищает программу от переполнения буфера, что очень важно с точки зрения безопасности.
    if(fgets(input, sizeof(input), stdin) == NULL) {
      printf("Exit\n"); 
      break;
    }

    // strcspn - находит количество символов в начале первой строки, которые не присутствуют во второй
    input[strcspn(input, "\n")] = 0;

    if(strcmp(input, "exit") == 0 || strcmp(input, "\\q") == 0) {
      printf("You have entered an output\n");
      break;
    }

    // strncmp - сравнивает первые n символов двух строк
    if (strncmp(input, "echo ", 5) == 0) {
      printf("%s\n", input+5); // %s - спецификатор формата для вывода строки, т.е принт будет искать указатель на строку и выведет ее
      printf("\n");
      continue;
    }

    // strcmp - сравнивает 2 строки, если равны то возвращает 0
    if(!isNotEmpty(input)){
      printf("Enter correct command dibil\n");
      continue;
    }

    if(strncmp(input, "\\e ", 3) == 0) {
      char* varName = input+3;
      varName[strcmp(varName, "\n")] = 0;
      // getenv() - функция для получения переменной окружения
      char* varValue = getenv(varName);
      if(varValue == NULL) {
        printf("Var not found: %s\n", varName);
      }
      else {
        printf("%s\n", varValue);
      }
      printf("\n");
      continue;
    }

    if (strncmp(input, "run ", 4) == 0) {
        char* comRun = input + 4;
        comRun[strcspn(comRun, "\n")] = 0;
        executeCommand(comRun);
        printf("\n");
        continue;
    } 
    
    if(strncmp(input, "SIGHUP", 7) == 0) {
      // в одном терминале собрали через gcc main.c -o main и запустили ./main, ввели SIGHUP
      // в другом терминале ps aux | grep main и нашли пид процесса, потом убили его с помощью kill -2 pid
      signal(SIGINT, handle_sighup);
      while(1) {
        pause();
        break;
      }
      printf("\n\n");
      continue;
    }

    printf("Your string> %s\n\n", input);
  }


  return 0;
}

