#include <CL/cl.h>
#include "cosmos.h"
#include "cosmos.h"

//returns body k in cosmos c
__device__ struct body * get_body(struct cosmos * c, int k);

//calculates the gravitational forces between two points
__device__ void point_update_compute(struct body * b1, struct body * b2, const float gravity);

//calculates the movement that a given point should take in a given step. Should be called after a
//point has been point_udate_computed with every other point
__device__ void calc_movement(struct body * b, const float time_step);




//returns body k in cosmos c
struct body * get_body(struct cosmos * c, int k){
  return (&(c->body_list[k]));
}

float sign( float f ){
  if (f == 0.0){
    return 0.0;
  }
  if (f < 0.0){
    return -1.0;
  }
  return 1.0;
}

void point_update_compute(struct body * b1, struct body * b2, float gravity){

  float xdiff = b1->x_posn - b2->x_posn;
  float ydiff = b1->y_posn - b2->y_posn;
  float dist_sq = (xdiff*xdiff) + (ydiff*ydiff);
  float force = gravity * b1->mass * b2->mass / dist_sq;
  float theta = atan(ydiff/xdiff);
  b1->x_force += force * sign(xdiff) * cos(theta);
  b1->y_force += force * sign(ydiff) * sin(theta);
  //may want these lines to be "+=" we'll find out
  b2->x_force -= force * sign(xdiff) * cos(theta);
  b2->y_force -= force * sign(ydiff) * sin(theta);

  return;
}


void calc_movement(struct body * b,const float time_step){

  //velocity = acceleration * time
  //v = v_0 + a * t
  b->x_velocity += (b->x_force / b->mass) * time_step;
  b->y_velocity += (b->y_force / b->mass) * time_step;

  //may need to have this divided by time_step, not sure
  //x = x_0 + v * t
  //the '-' here is a hack because we seem to be getting the wrong numbers
  b->x_posn = b->x_posn - (b->x_velocity * time_step);
  //or maybe the '+' here is the hack. We'll find out
  b->y_posn = b->y_posn + (b->y_velocity * time_step);

  return;
}

__kernel void nbodycompute( __global const struct cosmos * c, __global struct body * b){

  //find the id of the particle you should be working on
  int i_am = get_global_id(0);
  
  //fetch the body that we're working with
  b1 = &(b[i_am]);

  for(int i = i_am + 1; i < c->num_bodies; i++){
    point_update_compute(b1, &(b[i]), GRAV_CONST);
  } 

  //sync before we update everything
  barrier(CLK_GLOBAL_MEM_FENCE);
  calc_movement(b1, c->time_step);
}
