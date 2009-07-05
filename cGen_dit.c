/*
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** Any non-GPL usage of this software or parts of this software is strictly
** forbidden.
**
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <linux/types.h>
#include <unistd.h>
#include <search.h>

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
char * cheat_make_passwd();
char * make_sentence();
char dup_check(char ** array, unsigned int len, char * string);
char * trim_newline(char * string);

unsigned short int MAX_LINE_SIZE = 100;

int main(int argc, char *argv[])
{
 
  // Default basedn
  char d_basedn[50] = "dc=example,dc=com";
  char basedn[50] = "dc=example,dc=com";

  // How many iterations
  unsigned int d_iter = 10000;
  unsigned int iter = d_iter;
 
  // Set fast passwd
  char fast_passwd = '1';

  // Set create_ou
  char create_ou = '0';

  // Starting uid/gid
  unsigned int guid = 20000;

  int opt;
  while ((opt = getopt(argc, argv, "b:i:rch")) != -1)
    {
      switch (opt) 
        {
	case 'b':
	  strncpy(basedn, argv[optind - 1], 50);
	  break;	    
        case 'i':
	  iter = atoi(optarg);
          break;
	case 'r':
	  fast_passwd = '0';
	  break;
	case 'c':
	  create_ou = '1';
	  break;
	case 'h':	  
	default: /* '?' */
          fprintf(stderr, "Usage: %s [-b basedn] [-i num_of_users] [-r] [-c] [-h]\n", argv[0]);
	  fprintf(stderr, "Generate an ldif format list of users for use in a directory\n");
	  fprintf(stderr, "  -b\t Specify a basedn (default is %s)\n", d_basedn);
	  fprintf(stderr, "  -i\t Specify the number of iterations (default is %d)\n", d_iter);
	  fprintf(stderr, "  -r\t Generate a new password hash for each user\n");
	  fprintf(stderr, "  -c\t Create entries for ou=People and ou=Groups\n");
	  fprintf(stderr, "  -h\t Shows this usage output\n");
          exit(EXIT_FAILURE);
        }
    }


  // Seed rand with time
  //unsigned int seed = (unsigned int)time(NULL);
  unsigned int seed = 1111;
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
  char ** uname_array = NULL;
  char ** domains_array = NULL;

  unsigned short int fname_array_len, lname_array_len, city_array_len, state_array_len, shell_array_len, empt_array_len, street_array_len, imgs_array_len, domains_array_len = 0;

  unsigned int uname_array_len = 0;

  // Load files into arrays
  fname_array_len = load_array("./lists/first_names", &fname_array);
  lname_array_len = load_array("./lists/last_names", &lname_array);
  city_array_len = load_array("./lists/cities", &city_array);
  state_array_len = load_array("./lists/states", &state_array);
  shell_array_len = load_array("./lists/shells", &shell_array);
  empt_array_len = load_array("./lists/employee_types", &empt_array);
  street_array_len = load_array("./lists/streets", &street_array);
  imgs_array_len = load_dir_array("./faces/",  &imgs_array);
  domains_array_len = load_array("./lists/domains", &domains_array);
  
  ENTRY e;
  hcreate(iter);

  if (create_ou == '1')
    {
      printf("dn: ou=People,%s\n", basedn);
      printf("objectclass: organizationalUnit\n");
      printf("ou: People\n\n");
      
      printf("dn: ou=Groups,%s\n", basedn);
      printf("objectclass: organizationalUnit\n");
      printf("ou: Groups\n\n");      
    }

  for (unsigned int i = 0; i < iter; i++)
    {      
      char * frand = get_randline(fname_array, fname_array_len);
      char * lrand = get_randline(lname_array, lname_array_len);
      char * uname = (char*)make_username(frand, lrand);

      e.key = uname;
      e.data = (void*)i;

      if (hsearch(e, FIND) == NULL)
	{
	  if (hsearch(e, ENTER) == NULL)
	    {
	      fprintf(stderr, "Error writing uname (%s) to the hash table.\n", uname);
	      exit(EXIT_FAILURE);
	    }

	  uname_array = (char **)realloc(uname_array, (uname_array_len + 1) * sizeof(char *));
	  uname_array[uname_array_len++] = (char*)strndup(uname, MAX_LINE_SIZE);
	} 
      else 
	{
	  fprintf(stderr, "# Decected dupe: %s\n", uname);
	  i--;
	  
	  free(frand);
	  free(lrand);
	  free(uname);
	  continue;
	}
      
      char * crand = get_randline(city_array, city_array_len);
      char * srand = get_randline(state_array, state_array_len);
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
      char * domain = get_randline(domains_array, domains_array_len);

      char * passwd = NULL;
      if (fast_passwd == '1')
	passwd = cheat_make_passwd();
      else
	passwd = make_passwd();
      
      char * sentence = make_sentence();
      
      printf("#--- %s ---#\n", uname);
      printf("dn: uid=%s,ou=People,%s\n", uname, basedn);
      printf("objectclass: person\n");
      printf("sn: %s\n", lrand);
      printf("userPassword: {crypt}%s\n", passwd);
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
      printf("jpegPhoto:< file:///tmp/faces/%s\n", img);
      printf("mobile: %s\n", mobile_phnnum);
      printf("pager: %s\n", pager_num);
      printf("objectclass: posixAccount\n");
      printf("uidNumber: %d\n", guid + i);
      printf("gidNumber: %d\n", guid + i);

      printf("loginShell: %s\n", shell);
      printf("objectclass: shadowAccount\n");
      printf("shadowLastChange: 14422\n");
      printf("shadowMin: 0\n");
      printf("shadowMax: 90\n");
      printf("shadowWarning: 14\n");
      printf("shadowInactive: 0\n");
      printf("cn: %s %s\n", frand, lrand);
      printf("uid: %s\n", uname);
      printf("mail: %s@%s\n", uname, domain);
      printf("homeDirectory: /home/%c/%s\n", uname[0], uname);

      printf("\n");


      printf("dn: cn=%s,ou=Groups,%s\n", uname, basedn);
      printf("objectclass: posixGroup\n");
      printf("cn: %s\n", uname);
      printf("gidNumber: %d\n", guid + i);
      printf("memberUid: %s\n", uname);

      printf("\n\n\n");


      // Cleanup...
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
      free(passwd);
      free(sentence);
      free(domain);
    }
  hdestroy(); 

  // Array for groups
  char ** groups_array = NULL;
  
  // Populate the groups array
  unsigned short int groups_array_len = load_array("./lists/groups", &groups_array);

  // Loop through each group
  for (unsigned short int i = 0; i < groups_array_len; i++)
    {
      // Remove new line from the groups strings
      groups_array[i] = trim_newline(groups_array[i]);

      // Print the dn, objectclass, posixGroup, cn, and gidNumber
      printf("\n#--- %s ---#\n", groups_array[i]);
      printf("dn: cn=%s,ou=Groups,%s\n", groups_array[i], basedn);
      printf("objectclass: posixGroup\ncn: admin\ngidNumber: %d\n", 2000 + i);
      
      // Loop through the users
      for(unsigned int ci = 0; ci < iter; ci++)
	{
	  char bool = rand() % groups_array_len;
	  if (bool == 1)
	    {
	      printf("memberUid:%s\n", uname_array[ci]);
	    }
	}
      printf("\n");
    }

  printf("\n\n\n");
  // Cleanup
  printf("# fname size: %d\n", free_array(fname_array, fname_array_len));
  printf("# lname size: %d\n", free_array(lname_array, lname_array_len));
  printf("# city size: %d\n", free_array(city_array, city_array_len));
  printf("# state size: %d\n", free_array(state_array, state_array_len));
  printf("# shell size: %d\n", free_array(shell_array, shell_array_len));
  printf("# empt size: %d\n", free_array(empt_array, empt_array_len));
  printf("# street size: %d\n", free_array(street_array, street_array_len));
  printf("# imgs size: %d\n", free_array(imgs_array, imgs_array_len));
  printf("# uname size: %d\n", free_array(uname_array, uname_array_len));
  printf("# groups size: %d\n", free_array(groups_array, groups_array_len));
  printf("# domains size: %d\n", free_array(domains_array, domains_array_len));

  return(EXIT_SUCCESS);
}

char dup_check(char ** array, unsigned int len, char * string)
{
  // Setup a char to be used as a boolean
  char bool = '0';

  // Get the length of string
  unsigned short int slen = strlen(string);

  // Loop through the array
  for (unsigned int i = 0; i < len; i++)
    {
      // If the first chars do not match quit
      if (string[0] == array[i][0])
	{
	  
	  // If the last the chars in slen do not match quit
	  if (string[slen] == array[i][slen])
	    {
	      
	      // If the strings match completely return 1
	      if (strncmp(array[i], string, slen + 1) == 0)
		{	  
		  bool = '1';       
		  return bool;
		}
	    }
	}
    }
  
  // Return
  return bool;
}

char * make_sentence()
{
  // Get a number of words
  unsigned short int count = (rand() % 10) + 10;

  // Figure an aproximate size for the sentence (words + space) * max_word length
  unsigned short int sent_len = (count + 1) * 13;

  // Make a char array
  char sent[sent_len];

  // Set the initial value of the sent
  strncpy(sent, "random_pseudo_sentence", sent_len + 1);

  // Loop through the word count
  for (unsigned int x = 0; x < count; x++)
    {
      // Generate a word length 2 - 12
      unsigned short int len = (rand() % 10) + 2;

      // Make word storage
      char word[len];

      // Run through each element and set it to a lowercase ascii value 97 - 122
      for (unsigned int i = 0; i < len; i++)
        {
          word[i] = (char)((rand() % ('z' - 'a')) + 'a');
        }

      // Set the last char to \0
      word[len] = '\0';

      // Concat the sentence and new word
      sprintf(sent, "%s %s", sent, word);
    }

  // return
  return(strndup(sent, sent_len));
}

char * cheat_make_passwd()
{
  // A function to speed up the program ~ 15 fold... Always returns the same 'redhat' salted hash!
  return(strndup("$1$FiEvfxd2$M2VDffWJOXHgaHiusqQxo1", 36));
}

char * make_passwd()
{
  // Init strorage for the salt
  char salt[11];
  
  // Turn salt into a string with the MD5 sum type
  sprintf(salt, "$1$%d", rand() % 9999999 );
  
  // Use crypt to generate a salted MD5 hash
  char * passwd = crypt((char*)"redhat", salt);

  // Get the length of the hash
  unsigned short int len = strnlen(passwd, 50);

  //printf("DEBUG: %s\n", passwd);

  // Return the hash
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
      if(!strncmp(string, ".", MAX_LINE_SIZE) || !strncmp(string, "..", MAX_LINE_SIZE))
	continue;
      
      strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char *));
      strarray[strcount++] = (char*)strndup(string, MAX_LINE_SIZE);
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
      if ((('a' < c) || (c > 'z')) || (('A' < c) || (c > 'Z')))
	{
	  /*
	  if (c == ' ')
	    {
	      lname[i] = '_';	  
	      }	 
	  */
	  // else
	  //{
	  lname[i] = tolower(c);
	  //}      
	}
      else
	{
	  lname[i] = '.';
	}
    }
  
  // Get a random number between 1000 & 9999
  unsigned short int nums = (rand() % 8999) + 1000;

  // Make a char array for the username
  x = ((x + sizeof(char) - 1) + (sizeof(unsigned short int) * 4) - 1);
  //printf("DEBUG: %d\n", x);
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
      strarray[strcount++] = (char*)strndup(line, MAX_LINE_SIZE);
      //strarray[strcount++] = (char*)line;
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
  const unsigned short int line_len = strnlen(strarray[choice], MAX_LINE_SIZE);

  // Set line to a random element in array
  char * line = strarray[choice];

  // Strip newline char
  line = trim_newline(line);

  // Strip a trailing space
  unsigned short int x = strnlen(line, line_len);
  x--;
  if ((line[x] == '\n') || (line[x] == ' '))
    line[x] = '\0';

  // Return the random line
  //return(line);
  return(strndup(line, strnlen(line, line_len)));
}

char * trim_newline(char * string)
{
  // Get the length of string
  unsigned int len = strnlen(string, 50) + 10;

  // Loop through character array
  for (unsigned int i = 0; i < len; i++)
    {
      // Once a newline is found, change it to \0 and break
      if (string[i] == '\n')
	{
	  string[i] = '\0';
	  len = i;
	  break;
	}
    }
  
  // Return new string
  //return(strndup(string, len));
  return(string);
}
