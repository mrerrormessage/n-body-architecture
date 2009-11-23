#include "serial.h"


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

//gets the next good line, checking for EOF and ignoring whitespace and commented lines
void getnextgoodline( char * s, int n, FILE * f){
  fgets(s, n, f);
  if(NULL == s){
    return;
  }else if(';' == s[0] || '\n' == s[0]){
    getnextgoodline( s, n, f);
    return;
  }
}


void chunk_string(char * s, char * delimiters, char ** result, int * numresults){
  int num_iter;
  int i = 0;
  int max_results = *numresults;
  char cp_iter[strlen(s) + 2];
  strcpy(cp_iter, s);
  char * cp_temp;
  char * cp_holder;
  cp_temp = cp_iter;
  while(cp_iter[0] != '\n' && max_results > i){
    num_iter = 0;
    num_iter = strcspn(cp_temp, delimiters);
    cp_holder = malloc(sizeof(char) * num_iter + 1);
    if(NULL == cp_holder){
      printf("malloc error\n");
      exit(0);
    }
    //copy the string into the temp string, then set the result pointer to point to that string
    strncpy(cp_holder, s, num_iter);    
    result[i] = cp_holder; 
    
    i++;
    cp_temp = &(cp_temp[++num_iter]);
    
  }
  *numresults = i;
}

void read_body( struct body * b, FILE * s){
  char thisline[80];
  char *tline;
  int num_iter = 0;
  char mass_string[20];
  char x_posn_string[20];
  char y_posn_string[20];
  long mass;
  long x;
  long y;
  getnextgoodline(thisline, 79, s);
  if(thisline == NULL){
    return;
  }
  /*the first number represents mass, the second x posn, the third y posn
  all objects start with zero velocities. All lines starting with semicolons
  are ignored. 
  */
  num_iter = strcspn(thisline, " ");
  //I'm not sure how strncpy works with memory...we'll find out?
  strncpy(mass_string, thisline, min(num_iter, 19));
  tline = &(thisline[num_iter]);
  num_iter = strcspn(tline, " ");
  strncpy(x_posn_string, tline, min(num_iter, 19));
  tline = &(thisline[num_iter++]);
  num_iter = strcspn(tline, " \n");
  strncpy(y_posn_string, tline, min(num_iter, 19));

  //note that due to the way that C likes to convert strings, our values must start off as whole numbers (not such a big deal), then be converted to longs, then to floats
  mass = atol(mass_string);
  x = atol(x_posn_string);
  y = atol(y_posn_string);
  b->mass = (float)mass;
  b->x_posn = (float)x;
  b->y_posn = (float)y;
  return;
}

/*
struct cosmos * read_cosmos( const char * filename ) {

  FILE * fp;
  char * temp_cp;
  char xsize[20];
  char ysize[20];
  char timestep[20];
  char num_objects[20];
  fp = fopen( filename, "r" );
  if(NULL == fp){
    printf("unable to open file %s \n", filename);
    exit(1);
  }
  char firstline[80];
  getnextgoodline(firstline, 79, fp);
  if( NULL == firstline){
    return NULL;
  } 
  if( '[' == firstline[0]){
    temp_cp = strpbrk( firstline, "0123456789"); 
    if(NULL == temp_cp){
      printf("test file in wrong format\n");
      return NULL;
    }
    //will replace with chunk_string, when it's been tested
    num_iter = strcspn( temp_cp, " ");
    strncpy(xsize, temp_cp, num_iter);
    firstline = &(temp_cp[++num_iter])
    

  }else{
    printf("test file in wrong format\n");
    return NULL;
  }
  
  
  for(int i = 0; i < c->num_bodies; i++){

  }
 
  return
}
*/
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


int main(){

  printf("hello, world\n");

  struct cosmos * p_my_cosmos;

  p_my_cosmos = get_cosmos( NUM_BODIES );

  print_cosmos( p_my_cosmos );

  simple_n_body( p_my_cosmos, 2);
  
  cosmos_free( p_my_cosmos );

  return 0;

}
