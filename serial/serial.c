#include <stdlib.h>
#include <stdio.h>

#define GRAV_CONST 1.000
#define TIME_STEP 0.5
#define NUM_BODIES 50
#define WORLD_X_SIZE 500.0
#define WORLD_Y_SIZE 500.0

struct body {
  float x_posn;
  float y_posn;
  float mass;
};

struct cosmos {
  int num_bodies;
  float x_world_size;
  float y_world_size;
  struct body * body_list;
  float time_step;
};


void body_free(struct body * b){
  free(b);
  return;
}

//cosmos_init initializes a cosmos with parameters as given
struct cosmos * cosmos_init(float ts, int numbodies, float xsize, float ysize){

  struct cosmos * p;
  p = malloc(sizeof(struct cosmos));
  if(p == NULL){
    printf("error in malloc");
    exit(1);
  }
  p->num_bodies = numbodies;
  p->time_step = ts;
  p->x_world_size = xsize;
  p->y_world_size = ysize;
  p->body_list = malloc(numbodies * sizeof(struct body));
  if(p->body_list == NULL){
    printf("error in malloc");
    exit(1);
  }
  
  return p;

}

//returns body k in cosmos c
struct body * get_body(struct cosmos * c, int k){
  return (&(c->body_list[k]));
}

//cosmos_free frees a cosmos
void cosmos_free( struct cosmos * c ){
    //free each body first
  free(c->body_list);
  free(c);
  return;
}

//get_bodies fills the cosmos with bodies. How we're getting the bodies is very unclear at this point
//we may take away the parameter n, which tells how many bodies to fetch
struct cosmos * get_cosmos( int n){
  struct cosmos * c;
  c = cosmos_init(TIME_STEP, n, WORLD_X_SIZE, WORLD_Y_SIZE); 


  for(int i = 0; i < n; i++){

    struct body * b;
    b = get_body(c,i);
    //these are filler values
    b->x_posn = 2.0;
    b->y_posn = 2.0;
    b->mass = 1.0;
  }
   
  return c;

}

//prints a body
void print_body( struct body * b){
  
  printf("body x-position: %f, body y-position: %f\n", b->x_posn, b->y_posn);
  printf("body mass: %f\n", b->mass);

  return;
}

//prints the given cosmos;
void print_cosmos( struct cosmos * c ){
  
  printf("cosmos x-size: %f, cosmos y-size: %f\n", c->x_world_size, c->y_world_size);
  printf("time step: %f\n", c->time_step);
  printf("number of bodies: %i\n", c->num_bodies);
  for(int i = 0; i < c->num_bodies; i++){
    print_body( get_body(c,i) );
  }
  return;

}

int main(){

  printf("hello, world\n");

  struct cosmos * p_my_cosmos;

  p_my_cosmos = get_cosmos( NUM_BODIES );

  print_cosmos( p_my_cosmos );
  
  cosmos_free( p_my_cosmos );

  return 0;

}
