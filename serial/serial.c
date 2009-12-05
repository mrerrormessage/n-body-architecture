#include "serial.h"



//one iteration of the n_body
void simple_n_body_iter ( struct cosmos * c ){

  for(int i = 0; i < c->num_bodies; i++){
    struct body * b; 
    b = get_body( c, i ); 
    for(int j = i + 1; j < c->num_bodies; j++){
      struct body * b2;
      b2 = get_body( c, j);
      
      point_update_compute(b, b2, GRAV_CONSTANT);
      
    }
    //calc movement down here for each body that has had all interactions calculated on it
    calc_movement(b, c->time_step);
    
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
