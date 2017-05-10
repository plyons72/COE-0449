//Patrick Lyons
//CoE 0449
//Project 5
//April 23 2017

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

//Function prototypes
int play_or_quit();
int roll_dice();
int yes_or_no();
int end_game(int status);

int main()
{
  //Allocates space of 20 chars to hold users name
  char *user_name = malloc(sizeof(char) * 20);

  //Stores value of point, if needed
  int point;

  //Holds 1 when the user hasnt won yet
  int still_playing = 1;

  //Stores value of rolled
  int roll;

  //Length of string
  int length;

  //Stores the number of times the die was rolled
  int roll_num;

  //0 if user wants to play again
  int go_again;

  //Initial Welcome statement
  printf("Welcome to Patrick's Casino!\n");

  //Gets the name from the user, storing all but the newline into user_name
  printf("Please enter your name: ");
  fgets(user_name, 20, stdin);
  length = strlen(user_name);
  if(user_name[length - 1] == '\n') { user_name[length - 1] = '\0'; }

  //Checks if user wants to play, and stores into play_or_quit
  printf("%s, would you like to Play or Quit? ", user_name);

  //User wants to play
  if(play_or_quit() == 0)
  {

    do
    {
      //Sets roll_num to 1, and sets point = to die roll
      roll_num = 1;
      point = roll_dice();
      roll = point;

      //Checks immediate win
      if(roll == 7 || roll == 11)
      {
        go_again = end_game(0);
      }

      //Checks immediate loss
      else if(roll == 2 || roll == 3 || roll == 12)
      {
        go_again = end_game(1);
      }

      //User did not win or lose yet
      else
      {
        do
        {
          //Rolls the dice again
          roll = roll_dice();

          //Checks if user rolled the point

          //User got point(win)
          if(point == roll)
          {
            still_playing = 0;
            go_again = end_game(0);
          }

          //A roll after the first one equals 7 (lose)
          else if(roll == 7)
          {
            still_playing = 0;
            go_again = end_game(1);
          }

          //User did not win or lose
          else
          {
            still_playing = 1;
          }

        }while(still_playing == 1);
      }
      printf("\n\n");

    }while(go_again == 0); //Loops while user is still playing

  }//End of game

  //User does not want to play
  else
  {
    //If user leaves play loop, give goodbye message and ends
    printf("Goodbye %s\n\n", user_name);
    return 0;
  }

  //If user leaves play loop, give goodbye message and ends
  printf("Goodbye %s\n\n", user_name);
  return 0;


}//End of main

//Function to check if user wants to play again.
//Returns 0 to play, or 1 to quit
int play_or_quit()
{
  //Holds error check
  int error = 1;

  //Holds length of string
  int length;

  //Allocates space for user to input if they still want to play
  char *play_or_quit = malloc(sizeof(char) * 20);

  //Stores strings of answers to compare to
  char *play = "play";
  char *quit = "quit";

  do {
    fgets(play_or_quit, 10, stdin);
    length = strlen(play_or_quit);
    if(play_or_quit[length - 1] == '\n') { play_or_quit[length - 1] = '\0'; }

    //Checks if user entered play
    if(strcmp(play_or_quit, play) == 0)
    {
      error = 0;
      return 0;
    }

    //Checks if user entered quit
    if(strcmp(play_or_quit, quit) == 0)
    {
      error = 0;
      return 1;
    }

    //User entered something invalid
    else
    {
      printf("\nInvalid. Please enter play or quit only ");
      error = 1;
    }

  } while(error == 1);

    return 1;

}

//Function that rolls the dice and retuns the value
int roll_dice()
{
  /*int first_roll = 0;
  int second_roll = 0;*/

  int f_roll;
  int s_roll;
  int total_roll = 0;

  //i is the file descriptor for first open
  //j is same as above, but for the second roll
  int i;
  int j;

  //Gets the dice from the driver

  //Opens dice for read only, and stores value into first_roll or second_roll
  i = open("/dev/dice", O_RDONLY);
  read(i, &f_roll, sizeof(int));
  close(i);

  j = open("/dev/dice", O_RDONLY);
  read(j, &s_roll, sizeof(int));
  close(j);

/*  first_roll = rand() % 6 + 1;
  second_roll = rand() % 6 + 1;*/
  total_roll = f_roll + s_roll;

  printf("\nYou rolled %d + %d = %d\n", f_roll, s_roll, total_roll);

  return total_roll;
}

//Returns 0 for yes, and 1 for no
int yes_or_no()
{
  char *yes_or_no = malloc(sizeof(char) * 5);

  //Strings to compare to user input
  char *yes = "yes";
  char *no = "no";

  //Holds error checker
  int error = 1;

  //Holds string length
  int length;

  do
  {
    fgets(yes_or_no, 5, stdin);
    length = strlen(yes_or_no);
    if(yes_or_no[length - 1] == '\n') { yes_or_no[length - 1] = '\0'; }

    //Checks if user entered play
    if(strcmp(yes_or_no, yes) == 0)
    {
      error = 0;
      return 0;
    }

    //Checks if user entered quit
    if(strcmp(yes_or_no, no) == 0)
    {
      error = 0;
      return 1;
    }

    //User entered something invalid
    else
    {
      printf("\nInvalid. Please enter yes or no only ");
      error = 1;
    }

  } while(error == 1);

  return 1;
}

//Returns 0 for play again, 1 for stop
//Status holds win or lose value 0 for win, 1 for lose
int end_game(int status)
{
  int again;

  if(status == 0)
  {
    printf("\nYou win!\n");
  }

  else
  {
    printf("You lose!\n");
  }

  printf("Would you like to play again(yes/no): ");
  again = yes_or_no();

  //User wants to play again
  if(again == 0)
  {
    return 0;
  }

  //User does not want to play again
  else
  {
    return 1;
  }
}
