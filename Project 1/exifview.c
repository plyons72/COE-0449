//Patrick Lyons exifview file

//Library imports
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struct for the headers
struct header {
  unsigned short start_marker;
  unsigned short app1_marker;
  unsigned short app1_length;
  const char exif_string[4];
  unsigned short null_term;
  const char tiff_start[2];
  unsigned short version_num;
  unsigned int exif_tiff_offset;
};

//Struct for the TIFF Tag
struct tiff {
  unsigned short tag_ID;
  unsigned short data_type;
  unsigned int num_data;
  unsigned int offset_of_data;
};


//fread reads in a file stream to an array
//fseek sets file position of the stream to the given offset from the whence term
//ftell returns the current file position of a given file stream


//Main
int main(int argc, char *argv[])
{
  //Opens binary file to read
  FILE *image_file;
  image_file = fopen(argv[1], "rb");

  //Unsigned short instantiation
  unsigned short count;
  unsigned short new_count;

  //Initiates structs
  struct header header_type;
  struct tiff tiff_tag;

  //Unsigned int declarations
  unsigned int pix_wid;
  unsigned int pix_height;
  unsigned int iso_speed;
  unsigned int focal_len;

  //Holds fractured ints
  unsigned int frac_1;
  unsigned int frac_2;

  //int declarations
  int location, i, j;

  //Stores strings for manufacturer name, model number, and date taken
  char manufacturer[tiff_tag.num_data];
  char model_num[tiff_tag.num_data];
  char date_taken[tiff_tag.num_data];

  //Declares fstop variable
  double f_stop;

  //Checks if the user passed an argument for the filename
  if(argc != 2) {
      printf("\nYou need to provide a file to read\n"); //Prints out usage of file
      exit(1); //Exits with an error if 2 arguments were not passed (including the file itself)
  }

  //Checks if the image_file file exists. If it doesn't, program terminates
  if (image_file == NULL) {
    printf("\n\nFile does not exist.\n");
    exit(1);
  }

  //Reads the file
  fread(&header_type, 1, 20, image_file);

  //Checks to see if the exif string is correct. Prints an error message and exits otherwise
  if (strcmp(header_type.exif_string, "Exif") != 0)
  {
      printf("\n\nExif tag not found\n");
      exit(1); //Exits with error
  }

  //Checks to see if the TIFF header contains MM instead of II, and prints error if so.
  if (strcmp(header_type.tiff_start, "MM*") == 0) {
      printf("\n\nEndianness not supported\n");
      exit(1); //Exits with error
  }

  //Reads the file
  fread(&count, 1, 2, image_file);

  for (i = 1; i <= count; i++)
  {
      fread(&tiff_tag, 1, 12, image_file);

      //Finds and stores manufacturer string
      if (tiff_tag.tag_ID == 0x010f)
      {
          fseek(image_file, tiff_tag.offset_of_data+12, SEEK_SET);
          fread(manufacturer, 1, tiff_tag.num_data, image_file);

          printf("Manufacturer:\t%s\n", manufacturer);

          //Move to the new offset
          fseek(image_file, 22 + (12 * i), SEEK_SET);
      }

      //Finds and stores the model of the camera
      if (tiff_tag.tag_ID == 0x0110)
      {
          fseek(image_file, tiff_tag.offset_of_data + 12, SEEK_SET);
          fread(model_num, 1, tiff_tag.num_data, image_file);

          printf("Model:\t\t%s\n", model_num);

          fseek(image_file, 22 + (12 * i), SEEK_SET);
      }

      //Exif sub-block address
      if (tiff_tag.tag_ID == 0x8769) {
          fseek(image_file, tiff_tag.offset_of_data+ 12, SEEK_SET);
          fread(&new_count, 1, 2, image_file);

          //
          for (j = 1; j <= new_count; j++) {

            //Reads the tiff tag
            fread(&tiff_tag, 1, 12, image_file);

            //Width in pixels
            if (tiff_tag.tag_ID == 0xa002) {
                pix_wid = tiff_tag.offset_of_data;
                printf("Width:\t\t%d pixels\n", pix_wid);
            }

            //Height in pixels
            if (tiff_tag.tag_ID == 0xa003) {
                pix_height = tiff_tag.offset_of_data;
                printf("Height:\t\t%d pixels\n", pix_height);
            }

            //ISO Speed
            if (tiff_tag.tag_ID == 0x8827) {
                iso_speed = tiff_tag.offset_of_data;
                printf("ISO:\t\tISO %d\n", iso_speed);
            }

            //Exposure speed
            if (tiff_tag.tag_ID == 0x829a) {
                location = ftell(image_file);

                fseek(image_file, tiff_tag.offset_of_data + 12, SEEK_SET);
                fread(&frac_1, 1, 4, image_file);
                fread(&frac_2, 1, 4, image_file);
                fseek(image_file, location, SEEK_SET);

                printf("Exposure Time:\t%d/%d seconds\n", frac_1, frac_2);
            }

            //F-stop
            if (tiff_tag.tag_ID == 0x829d) {
                location = ftell(image_file);

                fseek(image_file, tiff_tag.offset_of_data + 12, SEEK_SET);
                fread(&frac_1, 1, 4, image_file);
                fread(&frac_2, 1, 4, image_file);
                fseek(image_file, location, SEEK_SET);

                f_stop = (double) frac_1 / frac_2;

                printf("F-stop:\t\tf/%.1f\n", f_stop);
            }

            //Lens focal Length
            if (tiff_tag.tag_ID == 0x920a) {
                location = ftell(image_file);

                fseek(image_file, tiff_tag.offset_of_data + 12, SEEK_SET);
                fread(&frac_1, 1, 4, image_file);
                fread(&frac_2, 1, 4, image_file);
                fseek(image_file, location, SEEK_SET);


                focal_len = frac_1 / frac_2;

                printf("Focal Length:\t%d mm \n", focal_len);
            }

            //Date taken
            if (tiff_tag.tag_ID == 0x9003) {
                location = ftell(image_file);

                fseek(image_file, tiff_tag.offset_of_data+12, SEEK_SET);
                fread(date_taken, 1, tiff_tag.num_data, image_file);
                fseek(image_file, location, SEEK_SET);

                printf("Date Taken:\t%s\n", date_taken);
            }

          }

          break;
        } //End of if statement ()

      } //End of outer for loop

  return 0; //Exits successfully
}
