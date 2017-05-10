// Patrick Lyons
// COE 0449 Project 4
// 04/09/2017

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//Declares get_args to find the number of args passed by user
int get_args(char*);

char *delims = " \t\n()<>|&;";

int main(){
  char** arg_array;
  char* arg;
  int num_args = 0;

  //String able to hold 1024 characters
  char user_input[1024];
  char token[1024];

  int check_fork = 0;
  int fork_status;
  int count = 0;

  //Until user exits
  while(1){

    printf("~$ ");
    fgets(user_input, 1024, stdin);

    num_args = get_args(user_input);
    arg_array = malloc(sizeof(char*) * (num_args + 1));

    //Copies the full user input into token string
    strcpy(token, user_input);

    //tokenizes a string separating the delimiters
    arg = strtok(token, delims);
    count = 0;

    while(arg != NULL){
      arg_array[count] = (char*) malloc(sizeof(char) * strlen(arg));
      strcpy(arg_array[count], arg);
      arg = strtok(NULL, delims);
      count++;
    }

    arg_array[num_args] = NULL;

    if(arg_array[0] == NULL)
      continue;

    //if client wants to exit, exit
    if(strcmp(arg_array[0], "exit") == 0)
      exit(0);

    //Changes user directory
    else if(strcmp(arg_array[0], "cd") == 0){
      //Change Directory unsuccessful
      if(chdir(arg_array[1]) == -1){
        if(arg_array[1] == NULL){
          printf("No directory specified\n");
        }
        else{
          printf("Directory does not exist\n");
        }
      }
    }

    // ls, pico, pwd, etc. with args
    else{

      check_fork = fork();

      //Could not create child process. Waiting
      if(check_fork != 0)
        wait(&fork_status);

      //Correctly created child process
      else{

        //Write to file (ls > foo)
        if(strstr(user_input, ">") != NULL){
          freopen(arg_array[num_args - 1], "w", stdout);
          free(arg_array[num_args - 1]);
          arg_array[num_args - 1] = NULL;
        }

        //Read from file (more < foo)
        else if(strstr(user_input, "<") != NULL){
          freopen(arg_array[num_args - 1], "r", stdin);
          free(arg_array[num_args - 1]);
          arg_array[num_args - 1] = NULL;
        }

        //Executes command
        execvp(arg_array[0], arg_array);
        exit(0);
      }//After child process executes

    } //Done executing other commands

  }//End of forever loop

  return 0;

}//End of main

//Gets the number of user passed arguments
int get_args(char* user_input){

  char arguments[1024];
  char* current;

  int count = 0;

  strcpy(arguments, user_input);
  current = strtok(arguments, delims);

  while(current != NULL){
    current = strtok(NULL, delims);
    count++;
  }

  return count;
}
