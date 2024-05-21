#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // fill later
#include <string.h>
#include<sys/wait.h> // To enable using the wait() system call.
#include <sys/types.h>

#define MAX 100

int main (int argc, char* argv[]) {

    char user_input[MAX];
    char *path = getenv("PATH"); // To get back the enviorment path.
    char *token_path = NULL;
    char *token_user_input = NULL;
    char *user_input_tokenized_arr[MAX];
    char final_path[MAX] = "";
    int user_input_length = 0;
    int pid;
    int stat;
    int i = 0;
    while (WEXITSTATUS(stat) != 7) {
        i = 0;
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            // Prompt
            printf("✌ ");

            // On success, the function returns the same str parameter.
            // If the End-of-File is encountered and no characters have been read, 
            // the contents of str remain unchanged and a null pointer is returned.
            if (fgets(user_input, MAX, stdin) != NULL) {
                user_input_length = strlen(user_input);
                user_input[user_input_length - 1] = '\0';
                if (strcmp(user_input, "Leave") == 0) {
                    exit(7); // stat = 2 * 256 = 512
                }
                else {
                    // Tokenize user's input and copy every token to user_input_tokenized_arr.
                    // Add "NULL" to the last element. (Execv requires this)
                    token_user_input = strtok(user_input, " \t");
                    while (token_user_input != NULL) {
                        user_input_tokenized_arr[i++] = token_user_input;
                        token_user_input = strtok(NULL, " \t");
                    }
                    user_input_tokenized_arr[i] = NULL;

                    token_path = strtok(path, ":");
                    while (token_path != NULL) {
                        strcpy(final_path, token_path);
                        strcat(final_path, "/");
                        strcat(final_path, user_input_tokenized_arr[0]);
                        execv(final_path, user_input_tokenized_arr);
                        token_path = strtok(NULL, ":");
                    }

                    printf("ERROR: Command Not Found.\n");
                    exit(5); // stat != 0, 5 * 256 = 1280
                }
            }
       
        } else {
            wait(&stat);
            
        }
    }
    return 0;
}
