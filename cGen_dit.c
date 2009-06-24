#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//#define MAX_LINE_SIZE 100;

char * get_randline(char ** strarray, char * line);
char ** load_array(char * filePath, char ** strarray);
unsigned short int free_array(char ** strarray);

unsigned short int MAX_LINE_SIZE = 25;

int main()
{
  printf("hello world\n\n");
  
  // Start rand seed
  unsigned int seed = (unsigned int)time(NULL);
  srand(seed);

  // initialize string storage, and pass to get_randline
  char line[MAX_LINE_SIZE] ;
  char ** fname_array = NULL;
  char ** lname_array = NULL;
  char ** city_array = NULL;
  char ** state_array = NULL;

  // load files into arrays
  fname_array = load_array("./lists/first_names", fname_array);
  lname_array = load_array("./lists/last_names", lname_array);
  city_array = load_array("./lists/cities", city_array);
  state_array = load_array("./lists/states", state_array);

  
  // DEBUG
  for (unsigned int i = 0; i < 1000000; i++)
    {
      printf("it: %d\n", i);
      printf("frand: %s\n", get_randline(fname_array, line));
      printf("lrand: %s\n", get_randline(lname_array, line));
      printf("crand: %s\n", get_randline(city_array, line));
      printf("srand: %s\n", get_randline(state_array, line));
      printf("\n");
    }


  // Cleanup
  printf("size: %d\n", free_array(fname_array));
  printf("size: %d\n", free_array(lname_array));
  printf("size: %d\n", free_array(city_array));
  printf("size: %d\n", free_array(state_array));

  return(0);
}

unsigned short int free_array(char ** strarray)
{
  unsigned short int i = 0;
  while (strarray[i])
    {
      i++;
      free(strarray[i]);
    }
  free(strarray);

  return i;
}

char ** load_array(char * filePath, char ** strarray)
{

  // Open a file, exit on error
  FILE * file;
  if ((file = fopen(filePath, "r")) == NULL)
    {
      printf("Unable to find: %s\n", filePath);
      return(NULL);
    }


  // init vars
  int strcount = 0;
  char line[MAX_LINE_SIZE];

  // loop through file, reallocate ram for each line we find
  while((fgets(line, MAX_LINE_SIZE, file)) != NULL)
    {
      strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char *));
      strarray[strcount++] = strdup(line);
    }
  
  
  // close file, and return array
  fclose(file);
  strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char*));
  strarray[strcount] = NULL;
  return(strarray);
}

char * get_randline(char ** strarray, char * line)
{
  // Count array length
  unsigned short int i = 0;
  while (strarray[i][0] != NULL)
    {
      i++;
    }

  // Generate a random number 0 - i (file length)
  unsigned int choice = (rand() % (i - 1));

  // Get size of line
  const unsigned short int line_len = strlen(strarray[choice]);

  // Set line to a random element in array
  line = strncpy(line, line_len);

  // Strip newline char
  int x = strnlen(line, line_len);
  x--;
  if ((line[x] == '\n') || (line[x] == ' '))
    line[x] = '\0';

  //Strip a trailing space
  x--;
  if ((line[x] == '\n') || (line[x] == ' '))
    line[x] = '\0';
  
  // return the random line
  return(line);
}
