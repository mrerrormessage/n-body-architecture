#include "serial.h"




void point_update_compute(struct body * b1, struct body * b2){

  float xdiff = b1->x_posn - b2->x_posn;
  float ydiff = b1->y_posn - b2->y_posn;
  float dist_sq = pow( xdiff, 2.0) + pow( ydiff, 2.0);
  float force = GRAV_CONST * b1->mass * b2->mass / dist_sq;
  float theta = atan(ydiff/xdiff);
  b1->x_force += force * sign(xdiff) * cos(theta);
  b1->y_force += force * sign(ydiff) * sin(theta);
  //may want these lines to be "+=" we'll find out
  b2->x_force -= force * sign(xdiff) * cos(theta);
  b2->y_force -= force * sign(ydiff) * sin(theta);

  return;
}


void calc_movement(struct cosmos * c, struct body * b){

  //velocity = acceleration * time
  //v = v_0 + a * t
  b->x_velocity += (b->x_force / b->mass) * c->time_step;
  b->y_velocity += (b->y_force / b->mass) * c->time_step;

  //may need to have this divided by time_step, not sure
  //x = x_0 + v * t
  //the '-' here is a hack because we seem to be getting the wrong numbers
  b->x_posn = b->x_posn - (b->x_velocity * c->time_step);
  b->y_posn = b->y_posn - (b->y_velocity * c->time_step);

  return;
}

//one iteration of the n_body
void simple_n_body_iter ( struct cosmos * c ){

  for(int i = 0; i < c->num_bodies; i++){
    struct body * b; 
    b = get_body( c, i ); 
    for(int j = i + 1; j < c->num_bodies; j++){
      struct body * b2;
      b2 = get_body( c, j);
      
      point_update_compute(b, b2);
      
    }

    //calc movement down here for each body that has had all interactions calculated on it
    calc_movement(c, b);
    

  }
  
  return;
}

//n-body simple. 
void simple_n_body ( struct cosmos * c, int steps ){

  for( int i = 0; i < steps ; i++ ){
    simple_n_body_iter ( c );
  }

  return;
}


int main(int argc, char ** argv){
  int num_steps;
  if(argc <= 1){
    num_steps = 200;
  } else {
    num_steps = atoi(argv[1]);
  }

  printf("beginning serial n-body...\n");
  char infilename[] = "test.dat";
  struct cosmos * p_my_cosmos = read_cosmos( infilename );

  print_cosmos( p_my_cosmos );

  simple_n_body( p_my_cosmos, num_steps);

  printf("simulating....\n");

  print_cosmos( p_my_cosmos );

  cosmos_free( p_my_cosmos );

  return 0;

}
