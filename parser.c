#include "parser.h"

#include <stdio.h>

typedef enum {
  LEFT,
  NONE,
  RIGHT
} movement_t;

// TODO - TM data structure

int skip_line(FILE *);
int count_zeros(FILE *);
int is_valid(char);

int parse_file(FILE *f)
{
  char c;
  int i, from, to, to_write, trigger, mv_c;
  movement_t movement;
  
  // Skip comments
  while((c = fgetc(f)) != EOF && c == 'c' && skip_line(f) != EOF);

  // Read 4 first 1s  
  for(i=0; i<4 && c != EOF; i++, c = fgetc(f))
  {printf("%c ", c);
    if(c != '1')
    {
      return -1;
    }
  }

  // Read actual TM  
  while(c != EOF)
  {
    ungetc(c, f);
    if(
      (from = count_zeros(f)) < 0 ||
      (trigger = count_zeros(f)) < 0 ||
      (to = count_zeros(f)) < 0 ||
      (to_write = count_zeros(f)) < 0 ||
      (mv_c = count_zeros(f)) < 0
    ) return -1;

    printf(":%d :%d :%d :%d :%d ", from, trigger, to, to_write, mv_c); 

    c = fgetc(f); if(c == EOF || c != '1') return -1; printf("%c ", c);
    c = fgetc(f); if(c == EOF || c != '1') return -1; printf("%c ", c);
    c = fgetc(f); if(c == EOF || c == '1') break; else return -1;
  }printf("%c ", c);

  // Read the last 1 [3 of them already read]
  if(c != '1')
  {
    return -1;
  }

  // Check if there is still more to read and if there is return error
  if(((c = fgetc(f)) != '\n' || c != EOF) && (c = fgetc(f)) != EOF) return -1;

  return 0;
}

int skip_line(FILE *f)
{
  char c;
  while((c = fgetc(f)) != EOF && c != '\n');

  return c == EOF ? EOF : 1;
}

int count_zeros(FILE *f)
{
  int counter = 0;
  char c;

  c = fgetc(f); 
  if(c == EOF) return -1; 
  while(c != '1')
  {
    counter++;
    c = fgetc(f);

    if(c == EOF || !is_valid(c)) return -1;
  }

  return counter > 0 ? counter : -1;
}

int is_valid(char c)
{
	return c == '0' || c == '1' ? 1 : 0;
}
