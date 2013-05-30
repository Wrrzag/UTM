#include "turing_machine.h"

#include<stdio.h>
#include<stdlib.h>

#define MAX_SIZE 50

int read_tape(char*);
void print_tapes(char*, char*, int);

int main(int argc, char *argv[])
{
  state *tm; /* The pointer will contain [q0 | q1 | q2 | ... | qn], each qn will contain [rule1 | rule2 | ... | rulen ] */
	char opt;
	char *tape_in, *tape_out;
	int q = 0;

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

		printf("TM loaded\n");
  }
  else /* Incorrect arg number */
  {
    printf("Universal Turing Machine \nUsage: %s [file]\nArguments: \n\tfile: text file containing the machine encoding.\n", argv[0]);
  }

	printf("Enter the input tape without spaces.\n");

	if((tape_in = malloc(MAX_SIZE)) == NULL || read_tape(tape_in) < 0 || (tape_out = malloc(MAX_SIZE)) == NULL )
	{
		printf("Error allocating memory\n");
		return -1;
	}

	printf("Press 'a' to run the TM until the end. Press another key to run a step. ");
	/* TODO - actual TM program */
	while((opt = getchar()) != 'a' && run_step(tm, tape_in, tape_out, &q) != -1)
	{
		printf("Running step:\n");
		print_tapes(tape_in, tape_out, q);
	}
	if(opt == 'a')
	{
		printf("Running until the end (warning: the TM could never finish):\n");
		run_all(tm, tape_in, tape_out, &q);
		print_tapes(tape_in, tape_out, q);
	}

	
	/* Memory freeing */
	free(tape_in);
	free(tape_out);
	destroy(&tm);

	return 0; 
}

/* PRIVATE */
int read_tape(char *tape)
{
	char c;
	unsigned int count = 0, max_size = MAX_SIZE;
	while((c = getchar()) != EOF && c != '\n')
	{
		tape[count++] = c;
		
		if(count >= max_size)
		{
			max_size += MAX_SIZE;
			
			if((tape = realloc(tape, max_size)) == NULL)
			{
					return -1;
			}
		}
	}

	tape[count] = '\0';
	return 0;
}

void print_tapes(char *tape_in, char *tape_out, int q)
{
}
