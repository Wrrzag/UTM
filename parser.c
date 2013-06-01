#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

/* Private declarations */
int skip_line(FILE *);
int count_zeros(FILE *, char*);
int is_valid(char);

/* Public functions implementation */
int parse_file(FILE *f, turing_machine **tm)
{
  char c;
  int i, from, to, to_write, trigger, mv_c, sym_count, syms;
  
  /* Skip comments */
  while((c = fgetc(f)) != EOF && c == 'c' && skip_line(f) != EOF);

	if(c == '{') /* User defined symbols */
	{
		sym_count = syms = 0;
		while((c = fgetc(f)) != EOF && c != '}')
		{
			if(sym_count++%2 && c != ',')	
			{
				return -1;
			}
			else
			{
				/*add_symbol(symbols, syms++, c);*/
			}
		}
		fgetc(f); /*trim whitespace */
		c = fgetc(f);
	}
	else
	{
		/* Standard {0, 1, b} */
		/* symbols = ['0', '1', 'b'] */
	}

  /* Read 4 first 1s */ 
  for(i=0; i<4 && c != EOF; i++, c = fgetc(f))
  {
    if(c != '1')
    {
      return -1;
    }
  }

  /* Read actual TM */  
  while(c != EOF)
  {
    ungetc(c, f);
    if(
      (from = count_zeros(f, &c)) < 0 ||
      (trigger = count_zeros(f, &c)) < 0 ||
      (to = count_zeros(f, &c)) < 0 ||
      (to_write = count_zeros(f, &c)) < 0 ||
      (mv_c = count_zeros(f, &c)) < 0
    ) return -1;

    if((add_to_tm(tm, --from, (--trigger)+'0', --to, (--to_write)+'0', --mv_c)) < 0)
    {
      return -1;
    }

    if(c == EOF || c != '1') return -1; 
    c = fgetc(f); if(c == EOF || c != '1') return -1;
    c = fgetc(f); if(c == EOF || c == '1') break; else if(c != '0') return -1;
  }

  /* Read the last 1 (3 of them already read) */
  if((c = fgetc(f)) != '1')
  {
    return -1;
  }

  /* Check if there is still more to read and if there is return error */
  if(((c = fgetc(f)) != '\n' || c != EOF) && (c = fgetc(f)) != EOF) return -1;

  return 0;
}

/* PRIVATE */
int skip_line(FILE *f)
{
  char c;
  while((c = fgetc(f)) != EOF && c != '\n');

  return c == EOF ? EOF : 1;
}

int count_zeros(FILE *f, char *c)
{
  int counter = 0;
  //char c;

  *c = fgetc(f); 
  if(*c == EOF) return -1; 
  while(*c != '1')
  {
    counter++;
    *c = fgetc(f);

    if(*c == EOF || !is_valid(*c)) return -1;
  }

  return counter > 0 ? counter : -1;
}

int is_valid(char c)
{
	return c == '0' || c == '1' ? 1 : 0;
}
