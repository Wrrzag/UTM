#include "turing_machine.h"

#include <stdio.h>
#include <stdlib.h>

#define INITIAL_STATES 5

/* Private declarations */
int parse_tm_file(const char*, state**);
int states_added = 0;
int max_states = INITIAL_STATES;

/* Function implementation */
int init_from_file(char* file_name, state **tm)
{ 
 if((*tm = malloc(sizeof(state)*INITIAL_STATES)) == NULL)
  {
    return -1;
  }
	if(parse_tm_file(file_name, tm) < 0) 
	{
		return -1;
	}
}

int init_from_stdin(state **tm)
{
 if((*tm = malloc(sizeof(state)*INITIAL_STATES)) == NULL)
  {
    return -1;
  }
  if(parse_file(stdin, tm) < 0)
  {
    printf("Input error\n");
    return -1;
  }
}

int destroy(state **tm) /* TODO */
{
  return 0;
}

int add_to_tm(state **tm_ptr, int from, int trigger, int to, int to_write, int mv_c)
{
  state *tm = *tm_ptr;
  int *last = &tm[from].last_added; /* It's tedious to use the complete name all the time (define?)*/

  if(states_added > max_states)
  {
    max_states += INITIAL_STATES;
    if((tm = realloc(tm, max_states)) == NULL)
    {
      return -1;
    }
  }

  if(tm[from].transitions == NULL)
  {
    if((tm[from].transitions = malloc(sizeof(transition))) == NULL)
    {
      return -1;
    }
    *last = 0;
  }
  else
  {
    if((tm[from].transitions = realloc(tm[from].transitions, 1+sizeof(transition)*tm[from].last_added)) == NULL)
    {
      return -1;
    }
  }

  tm[from].transitions[*last].to = to;
  tm[from].transitions[*last].movement = mv_c;
  tm[from].transitions[*last].to_write = to_write+'0';
  (*last)++;
}

/* PRIVATE */
int parse_tm_file(const char *filename, state **tm)
{
		FILE *f;
		if((f = fopen(filename, "r")) == NULL)
		{
		  return -1;
		}
    if(parse_file(f, tm) < 0)
    {
  		fclose(f);
      return -1;
    }
		fclose(f);

		return 0;
}
