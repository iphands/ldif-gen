#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
//#define _GNU_SOURCE
#include <string.h>


//#define MAX_LINE_SIZE 100;

char * get_randline(char ** strarray, char * line, unsigned short int len);
unsigned short int load_array(char * filePath, char *** strarray);
unsigned short int free_array(char ** strarray);
char * make_username(char * fname, char * lname);

unsigned short int MAX_LINE_SIZE = 30;

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

  unsigned short int fname_array_len, lname_array_len, city_array_len, state_array_len = 0;

  // load files into arrays
  fname_array_len = load_array("./lists/first_names", &fname_array);
  lname_array_len = load_array("./lists/last_names", &lname_array);
  city_array_len = load_array("./lists/cities", &city_array);
  state_array_len = load_array("./lists/states", &state_array);

  
  // DEBUG
  for (unsigned int i = 0; i < 1000000; i++)
    {
      //printf("it: %d\n", i);
      
      char * frand = NULL;
      char * lrand = NULL;
      char * crand = NULL;
      char * srand = NULL;
      char * uname = NULL;
      
      frand = (char*)strdup(get_randline(fname_array, line, fname_array_len), 25);
      lrand = (char*)strdup(get_randline(lname_array, line, lname_array_len), 25);
      crand = (char*)strdup(get_randline(city_array, line, city_array_len) ,25);
      srand = (char*)strdup(get_randline(state_array, line, state_array_len), 25);
      uname = (char*)make_username(frand, lrand);

      printf("%s %s (%s) from %s, %s\n", frand, lrand, uname, crand, srand);

      free(frand);
      free(lrand);
      free(crand);
      free(srand);
      free(uname);
      //printf("\n");
    }


  // Cleanup
  printf("size: %d\n", free_array(fname_array));
  printf("size: %d\n", free_array(lname_array));
  printf("size: %d\n", free_array(city_array));
  printf("size: %d\n", free_array(state_array));

  return(0);
}

char * make_username(char * pfname, char * plname)
{
  // Set fc to the lowercase first char in pfname
  char fc = tolower(pfname[0]); 

  // Make a local lname
  char * lname = NULL;
  
  // Get the length of plname and malloc space for it
  unsigned short int x = strnlen(plname, 25);
  lname = calloc(x + 1, sizeof(char));

  // Run through plname and make lname, but lowercase
  for (unsigned short int i = 0; i < x; i++)
    {      
      char c = plname[i];
      if (c == ' ')
	{
	  lname[i] = '_';	  
	}
      else
	{
	  lname[i] = tolower(c);
	}      
    }

  // Get a random number between 100 & 999
  unsigned int nums = (rand() % 899) + 100;

  // make a char array for the username
  x = (x + sizeof(char) + (sizeof(unsigned short int) * 3)) + 1;
  char uname[x];
  
  // Build uname
  snprintf(uname, x, "%c%s%d", fc, lname, nums);

  // Cleanup and return
  free(lname);
  return(strdup(uname, x));
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

unsigned short int load_array(char * filePath, char *** pstrarray)
{ 
  // Declare temporary pointer storage for the passed in strarray
  char ** strarray = *pstrarray;

  // Open a file, exit on error
  FILE * file;
  if ((file = fopen(filePath, "r")) == NULL)
    {
      printf("Unable to find: %s\n", filePath);
      return(0);
    }


  // init vars
  int strcount = 0;
  char line[MAX_LINE_SIZE];

  // loop through file, reallocate ram for each line we find
  while((fgets(line, MAX_LINE_SIZE, file)) != NULL)
    {
      strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char *));
      strarray[strcount++] = (char*)strdup(line);
    }
  
  
  // close file, and return array
  fclose(file);


  strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char*));
  strarray[strcount] = NULL;  

  // Set the passed in array to the temporary storage address
  *pstrarray = strarray;

  // Return the array length
  return(strcount);
}

char * get_randline(char ** strarray, char * line, unsigned short int len)
{
  // Count array length
  unsigned short int i = len;

  // Generate a random number 0 - i (file length)
  unsigned int choice = (rand() % (i - 1));

  // Get size of line
  const unsigned short int line_len = strlen(strarray[choice]);

  // Set line to a random element in array
  line = strncpy(line, strarray[choice], line_len);

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
