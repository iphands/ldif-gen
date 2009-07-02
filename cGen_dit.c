#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <dirent.h>
#include <linux/types.h>
#include <unistd.h>

//#define MAX_LINE_SIZE 100;

//char * get_randline(char ** strarray, char * line, unsigned short int len);
char * get_randline(char ** strarray, unsigned short int len);
unsigned short int load_array(char * filePath, char *** strarray);
unsigned short int free_array(char ** strarray, unsigned short int len);
char * make_username(char * fname, char * lname);
char * make_phonenum();
char * make_street_addr(char * street, unsigned short int len);
char * make_postal();
char * make_employee_num();
unsigned short int load_dir_array(char * path, char *** pstrarray);
char * make_passwd();
char * make_sentence();

unsigned short int MAX_LINE_SIZE = 100;

int main()
{
  printf("hello world\n\n");
  
  // Start rand seed
  unsigned int seed = (unsigned int)time(NULL);
  srand(seed);

  // Initialize string storage, and pass to get_randline
  //char line[MAX_LINE_SIZE] ;
  char ** fname_array = NULL;
  char ** lname_array = NULL;
  char ** city_array = NULL;
  char ** state_array = NULL;
  char ** shell_array = NULL;
  char ** empt_array = NULL;
  char ** street_array = NULL;
  char ** imgs_array = NULL;

  unsigned short int fname_array_len, lname_array_len, city_array_len, state_array_len, shell_array_len, empt_array_len, street_array_len, imgs_array_len = 0;

  // Load files into arrays
  fname_array_len = load_array("./lists/first_names", &fname_array);
  lname_array_len = load_array("./lists/last_names", &lname_array);
  city_array_len = load_array("./lists/cities", &city_array);
  state_array_len = load_array("./lists/states", &state_array);
  shell_array_len = load_array("./lists/shells", &shell_array);
  empt_array_len = load_array("./lists/employee_types", &empt_array);
  street_array_len = load_array("./lists/streets", &street_array);
  imgs_array_len = load_dir_array("./faces/",  &imgs_array);  

  unsigned int guid = 20000;


  // Test

  // DEBUG
  for (unsigned int i = 0; i < 1000000; i++)
    {
      //printf("it: %d\n", i);
      
      char * frand = get_randline(fname_array, fname_array_len);
      char * lrand = get_randline(lname_array, lname_array_len);
      char * crand = get_randline(city_array, city_array_len);
      char * srand = get_randline(state_array, state_array_len);
      char * uname = (char*)make_username(frand, lrand);
      char * shell = get_randline(shell_array, shell_array_len);
      char * empt = get_randline(empt_array, empt_array_len);
      char * phnnum = make_phonenum();
      char * street_name = get_randline(street_array, street_array_len);
      char * street_addr = make_street_addr(street_name, (strnlen(street_name, 100) + 6 + 1));            
      char * postal_code = make_postal();
      char * empl_num = make_employee_num();
      char * mobile_phnnum = make_phonenum();
      char * pager_num = make_phonenum();      
      char * img = get_randline(imgs_array, imgs_array_len);
      //char * passwd = make_passwd();
      char * sentence = make_sentence();

      printf("dn: uid=%s,ou=People,dc=example,dc=com\n", uname);
      printf("objectclass: person\n");
      printf("sn: %s\n", lrand);
      //printf("userPassword: %s\n", passwd);
      printf("telephoneNumber: %s\n", phnnum);
      printf("description: %s\n", sentence);
      printf("objectclass: organizationalPerson\n");
      printf("facsimileTelephoneNumber: %s\n", mobile_phnnum);
      printf("street: %s\n", street_addr);
      printf("postOfficeBox: %d\n", (rand() % 899999) + 100000);
      printf("postalCode: %s\n", postal_code);
      printf("ou: Global Learning Services\n");
      printf("st: %s\n", srand);
      printf("l: %s\n", crand);
      printf("objectclass: inetOrgPerson\n");
      printf("employeeNumber: %s\n", empl_num);
      printf("employeeType: %s\n", empt);
      printf("givenName: %s\n", frand);
      printf("homePhone: %s\n", phnnum);
      printf("homePostalAddress: %s $ %s, %s %s\n", street_addr, crand, srand, postal_code);
      printf("jpegPhoto:< file:///home/iphands/prog/python/gen_dit/faces/%s\n", img);
      printf("mobile: %s\n", mobile_phnnum);
      printf("pager: %s\n", pager_num);
      printf("objectclass: posixAccount\n");
      printf("uidNumber: %d\n", guid);
      printf("gidNumber: %d\n", guid);
      guid++;

      printf("loginShell: %s\n", shell);
      printf("objectclass: shadowAccount\n");
      printf("shadowLastChange: 14422\n");
      printf("shadowMin: 0\n");
      printf("shadowMax: 90\n");
      printf("shadowWarning: 14\n");
      printf("shadowInactive: 0\n");
      printf("cn: %s %s\n", frand, lrand);
      printf("uid: %s\n", uname);
      printf("mail: %s@example.com\n", uname);
      printf("homeDirectory: /home/%c/%s\n", uname[0], uname);



      //printf("name: %s %s\nuid: %s\nphone: %s\nstate: %s\ncity: %s\n\n", frand, lrand, uname, phnnum, crand, srand);

      // Cleanup
      free(phnnum);
      free(frand);
      free(lrand);
      free(crand);
      free(srand);
      free(uname);
      free(shell);
      free(empt);
      free(street_name);
      free(street_addr);
      free(postal_code);
      free(empl_num);
      free(mobile_phnnum);
      free(pager_num);
      free(img);
      //free(passwd);
      free(sentence);
      printf("\n");
    }



  // Cleanup
  printf("fname size: %d\n", free_array(fname_array, fname_array_len));
  printf("lname size: %d\n", free_array(lname_array, lname_array_len));
  printf("city size: %d\n", free_array(city_array, city_array_len));
  printf("state size: %d\n", free_array(state_array, state_array_len));
  printf("shell size: %d\n", free_array(shell_array, shell_array_len));
  printf("empt size: %d\n", free_array(empt_array, empt_array_len));
  printf("street size: %d\n", free_array(street_array, street_array_len));
  printf("imgs size: %d\n", free_array(imgs_array, imgs_array_len));


  return(0);
}

char * make_sentence()
{
  unsigned short int count = (rand() % 10) + 10;
  unsigned short int sent_len = (count + 2) * 13;
  char sent[sent_len];

  strcpy(sent, "random_pseudo_sentence");

  for (unsigned int x = 0; x < count; x++)
    {
      unsigned short int len = (rand() % 10) + 2;
      char word[len];
      for (unsigned int i = 0; i < len; i++)
        {
          word[i] = (char)((rand() % 25) + 97);
        }
      word[len] = '\0';

      sprintf(sent, "%s %s", sent, word);
    }

  return(strndup(sent, sent_len));
}

char * make_passwd()
{
  char salt[11];
  sprintf(salt, "$1$%d", rand() % 9999999 );
  char * passwd = crypt((char*)"redhat", salt);
  unsigned short int len = strnlen(passwd, 35);

  //printf("DEBUG: %s\n", passwd);
  return(strndup(passwd, len));
}

unsigned short int load_dir_array(char * dirPath, char *** pstrarray)
{
  // Declare temporary pointer storage for the passed in strarray
  char ** strarray = *pstrarray;

  // Open a FD on the supplied path
  const DIR * dir = opendir(dirPath);
  
  // If the dir cant be opened return
  if (dir == NULL)
    {
      return(0);
    }
  
  // Local storage for files found
  char * string;

  // A struct to interact with the directory entry
  const struct dirent *pDirEnt;

  // Init vars
  unsigned short int strcount = 0;

  // Loop through the directory list
  for (pDirEnt = readdir((DIR*)dir); pDirEnt; pDirEnt = readdir((DIR*)dir))
    {
      string = (char*)pDirEnt->d_name;
      if(!strcmp(string, ".") || !strcmp(string, ".."))
	continue;
      
      strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char *));
      strarray[strcount++] = (char*)strdup(string);
    }

  // Close file, and return array
  closedir(dir);
  
  strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char*));
  strarray[strcount] = NULL;  
  
  // Set the passed in array to the temporary storage address
  *pstrarray = strarray;  
  
  return(strcount);
}


char * make_postal()
{
  /*
    Example:
    postalCode: 30905-7182
  */

  // Make storage
  char postal_code[11];

  // Build string
  snprintf(postal_code, 11, "%d-%d", (rand() % 89999) + 10000, (rand() % 8999) + 1000);
 
  // Return formatted postal code
  return((char*)strndup(postal_code, 11));
}

char * make_street_addr(char * street, unsigned short int len)
{
  /* 
     Example: 
     street: 6745 Porter Alley
  */
  
  //printf("\nDEBUG: %d -- %s\n", len, street);

  // Make storage
  char addr[len + 6];

  // Build string
  snprintf(addr, (len + 6), "%d %s", (rand() % 18000), street);
  
  // Return formatted street address
  return((char*)strndup(addr, len));
}

char * make_phonenum()
{
  // Make storage
  char phnnum[13];

  // Build string
  snprintf(phnnum, 13, "%d %d %d", (rand() % 690) + 300, (rand() % 899) + 100, (rand() % 8999) + 1000);

  // Return formatted phone number
  return((char*)strndup(phnnum, 13));
}

char * make_employee_num()
{
  // Make storage
  char emplnum[12];

  // Build string
  snprintf(emplnum, 12, "%d-%d-%d", (rand() % 690) + 300, (rand() % 89) + 10, (rand() % 8999) + 1000);

  // Return formatted phone number
  return((char*)strndup(emplnum, 13));
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
  return((char*)strndup(uname, x));
}



unsigned short int free_array(char ** strarray, unsigned short int len)
{

  for (unsigned short int i = 0; i < len; i++)
    {
      free(strarray[i]);
    }
  free(strarray);

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

char * get_randline(char ** strarray, unsigned short int len)
{
  // Count array length
  unsigned short int i = len;

  // Generate a random number 0 - i (file length)
  unsigned int choice = (rand() % (i - 1));

  // Get size of line
  const unsigned short int line_len = strlen(strarray[choice]);

  // Set line to a random element in array
  char line[line_len + 1];
  strncpy(line, strarray[choice], line_len + 1);

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
  return(strndup(line, line_len + 1));
}

