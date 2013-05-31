#include "turing_machine.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int init_tape(tape **tape)
{
	if((*tape = malloc(MAX_SIZE*sizeof(tape))) == NULL)
	{
		return -1;
	}

	return 0;
}

int destroy_tm(state **tm) /* TODO */
{
  return 0;
}

int destroy_tape(tape *tape) /* TODO */
{
	return 0;
}

int add_to_tm(state **tm_ptr, const int from, const char trigger, const int to, const char to_write, const int mv_c)
{
  state *tm = *tm_ptr;
  int *last = &tm[from].last_added; /* It's tedious to use the complete name all the time */

  if(states_added > max_states || to > max_states)
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
		states_added++;
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
  tm[from].transitions[*last].to_write = to_write;
  tm[from].transitions[*last].movement = mv_c-1;
  (*last)++;

	if(tm[to].transitions == NULL)
	{
    if((tm[to].transitions = malloc(sizeof(transition))) == NULL)
    {
      return -1;
    }
    tm[to].last_added = 0;
		states_added++;
  }

	return 0;
}

/* TODO */
int run_step(const state *tm, tape *tape, int *pos, int *q)
{
  int found = 0; /* Marks if the state is not found to stop the TM */
  int i;
 
  for(i = 0; i <= tm[*q].last_added && !found; i++)
  {  //printf("TRaNS: %c \n", tm[*q].transitions[i].in_tape);
    if(tape->elements[*pos] == tm[*q].transitions[i].in_tape)
    {
			if(tape->size <= (*pos)+1) /* The tape could be too small */
			{
				char *elems = malloc(++(tape->size));
				memcpy(elems, tape->elements, tape->size);
				tape->elements = elems;
printf("ergh %c ", tape->elements[(*pos)]);

				tape->elements[(*pos)+1] = '2';
			}
      tape->elements[*pos] = tm[*q].transitions[i].to_write; /* Write to tape */
			printf("TAPE POS: %d, MOVE: %d, Q: %d, TO: %d, WRITTEN: %c, SIZE: %d", *pos, tm[*q].transitions[i].movement, *q , tm[*q].transitions[i].to,tape->elements[*pos], tape->size);
      if((*pos += tm[*q].transitions[i].movement) < 0) (*pos)++; /* Move tape position */
      *q = tm[*q].transitions[i].to; /* Change state */ 
      found = 1;
    }
  }

  return found ? 1 : 0;
}

int run_all(const state *tm, tape *tape, int *pos, int *q)
{
	while(run_step(tm, tape, pos, q) > 0);
	return *q;
}

int read_tape(tape *tape)
{
	char c;
	unsigned int count = 0, max_size = MAX_SIZE;
	while((c = getchar()) != EOF && c != '\n')
	{
		tape->elements[count++] = c;
		
		if(count >= max_size)
		{
			max_size += MAX_SIZE;
			
			if((tape = realloc(tape, max_size)) == NULL)
			{
					return -1;
			}
		}
	}

	tape->elements[count] = '\0';
	tape->size = count;

	return 0;
}

void print_tape(tape tape, int pos)
{
  printf("\n[");
  int c;
  for( c = 0; c < tape.size; c++)
  {
    if(c==pos)
    {
      printf(" [%c] ", tape.elements[c]);
    }
    else
    {
      printf("  %c  ", tape.elements[c]);
    }
  }
  printf("]\n");
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
