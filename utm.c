/** utm.c
*
* A universal Turing Machine simulator.
* It creates a TM either from a file or directly from the user. It gets 
* the tape input from the user and perform the calculations
* of the TM.
*
* Author: Marc Piñol <marcpinolpueyo@gmail.com>
* Version: 0.20
* License: GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
*
*/

#include "turing_machine.h"

#include<stdio.h>
#include<stdlib.h>
#include <getopt.h>

void print_help();
void print_version();

int main(int argc, char *argv[])
{
	int file, quiet, opt; /*  Parsing flags  */  
	char *filename = NULL; 

  turing_machine *tm = NULL;
	char opt_tape;
	unsigned long q = 0, pos_in = 0;
  int res;

	file = quiet = opt = 0;
  static struct option long_options[] = {
      {"help",      no_argument,       0,  'h' },
      {"quiet", 		no_argument,       0,  'q' },
      {"file",    	required_argument, 0,  'f' },
      {"version",   no_argument,	     0,  'v' },
      {0,           0,                 0,  0   }
  };

  while ((opt = getopt_long(argc, argv, "hqf:v", long_options, NULL )) != -1) 
	{
    switch (opt) 
		{
		  case 'h':
				print_help();	
				return 0;
				break;
			case 'v': 
				print_version();
				return 0;
	      break;
		  case 'q': 
					quiet = 1;
		      break;
		  case 'f': 
				file = 1;
				filename = optarg;
	      break;
		  default: 
				print_help(); 
	      return -1;
		}
  }

	if(file)
  {
		printf("Loading TM...\n");

  	if(init_from_file(filename, &tm) < 0) 
	  {
		  printf("Error parsing the file\n");
		  return -1;
	  }

		printf("TM '%s' loaded\n", filename);
  }
	else
	{
		printf("Please input the TM's codification. To end press end-of-input.\n");
    if(init_from_stdin(&tm) < 0)
	  {
		  printf("Error parsing the file\n");
		  return -1;
	  }
  }

	printf("Enter the input tape without spaces.\n");

	if(read_tape(tm) < 0 )
	{
		printf("Error allocating memory\n");
		return -1;
	}

	printf("Press 'a' to run the TM until the end. Press another key to run a step. ");
  if(!quiet) print_tape(tm, 0);
	while((opt_tape = getchar()) != 'a')
	{
			res = run_step(tm, &pos_in, &q);
			
			if(!quiet)
			{
				printf("Running step:");
				print_tape(tm, pos_in);
			}

			if(res == TRANSITION_NOT_FOUND || res == END_STATE)
			{
				break;	
			}
	}
	if(opt_tape == 'a')
	{
		printf("Running until the end (warning: the TM could never finish)...\n");
		res = run_all(tm, &pos_in, &q);
		if(!quiet) print_tape(tm, pos_in);
	}

	if(res == END_STATE)
  {
    printf("\nYES\n");
  }

	/* Memory freeing */
	destroy_tm(&tm);

	return 0; 
}

/* PRIVATE */
void print_help()
{
	printf("utm: a Universal Turing Machine. \n\
Arguments: \n\
	[-f filename] [--filename filename] Gets the TM codification from the file \n\
	[-q] [--quiet] Doesen't output the tape \n\
	[-v] [--version] Shows the program's version and info \n\
	[-h] [--h] Shows this help \n\n\
Report bugs to: marcpinolpueyo@gmail.com \n\
utm home page: <https://github.com/Wrrzag/UTM>\n");
}

void print_version()
{
	printf("utm 0.20 \n\
Copyright (C) 2013 Marc Piñol Pueyo \n\
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html> \n\
This is free software: you are free to change and redistribute it. \n\
There is NO WARRANTY, to the extent permitted by law.\n");
}
