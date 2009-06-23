#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char * getRandLine(char * filePath, char * line);

int main()
{
  printf("hello world\n\n");
  
  // Start rand seed
  unsigned int seed = (unsigned int)time(NULL);
  srand(seed);


  // initialize string storage, and pass to getRandLine
  char line[25];
  char firstName[25], lastName[25], city[25], state[25];



  for (unsigned short int i = 0; i < 10000; i++) {
    strncpy(lastName, getRandLine("./lists/last_names", line), 25);
    strncpy(firstName, getRandLine("./lists/first_names", line), 25);
    strncpy(city, getRandLine("./lists/cities", line), 25);
    strncpy(state, getRandLine("./lists/states", line), 25);
  

    // test
    // printf("Random person: %s %s from %s, %s\n", firstName, lastName, city, state);    
  }

  return(0);
}



char * getRandLine(char * filePath, char * line)
{

  // Open a file, exit on error
  FILE * file;
  if ((file = fopen(filePath, "r")) == NULL)
    {
      printf("Unable to find: %s\n", filePath);   
      return(NULL);
    } 

  // Run through the file to determine its length. Is this optimal??
  unsigned short int i = 0;
  while (fgets(line, 25, file))
    {
      i++;
    }

  // Generate a random number 0 - i (file length)
  unsigned int choice = rand() % i;

  // Go back to the top of the file
  rewind(file);

  // Run through the file until we find get to the choice (random) element
  i = 0;
  while ((fgets(line, 25, file)) && (i < choice))
    {
      i++;
    }

  // Strip newline char
  int x = strnlen(line, 25) - 1;
  if ((line[x] == '\n') || (line[x] == ' '))
    line[x] = '\0';

  //Strip a trailing space
  x--;
  if ((line[x] == '\n') || (line[x] == ' '))
    line[x] = '\0';
  
  // Cleanup and return
  fclose(file);
  return(line);
}
