/** parser.h
*
* Contains the definitions of the methods used to parse a TM file.
*
* Author: Marc Piñol <marcpinolpueyo@gmail.com>
* Version: 0.20
* License: GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
*
*/

#ifndef _PARSER_
#define _PARSER_
#include "turing_machine.h"
#include <stdio.h>
#endif

/** Parses the TM file and adds the information to the TM. The file can be 
* in any format accepted by the parser but it has to add the symbols
* to be used, the transitions and the final states (if any) to the TM.
*
* Params:
*		A pointer to the FILE to get the info from
*		A pointer to the TM to add the info to
*
*	Returns:
*		PARSING_ERROR if the format of the file is wrong
*		ALLOC_ERROR if the TM has had a problem allocating
*		OK if all has gone well
*
*/
int parse_file(FILE*, turing_machine*);
