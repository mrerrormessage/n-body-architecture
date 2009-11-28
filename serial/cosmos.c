#include "cosmos.h"

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


void read_body( struct body * b, char * s){
  char * chunk_res[3];
  int num_chunks = chunk_string(s , " " , 3, chunk_res);
  if(3 != num_chunks){
    printf("string \"%s\" is not a valid body!\n", s);
  }

  /*the first number represents mass, the second x posn, the third y posn
  all objects start with zero velocities. All lines starting with semicolons
  are ignored. 
  */
  
  b->mass = string_to_float( chunk_res[0] );
  b->x_posn = string_to_float( chunk_res[1] );
  b->y_posn = string_to_float( chunk_res[2] );
  
  free_chunks(chunk_res, 3);
  return;
}

//get_bodies fills the cosmos with bodies.
//the bodies it fills it with are "dummy" bodies" all at the same place,
//the same mass, etc. 
struct cosmos * get_cosmos( int n){
  struct cosmos * c;
  c = cosmos_init(0.5, n, 500, 500); 


  for(int i = 0; i < n; i++){

    struct body * b;
    b = get_body(c,i);
    //these are filler values
    b->x_posn = 2.0;
    b->y_posn = 2.0;
    b->mass = 1.0;
    b->x_velocity = 0.0;
    b->y_velocity = 0.0;
  }
   
  return c;

}

//cosmos_init initializes a cosmos with parameters as given
struct cosmos * cosmos_init( float xsize, float ysize, float ts, int numbodies){

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

//takes a filename, gets a cosmos, viola!
struct cosmos * read_cosmos( const char * filename ) {

  FILE * fp;
  int num_res;
  char * firstlineargs[4];
  fp = fopen( filename, "r" );

  if(NULL == fp){
    printf("unable to open file %s \n", filename);
    exit(1);
  }

  char firstline[80];
  getnextgoodline(firstline, 79, fp);
  if( NULL == firstline || ':' != firstline[0]){
    printf("test file in wrong format\n");
    return NULL;
  }
  num_res = chunk_string( &(firstline[1]), " ", 4 , firstlineargs );
  if(4 != num_res){
    printf("wrong number of arguments on input file!\n");
    return NULL;
  }

  struct cosmos * c = 
    cosmos_init(string_to_float( firstlineargs[0] ), 
		string_to_float( firstlineargs[1] ), 
		string_to_float( firstlineargs[2] ), 
		atoi( firstlineargs[3] ) );

  free_chunks(firstlineargs, 4);

  char line[100];
  for(int i = 0; i < c->num_bodies && (0 == getnextgoodline(line, 99, fp)); i++){
    read_body(  &(c->body_list[i]) , line);
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

