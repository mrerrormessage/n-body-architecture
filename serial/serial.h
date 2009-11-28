#ifndef _SERIAL_H_
#define _SERIAL_H_
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "cosmos.h"

#define GRAV_CONST 1.000
#define TIME_STEP 0.5
//#define NUM_BODIES 50
//#define WORLD_X_SIZE 500.0
//#define WORLD_Y_SIZE 500.0
#define GRAV_CONSTANT 6.67428e-11


//calculates the force interactions between two points, updating both of them
void point_update_compute(struct body * b1, struct body * b2);

//once a point has been updated with all other points, it then has it's
//movement calculated based on the sum of all forces
void calc_movement(struct cosmos * c, struct body * b);

//one iteration of the n_body
void simple_n_body_iter ( struct cosmos * c );

//n-body simple. 
void simple_n_body ( struct cosmos * c, int steps );

void compute_world ( struct cosmos * c, int steps, char * outfilename);

#endif
