/*
 *  	Execute.h
 *  	Purpose:  Interface for the execute module. Used
 *	          to execute an instruction cycle for a UM
 * 				         	  
 *  	Date:     4/14/2016 
 *      By:	  Ryan Schneiderman and Matthew Epstein
 */

#ifndef EXECUTE_H
#define EXECUTE_H

#include <stdlib.h>
#include <stdio.h>

#include "opmemory.h"

void execute(Mem_T memory);

#endif