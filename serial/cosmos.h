#ifndef _COSMOS_H_
#define _COSMOS_H_
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nbodyutil.h"

struct body {
  float x_posn;
  float y_posn;
  float x_velocity;
  float y_velocity;
  float x_force;
  float y_force;
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
struct cosmos * cosmos_init( float xsize, float ysize, float ts, int numbodies);

//returns body k in cosmos c
struct body * get_body(struct cosmos * c, int k);

//cosmos_free frees a cosmos
void cosmos_free( struct cosmos * c );

//code to read a particular body from a file. 
void read_body( struct body * b, char * s);

//reads a cosmos in from a filename
struct cosmos * read_cosmos( const char * filename );

//prints a body
void print_body( struct body * b);

//prints the given cosmos;
void print_cosmos( struct cosmos * c );

//get_bodies fills the cosmos with bodies. How we're getting the bodies is very unclear at this point
//we may take away the parameter n, which tells how many bodies to fetch
//this is a dummy test function to give a sample n-body cosmos - a trivial 
//cosmos of n bodies
struct cosmos * get_cosmos( int n );


#endif
