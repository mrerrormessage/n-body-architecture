#ifndef _NBODYUTIL_H_
#define _NBODYUTIL_H_
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


//gets the next good line, checking for EOF and ignoring whitespace and commented lines
int getnextgoodline( char * s, int n, FILE * f);

//splits a string into chunks based on the delimiter, resultc is max results
//returns number of results retrieved
int chunk_string(char * s, char * delim, int resultc, char ** res);

//frees chunked arrays
void free_chunks( char ** c , int num);

//converts a string to a floating point number
float string_to_float( char * s );


//gets the sign of a number
float sign( float f );

//I couldn't get the math min to work, so I used this
int min( int x, int y );

#endif
