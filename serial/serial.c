#include "serial.h"


//splits a string into chunks based on the delimiter, resultc is max results
//returns number of results retrieved
int chunk_string(char * s, char * delim, int resultc, char ** res){
  if(0 == resultc || '\n' == s[0] || '\0' == s[0]){
    return 0;
  }
  int ch_cnt = strcspn( s, delim );
  res[0] = malloc( (ch_cnt + 1) * sizeof(char));
  if(NULL == res[0]){
    printf("error in malloc\n");
    exit(0);
  }
  strncpy(res[0], s, (ch_cnt));
  if('\0' == s[ch_cnt] || '\n' == s[ch_cnt] ){
    return 1;
  } 
  return 1 + chunk_string( &(s[ch_cnt + 1]), delim, --resultc, &(res[1]));
}

//frees chunk-based arrays
void free_chunks(char ** c, int num){
  
  for(int i = 0; i < num; i++){
    free(c[i]);
  }

  return;
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
    b->x_velocity = 0.0;
    b->y_velocity = 0.0;
  }
   
  return c;

}

int min( int x, int y ){
  if (x < y){
    return x; 
  }
  return y;
}

//converts a string to a floating point number
float string_to_float( char * s ){
  char * nums[2];
  int resct = chunk_string( s, ".", 2, nums);
  if(0 == resct || 2 < resct){
    printf("string: \"%s\" is not a float!\n", s);
    return 0.0;
  }
  if(1 == resct){
    return (float)atol(nums[0]);
  }
  return (float)atol(nums[0]) + (atol(nums[1]) / pow(10, (int)strlen(nums[1])) ) ;
}

//gets the next good line, checking for EOF and ignoring whitespace and commented lines
int getnextgoodline( char * s, int n, FILE * f){
  fgets(s, n, f);
  if(NULL == s){
    return 1;
    //semicolon escapes commented lines
  }else if(';' == s[0] || '\n' == s[0]){
    return getnextgoodline( s, n, f);    
  }
  return 0;
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
  
  return;
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

float sign( float f ){
  if (f == 0.0){
    return 0.0;
  }
  if (f < 0.0){
    return -1.0;
  }
  return 1.0;
}

void calc_gravity (struct cosmos * c, struct body * b){
  float xdiff;
  float ydiff;
  float dist_sq;
  float force;
  float dist;
  float theta;
  //  float x_accel = 0.0;
  //  float y_accel = 0.0;
  float x_force_sum = 0.0;
  float y_force_sum = 0.0;
  for(int i = 0; i < c->num_bodies; i++){
    struct body * b2;
    b2 = get_body(c, i);

    if( b2 != b ){
      xdiff = b->x_posn - b2->x_posn;
      ydiff = b->y_posn - b2->y_posn;
      //note that this is actually distance squared, but that's the term we need anyhow
      dist_sq = pow( xdiff, 2.0) + pow(ydiff, 2.0);
      force = GRAV_CONST * (b->mass) * (b2->mass) / dist_sq;
      //now we need to split it out into two separate vectors
      dist = sqrt(dist_sq);
      theta = atan(ydiff/xdiff);
      x_force_sum += force * sign(xdiff) * cos(theta);
      y_force_sum += force * sign(ydiff) * sin(theta);
    }
  }
  
  //velocity = acceleration * time
  //v = v_0 + a * t
  b->x_velocity += (x_force_sum / b->mass) * c->time_step;
  b->y_velocity += (y_force_sum / b->mass) * c->time_step;
  
  //may need to have this divided by time_step, not sure
  //x = x_0 + v * t
  b->x_posn += b->x_velocity * c->time_step;
  b->y_posn += b->y_velocity * c->time_step;

  
  return;
}

//one iteration of the n_body
void simple_n_body_iter ( struct cosmos * c ){

  for(int i = 0; i < c->num_bodies; i++){
    struct body * b; 
    b = get_body( c, i ); 
    calc_gravity( c, b );
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

  printf("beginning serial n-body...\n");
  char infilename[] = "test.dat";
  struct cosmos * p_my_cosmos = read_cosmos( infilename );

  print_cosmos( p_my_cosmos );

  simple_n_body( p_my_cosmos, 2);
  
  cosmos_free( p_my_cosmos );

  return 0;

}
