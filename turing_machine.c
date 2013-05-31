#include "turing_machine.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#define INITIAL_STATES 5

/* Private declarations */
int parse_tm_file(const char*, state**);
int states_added = 0;
int max_states = INITIAL_STATES;

/* Function implementation */
int init_from_file(const char* file_name, state **tm)
{ 
 if((*tm = malloc(sizeof(state)*INITIAL_STATES)) == NULL)
  {
    return -1;
  }
	if(parse_tm_file(file_name, tm) < 0) 
	{
		return -1;
	}

	return 0;
}

int init_from_stdin(state **tm)
{
 if((*tm = malloc(sizeof(state)*INITIAL_STATES)) == NULL)
  {
    return -1;
  }
  if(parse_file(stdin, tm) < 0)
  {
    return -1;
  }

	return 0;
}

int destroy(state **tm) /* TODO */
{
  return 0;
}

int add_to_tm(state **tm_ptr, const int from, const int trigger, const int to, const int to_write, const int mv_c)
{
  state *tm = *tm_ptr;
  int *last = &tm[from].last_added; /* It's tedious to use the complete name all the time */

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
    if((tm[from].transitions = realloc(tm[from].transitions, sizeof(transition)*((*last)+1))) == NULL)
    {
      return -1;
    }
  }

  tm[from].transitions[*last].to = to;
  tm[from].transitions[*last].in_tape = trigger;
  tm[from].transitions[*last].movement = mv_c-1;
  tm[from].transitions[*last].to_write = to_write+'0';
  (*last)++;

	return 0;
}

/* TODO */
int run_step(const state *tm, char **tape_in, int *pos_in, int *q)
{
  int found = 0; /* Marks if the state is not found to stop the TM */
  int i;

  for(i = 0; i < tm[*q].last_added+1 && !found; i++)
  {  
    if((*tape_in)[*pos_in]-'0' == tm[*q].transitions[i].in_tape)
    {
      (*tape_in)[*pos_in] = tm[*q].transitions[i].to_write; /* Write to tape */
      if((*pos_in += tm[*q].transitions[i].movement) < 0) (*pos_in)++; /* Move tape position */
      *q = tm[*q].transitions[i].to; /* Change state */ 
      found = 1;
    }
  }

  return found ? 1 : 0;
}

int run_all(const state *tm, char **tape_in, int *pos_in, int *q)
{
	while(run_step(tm, tape_in, pos_in, q) > 0);
	return *q;
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
