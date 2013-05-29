#include "parser.h"

#include<stdio.h>
#include<stdlib.h>


int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    //read from stdin
  }
  else if(argc == 2)
  {
    if(parse_file(argv[1]) < 0)
    {
      printf("Error parsing the file\n");
      exit(1);
    }
  }
  else
  {
    // ERROR
    printf("Universal Turing Machine \nUsage: %s [file]\nArguments: \n\tfile: text file containing the machine encoding.\n", argv[0]);
  }

}
