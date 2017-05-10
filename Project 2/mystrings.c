/*
Patrick Lyons
COE 0449
Project 2
*/


#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

  //Points to file and opens it
  FILE *user_file;
  user_file = fopen(argv[1], "rb");

  //Chars to store each byte, and the string to be output
  char byte_read;
  char str_output[500];

  //Stores the length of each string to compare
  int length = 0;


  const int lower_ascii = 32;
  const int upper_ascii = 126;

  //32 is lowest readable char
  //126 is highest readable char

  //Makes sure that user passes 1 argument to the file
  if (argc != 2) {
    printf("Error. Please see correct usage: %s FILENAME\n", argv[0]);
    return 0;
  }

  //Checks if file exists, and exits if not
  if (user_file == NULL) {
    printf("\nFile does not exist.\n\n");
    return 0;
  }

  //While not at end of file
  while(feof(user_file) == 0) {
    //Reads chars to byteread from userfile
    fread(&byte_read, sizeof(byte_read), 1, user_file);

    //Checks if a byte is within the range of readable ascii characters, stores it to the output string, and increments length if so.
    if(byte_read >= lower_ascii && byte_read <= upper_ascii) {

        str_output[length] = byte_read;
        length = length + 1;

        //Resets the while loop
        continue;
    }

    //Gets rid of the rest of the string
    str_output[length] = '\0';

    //Prints the string if the length is at least 4
    if (length >= 4) {
      printf("%s\n", str_output);
    }

    //Resets the value of length
    length = 0;


  }//End of while

  //Closes the file
  fclose(user_file);

  //End of program
  return 0;
}
