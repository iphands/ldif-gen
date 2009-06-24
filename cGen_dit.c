#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char * get_randline(char ** strarray, char * line);
char ** load_array(char * filePath, char ** strarray);
unsigned short int free_array(char ** strarray);

int main()
{
  printf("hello world\n\n");
  
  // Start rand seed
  unsigned int seed = (unsigned int)time(NULL);
  srand(seed);


  // initialize string storage, and pass to get_randline
  char line[25];

  /*
    char firstName[25], lastName[25], city[25], state[4];

    for (unsigned short int i = 0; i < 10; i++) {
    strncpy(lastName, get_randline("./lists/last_names", line), 25);
    strncpy(firstName, get_randline("./lists/first_names", line), 25);
    strncpy(city, get_randline("./lists/cities", line), 25);
    strncpy(state, get_randline("./lists/states", line), 4);
  

    // test
    printf("Random person: %s %s from %s, %s\n", firstName, lastName, city, state);    
    }
  */

  char ** fname_array = NULL;
  char ** lname_array = NULL;
  char ** city_array = NULL;
  char ** state_array = NULL;
  //fname_array = load_array("./lists/first_names", fname_array);
  //lname_array = load_array("./lists/last_names", lname_array);
  //city_array = load_array("./lists/cities", city_array);
  state_array = load_array("./lists/states", state_array);
  for (unsigned int i = 0; i < 100000; i++)
    {
      //printf("frand: %s\n", get_randline(fname_array, line));
      //printf("lrand: %s\n", get_randline(lname_array, line));
      //printf("crand: %s\n", get_randline(city_array, line));
      printf("srand: %s\n\n", get_randline(state_array, line));
    }
  printf("size: %d\n", free_array(fname_array));
  
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
  char line[25];

  // loop through file, reallocate ram for each line we find
  while((fgets(line, 25, file)) != NULL)
    {
      strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char *));
      strarray[strcount++] = strdup(line);
    }
  
  
  // close file, and return array
  fclose(file);
  return(strarray);
}

char * get_randline(char ** strarray, char * line)
{
  // Count array length
  unsigned short int i = 0;
  while (strarray[i])
    {
      i++;
    }

  // Generate a random number 0 - i (file length)
  unsigned int choice = (rand() % (i - 1));

  // Set line to a random element in array
  line = strncpy(line, strarray[choice], 25);

  // Strip newline char
  int x = strnlen(line, 25) - 1;
  if ((line[x] == '\n') || (line[x] == ' '))
    line[x] = '\0';

  //Strip a trailing space
  x--;
  if ((line[x] == '\n') || (line[x] == ' '))
    line[x] = '\0';
  
  // return the random line
  return(line);
}
