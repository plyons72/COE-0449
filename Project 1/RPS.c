//Patrick Lyons Rock Paper Scissors file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
    char user_choice[10]; //Stores the name of the users choice
    char play_game[5]; //Stores the yes or no value of whether or not they want to play

    int computer_score; //Keeps track of the tournament score for the computer
    int user_score; //Keeps track of the users computer score
    int computer_choice; //Stores the choice of the computer as an easily comparable value
    int choice_value; //Stores the choice of the user as an easily comparable value

    printf("Welcome to Rock, Paper, Scissors \n\n");

    //Stores the users response as to whether or not they want to continue playing the game
    printf("Would you like to play? ");
    scanf("%s", play_game);

    if (strcmp(play_game, "yes") != 0)
    {
      return 0;
    }


    while(strcmp(play_game, "yes") == 0)
    {
      //Initiates scores
      user_score = 0;
      computer_score = 0;

      //While neither side has 3 wins
      while((computer_score < 3 && user_score < 3))
      {
          printf("What is your choice? ");
          scanf("%s", user_choice);

          //user chose rock
          if(strcmp(user_choice, "rock") == 0)
            {
              choice_value = 0;
            }

          //user chose paper
          if(strcmp(user_choice, "paper") == 0)
            {
              choice_value = 1;
            }

          //user chose scissors
          if(strcmp(user_choice, "scissors") == 0)
            {
              choice_value = 2;
            }

          //Computer chooses a random value
          computer_choice = rand() % 3;

          //Depending on the value, string shows what computer chose
          //0 = rock
          //1 = paper
          //2 = scissors
          if(computer_choice == 0)
          {
              printf("The computer chooses rock. ");
          }
          else if(computer_choice == 1)
          {
              printf("The computer chooses paper. ");
          }
          else if(computer_choice == 2)
          {
              printf("The computer chooses scissors. ");
          }

          //Checks if user and computer have the same choice
          if(choice_value == computer_choice)
          {
              printf("You tied!\n");
          }
          //Decides if user won
          else
          {
              if((choice_value == 0 && computer_choice == 2)
                || (choice_value == 1 && computer_choice == 0)
                || (choice_value == 2 && computer_choice == 1))
              {
                  printf("You win this game!\n");
                  user_score++; //Increments users score
              }
              else
              {
                  printf("You lose this game!\n");
                  computer_score++; //Increments computer score
              }
          }

          //Prints out current score
          printf("The score is now you: %d computer %d \n\n", user_score, computer_score);

      }//At end of loop, one of the players has 3 wins

      //If user has 3 wins, user wins
      if(user_score == 3)
      {
          printf("Congratulations! You won the best of 5 tournament. \n");
      }

      //If computer has 3 wins
      else
      {
          printf("The computer won the best of 5 tournament. \n");
      }

      printf("Would you like to play again? ");
      scanf("%s", play_game); //Checks if user wants to play again. If they do, game resets

  } //While loop to continue playing new tournaments

    return 0;
}
