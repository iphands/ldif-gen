#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
//#define _GNU_SOURCE
#include <string.h>


//#define MAX_LINE_SIZE 100;

char * get_randline(char ** strarray, char * line, unsigned short int len);
unsigned short int load_array(char * filePath, char *** strarray);
unsigned short int free_array(char ** strarray, unsigned short int len);
char * make_username(char * fname, char * lname);
char * make_phonenum();

unsigned short int MAX_LINE_SIZE = 30;

int main()
{
  printf("hello world\n\n");
  
  // Start rand seed
  unsigned int seed = (unsigned int)time(NULL);
  srand(seed);

  // Initialize string storage, and pass to get_randline
  char line[MAX_LINE_SIZE] ;
  char ** fname_array = NULL;
  char ** lname_array = NULL;
  char ** city_array = NULL;
  char ** state_array = NULL;
  char ** shell_array = NULL;
  char ** empt_array = NULL;

  unsigned short int fname_array_len, lname_array_len, city_array_len, state_array_len, shell_array_len, empt_array_len = 0;

  // Load files into arrays
  fname_array_len = load_array("./lists/first_names", &fname_array);
  lname_array_len = load_array("./lists/last_names", &lname_array);
  city_array_len = load_array("./lists/cities", &city_array);
  state_array_len = load_array("./lists/states", &state_array);
  shell_array_len = load_array("./lists/shells", &shell_array);
  empt_array_len = load_array("./lists/employee_types", &empt_array);

  
  // DEBUG
  for (unsigned int i = 0; i < 100; i++)
    {
      //printf("it: %d\n", i);
      
      char * frand = (char*)strndup(get_randline(fname_array, line, fname_array_len), 25);
      char * lrand = (char*)strndup(get_randline(lname_array, line, lname_array_len), 25);
      char * crand = (char*)strndup(get_randline(city_array, line, city_array_len) ,25);
      char * srand = (char*)strndup(get_randline(state_array, line, state_array_len), 25);
      char * uname = (char*)make_username(frand, lrand);
      char * shell = (char*)strndup(get_randline(shell_array, line, shell_array_len), 25);
      char * empt = (char*)strndup(get_randline(empt_array, line, empt_array_len), 25);
      char * phnnum = make_phonenum();

      printf("dn: uid=%s,ou=People,dc=example,dc=com\n", uname);
      printf("objectclass: person\n");
      printf("sn: %s\n", lrand);
      //userPassword:
      printf("telephoneNumber: %s\n", phnnum);
      //description:
      //objectclass: organizationalPerson
      //facsimileTelephoneNumber: 048 265 5944
      //street: 6745 Porter Alley
      //postOfficeBox: 9868
      //postalCode: 30905-7182
      //ou: Global Learning Services
      printf("st: %s\n", srand);
      //l: Paterson
      //objectclass: inetOrgPerson
      //employeeNumber: 613-19-6955
      printf("employeeType: %s\n", empt);
      printf("givenName: %s\n", frand);
      printf("homePhone: %s\n", phnnum);
      //homePostalAddress: 2381 Hightower Road $ Downey, RI 78307-9416
      //jpegPhoto:< file:///home/iphands/prog/python/gen_dit/faces/butterfly.png
      //mobile: 241 293 9212
      //pager: 193 622 1437
      //objectclass: inetOrgPerson
      //uidNumber: 10000
      //gidNumber: 10000
      printf("loginShell: %s\n", shell);
      //objectclass: shadowAccount
      //shadowLastChange: 14421
      //shadowMin: 0
      //shadowMax: 90
      //shadowWarning: 14
      //shadowInactive: 0
      printf("cn: %s %s\n", frand, lrand);
      printf("uid: %s\n", uname);
      printf("mail: %s@example.com\n", uname);
      //homeDirectory: /home/j/jaimdhundhara 
      
      //printf("name: %s %s\nuid: %s\nphone: %s\nstate: %s\ncity: %s\n\n", frand, lrand, uname, phnnum, crand, srand);

      free(phnnum);
      free(frand);
      free(lrand);
      free(crand);
      free(srand);
      free(uname);
      free(shell);
      free(empt);
      printf("\n");
    }


  // Cleanup

  printf("size: %d\n", free_array(fname_array, fname_array_len));
  printf("size: %d\n", free_array(lname_array, lname_array_len));
  printf("size: %d\n", free_array(city_array, city_array_len));
  printf("size: %d\n", free_array(state_array, state_array_len));
  printf("size: %d\n", free_array(shell_array, shell_array_len));
  printf("size: %d\n", free_array(empt_array, empt_array_len));

  return(0);
}

char * make_phonenum()
{
  char phnnum[13];
  snprintf(phnnum, 13, "%d %d %d", (rand() % 690) + 300, (rand() % 899) + 100, (rand() % 8999) + 1000);
  //printf("(%s) ", phnnum);
  return(strndup(phnnum, 13));
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
  unsigned short int nums = (rand() % 899) + 100;

  // Make a char array for the username
  x = (x + sizeof(char) + (sizeof(unsigned short int) * 3)) + 1;
  char uname[x];
  
  // Build uname
  snprintf(uname, x, "%c%s%d", fc, lname, nums);

  // Cleanup and return
  free(lname);
  return(strndup(uname, x));
}



unsigned short int free_array(char ** strarray, unsigned short int len)
{

  for (unsigned short int i = 0; i < len; i++)
    {
      free(strarray[i]);
    }
  free(strarray);

  /*
  unsigned short int i = 0;
  while (strarray[i])
    {
      i++;
      free(strarray[i]);
    }
  free(strarray);
  */

  return len;
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


  // Init vars
  unsigned short int strcount = 0;
  char line[MAX_LINE_SIZE];

  // Loop through file, reallocate ram for each line we find
  while((fgets(line, MAX_LINE_SIZE, file)) != NULL)
    {
      strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char *));
      strarray[strcount++] = (char*)strdup(line);
    }
  
  
  // Close file, and return array
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
  unsigned short int x = strnlen(line, line_len);
  x--;
  if ((line[x] == '\n') || (line[x] == ' '))
    line[x] = '\0';

  // Strip a trailing space
  x--;
  if ((line[x] == '\n') || (line[x] == ' '))
    line[x] = '\0';
  
  // Return the random line
  return(line);
}
