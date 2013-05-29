#include "parser.h"

#include<stdio.h>
#include<stdlib.h>

#define MAX_SIZE 50

int parse_tm_file(const char*);
int read_tape(char*);

int main(int argc, char *argv[])
{
  if(argc < 2) /* No args: input from stdin */
  {
		printf("Please input the TM's codification. To end press end-of-input.\n");
    if(parse_file(stdin) < 0)
    {
      printf("Input error\n");
      return -1;
    }
  }
  else if(argc == 2) /* One arg: input from file */
  {
		printf("Loading TM...\n");

		if(parse_tm_file(argv[1]) < 0) 
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
	char* tape;
	if((tape = malloc(MAX_SIZE)) == NULL || read_tape(tape) < 0)
	{
		printf("Error allocating memory\n");
		return -1;
	}

	/* TODO - actual TM program */

	free(tape);
	return 0; 
}

int parse_tm_file(const char *filename)
{
		FILE *f;
		if((f = fopen(filename, "r")) == NULL)
		{
		  return -1;
		}
    if(parse_file(f) < 0)
    {
  		fclose(f);
      return -1;
    }
		fclose(f);

		return 0;
}

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
