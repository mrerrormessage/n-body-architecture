#ifndef _SERIAL_H_
#define _SERIAL_H_
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


#define GRAV_CONST 1.000
#define TIME_STEP 0.5
#define NUM_BODIES 50
#define WORLD_X_SIZE 500.0
#define WORLD_Y_SIZE 500.0
#define GRAV_CONSTANT 6.67428e-11

struct body {
  float x_posn;
  float y_posn;
  float x_velocity;
  float y_velocity;
  float mass;
};

struct cosmos {
  int num_bodies;
  float x_world_size;
  float y_world_size;
  struct body * body_list;
  float time_step;
};


//cosmos_init initializes a cosmos with parameters as given
struct cosmos * cosmos_init(float ts, int numbodies, float xsize, float ysize);

//returns body k in cosmos c
struct body * get_body(struct cosmos * c, int k);

//cosmos_free frees a cosmos
void cosmos_free( struct cosmos * c );

//get_bodies fills the cosmos with bodies. How we're getting the bodies is very unclear at this point
//we may take away the parameter n, which tells how many bodies to fetch
//this is a dummy test function to give a sample n-body cosmos - a trivial 
//cosmos of n bodies
struct cosmos * get_cosmos( int n );

//I couldn't get the math min to work, so I used this
int min( int x, int y );

//gets the next good line, checking for EOF and ignoring whitespace and commented lines
void getnextgoodline( char * s, int n, FILE * f);

//returns a string in chunks
void chunk_string(char * s, char * delimiters, char ** result, int * numresults);

//code to read a particular body from a file. 
void read_body( struct body * b, FILE * s);

//reads a cosmos in from a filename
struct cosmos * read_cosmos( const char * filename );

//prints a body
void print_body( struct body * b);

//prints the given cosmos;
void print_cosmos( struct cosmos * c );

//gets the sign of a number
float sign( float f );

void calc_gravity (struct cosmos * c, struct body * b);

//one iteration of the n_body
void simple_n_body_iter ( struct cosmos * c );

//n-body simple. 
void simple_n_body ( struct cosmos * c, int steps );


#endif
