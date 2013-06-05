#include "turing_machine.h"

#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
  turing_machine *tm = NULL;
	char opt;
	int q = 0, pos_in = 0, res;

  if(argc < 2) /* No args: input from stdin */
  {
		printf("Please input the TM's codification. To end press end-of-input.\n");
    if(init_from_stdin(&tm) < 0)
	  {
		  printf("Error parsing the file\n");
		  return -1;
	  }
  }
  else if(argc == 2) /* One arg: input from file */
  {
		printf("Loading TM...\n");

  	if(init_from_file(argv[1], &tm) < 0) 
	  {
		  printf("Error parsing the file\n");
		  return -1;
	  }

		printf("TM '%s' loaded\n", argv[1]);
  }
  else /* Incorrect arg number */
  {
    printf("Universal Turing Machine \nUsage: %s [file]\nArguments: \n\tfile: text file containing the machine encoding.\n", argv[0]);
    return -1;
  }

	printf("Enter the input tape without spaces.\n");

	if(init_tape(&(tm->tape)) < 0 || read_tape(tm->tape) < 0 )
	{
		printf("Error allocating memory\n");
		return -1;
	}

	printf("Press 'a' to run the TM until the end. Press another key to run a step. ");
  print_tape(*(tm->tape), 0);
	while((opt = getchar()) != 'a')
	{
			printf("Running step:");
			res = run_step(tm, &pos_in, &q);
			print_tape(*(tm->tape), pos_in);

			if(res <= 0)
			{
				break;	
			}
	}
	if(opt == 'a')
	{
		printf("Running until the end (warning: the TM could never finish)...\n");
		run_all(tm, &pos_in, &q);
		print_tape(*(tm->tape), pos_in);
	}

	
	/* Memory freeing */
	destroy_tm(&tm);

	return 0; 
}
