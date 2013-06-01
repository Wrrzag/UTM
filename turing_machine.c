#include "turing_machine.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Private declarations */
int parse_tm_file(const char*, turing_machine**);

/* Function implementation */
int init_from_file(const char* file_name, turing_machine **tm)
{
	if(((*tm) = malloc(sizeof(turing_machine))) == NULL)
  {
    return -1;
  }
	if(((*tm)->states = malloc(sizeof(state)*INITIAL_STATES)) == NULL)
  {
    return -1;
  }
	if(init_tape(&((*tm)->tape)) < 0)
  {
    return -1;
  }
	if(parse_tm_file(file_name, tm) < 0) 
	{
		return -1;
	}

	(*tm)->state_num = 0;
	(*tm)->max_states = INITIAL_STATES;
	return 0;
}

int init_from_stdin(turing_machine **tm)
{
	if((*tm = malloc(sizeof(turing_machine))) == NULL)
  {
    return -1;
  }
	if(((*tm)->states = malloc(sizeof(state)*INITIAL_STATES)) == NULL)
  {
    return -1;
  }
	if(init_tape(&((*tm)->tape)) < 0)
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

int destroy_tm(turing_machine **tm) /* TODO */
{
  return 0;
}

int destroy_tape(tape *tape) /* TODO */
{
	return 0;
}

int add_to_tm(turing_machine **tm_ptr, const int from, const char trigger, const int to, const char to_write, const int mv_c)
{
  turing_machine *tm = *tm_ptr;
  int *last = &(tm->states[from].last_added); /* It's tedious to use the complete name all the time */

  if(tm->state_num > tm->max_states || to > tm->max_states)
  {
    tm->max_states += INITIAL_STATES;
		state *tmp_state_ptr;
    if((tmp_state_ptr = malloc(sizeof(state)*(tm->max_states))) == NULL || (tm->states = memcpy(tmp_state_ptr, tm->states, tm->max_states)) == NULL)
    {
      return -1;
    }
  }

  if(tm->states[from].transitions == NULL)
  {
    if((tm->states[from].transitions = malloc(sizeof(transition))) == NULL)
    {
      return -1;
    }
    *last = 0;
		tm->state_num++;
  }
  else
  {
		transition *tmp_trans_ptr;
    if((tmp_trans_ptr = malloc(sizeof(transition)*((*last)+1))) == NULL || (tm->states[from].transitions = memcpy(tmp_trans_ptr, tm->states[from].transitions, sizeof(transition)*((*last)+1))) == NULL)
    {
      return -1;
    }
  }

  tm->states[from].transitions[*last].to = to;
  tm->states[from].transitions[*last].in_tape = trigger;
  tm->states[from].transitions[*last].to_write = to_write;
  tm->states[from].transitions[*last].movement = mv_c-1;
  (*last)++;

	if(tm->states[to].transitions == NULL)
	{
    if((tm->states[to].transitions = malloc(sizeof(transition))) == NULL)
    {
      return -1;
    }
    tm->states[to].last_added = 0;
		tm->state_num++;
  }

	return 0;
}

/* TODO */
int run_step(const turing_machine *tm, int *pos, int *q)
{
  int found = 0; /* Marks if the state is not found to stop the TM */
  int i;
 
  for(i = 0; i <= tm->states[*q].last_added && !found; i++)
  {  //printf("TRaNS: %c \n", tm[*q].transitions[i].in_tape);
    if(tm->tape->elements[*pos] == tm->states[*q].transitions[i].in_tape)
    {
			if(tm->tape->size <= (*pos)+1) /* The tape could be too small */
			{
				char *elems = malloc(++(tm->tape->size));
				memcpy(elems, tm->tape->elements, tm->tape->size);
				tm->tape->elements = elems;

				tm->tape->elements[(*pos)+1] = '2';
			}
      tm->tape->elements[*pos] = tm->states[*q].transitions[i].to_write; /* Write to tape */
			//printf("TAPE POS: %d, MOVE: %d, Q: %d, TO: %d, WRITTEN: %c, SIZE: %d", *pos, tm[*q].transitions[i].movement, *q , tm[*q].transitions[i].to,tape->elements[*pos], tape->size);
      if((*pos += tm->states[*q].transitions[i].movement) < 0) (*pos)++; /* Move tape position */
      *q = tm->states[*q].transitions[i].to; /* Change state */ 
      found = 1;
    }
  }

  return found ? 1 : 0;
}

int run_all(const turing_machine *tm, int *pos, int *q)
{
	while(run_step(tm, pos, q) > 0);
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
	  if(c == pos)
	  {
			if(tape.elements[c] != '2')
	    	printf(" [%c] ", tape.elements[c]);
			else
				printf(" [ ] ");
	  }
	  else
	  {
			if(tape.elements[c] != '2')
	    	printf("  %c  ", tape.elements[c]);
			else
				printf("     ");
	  }
  }
  printf("]\n");
}

/* PRIVATE */
int parse_tm_file(const char *filename, turing_machine **tm)
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
