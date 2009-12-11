#include "cosmos.cu"

//utility I/O functions for reading in Cosmoses
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

//splits a string into chunks based on the delimiter, resultc is max results
//returns number of results retrieved
int chunk_string(char * s, char * delim, int resultc, char ** res){
  if(0 == resultc || '\n' == s[0] || '\0' == s[0]){
    return 0;
  }
  int ch_cnt = strcspn( s, delim );
  res[0] = (char *) malloc( (ch_cnt + 1) * sizeof(char));
  if(NULL == res[0]){
    printf("error in malloc\n");
    exit(0);
  }
  strncpy(res[0], s, (ch_cnt));
  res[0][ch_cnt] = '\0';
  if('\0' == s[ch_cnt] || '\n' == s[ch_cnt] ){
    return 1;
  } 
  return 1 + chunk_string( &(s[ch_cnt + 1]), delim, --resultc, &(res[1]));
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

//frees chunk-based arrays
void free_chunks(char ** c, int num){
  
  for(int i = 0; i < num; i++){
    free(c[i]);
  }

  return;
}


struct body * get_body_host(struct cosmos * c, int k){
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
  b->x_velocity = 0.0;
  b->y_velocity = 0.0;
  b->x_force = 0.0;
  b->y_force = 0.0;
  free_chunks(chunk_res, 3);
  return;
}

//cosmos_init initializes a cosmos with parameters as given
struct cosmos * cosmos_init( float xsize, float ysize, float ts, int numbodies){

  struct cosmos * p;
  p = (struct cosmos *) malloc(sizeof(struct cosmos));
  if(p == NULL){
    printf("error in malloc");
    exit(1);
  }
  p->num_bodies = numbodies;
  p->time_step = ts;
  p->x_world_size = xsize;
  p->y_world_size = ysize;
  p->body_list = (struct body *)malloc(numbodies * sizeof(struct body));
  if(p->body_list == NULL){
    printf("error in malloc");
    exit(1);
  }
  
  return p;

}

//get_bodies fills the cosmos with bodies.
//the bodies it fills it with are "dummy" bodies" all at the same place,
//the same mass, etc. 
struct cosmos * get_cosmos( int n){
  struct cosmos * c;
  c = cosmos_init(0.5, n, 500, 500); 


  for(int i = 0; i < n; i++){

    struct body * b;
    b = get_body_host(c,i);
    //these are filler values
    b->x_posn = 2.0;
    b->y_posn = 2.0;
    b->mass = 1.0;
    b->x_velocity = 0.0;
    b->y_velocity = 0.0;
  }
   
  return c;

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
    print_body( get_body_host(c,i) );
  }
  return;

}

//please note that this code has NEVER been compiled or tested
int main(){
// create OpenCL device & context
cl_context hContext;
hContext = clCreateContextFromType(0, CL_DEVICE_TYPE_GPU,
                                   0, 0, 0);

// query all devices available to the context
size_t nContextDescriptorSize;
clGetContextInfo(hContext, CL_CONTEXT_DEVICES,
                 0, 0, &nContextDescriptorSize);
cl_device_id * aDevices = (cl_device_id *)malloc(nContextDescriptorSize);
clGetContextInfo(hContext, CL_CONTEXT_DEVICES,
                 nContextDescriptorSize, aDevices, 0);


const char **kernelcode = {"copy nbody.cl into this string"};
  // create OpenCL device & context
  cl_context hContext;
  hContext = clCreateContextFromType(0, CL_DEVICE_TYPE_GPU,
                                   0, 0, 0);

  // query all devices available to the context
  size_t nContextDescriptorSize;
  clGetContextInfo(hContext, CL_CONTEXT_DEVICES,
                 0, 0, &nContextDescriptorSize);
  cl_device_id * aDevices = (cl_device_id *)malloc(nContextDescriptorSize);
  clGetContextInfo(hContext, CL_CONTEXT_DEVICES,
                 nContextDescriptorSize, aDevices, 0);

  cl_program hProgram;
  hProgram = clCreateProgramWithSource(hContext, 1,
                                     sProg, 0, 0);

  clBuildProgram(hProgram, 
	0, 
	0, 
	0, 
	0, 
	0);


  // create kernel
  cl_kernel krn;
  krn = clCreateKernel(hProgram, 
	"void nbodycompute", 
	0);
  cl_command_queue hCmdQueue;
  CmdQueue = clCreateCommandQueue(hContext, aDevices[0], 0, 0);

  struct cosmos * c = read_cosmos("test.dat");
  struct cosmos * b_res = malloc(sizeof(struct body) * c->num_bodies);

  cl_mem c_d  = clCreateBuffer(hContext,
			       CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                               sizeof(struct cosmos),
                               c,
			       NULL); 

  cl_mem b_d = clCreateBuffer(hContext,
                              CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                              sizeof(struct body) * c->num_bodies,
                              b,
			      NULL);
  
  clSetKernelArg(krn, 0, sizeof(cl_mem), (void *) &c_d)
  clSetKernalArg(krn, 1, sizeof(cl_mem), (void *) &b_d)

  clEnqueueNDRangeKernel(CmdQueue, krn,
			 1, //work dimension
			 0, //global work offset
                         c->num_bodies, //global work size = number of work-items (i.e. threads) in work_dim dimensions that execute the kernel function
                         0, //local work size
                         0, //number of waiting events
                         0, //list of waiting events
                         0); //pointer to some event

//since the world isn't altered, we just need to read the bodies back
  clEnqueueReadBuffer(CmdQueue, b_d, CL_TRUE, 0,
			sizeof(struct body) * c->num_bodies , //size of memory to be copied
			b_res, 0, 0, 0); //more waiting events stuff
 
  c->body_list = b_res;
  print_cosmos(c);
  
  free_cosmos(c);
  clReleaseMemObj(c_d);
  clReleaseMemObj(b_d);

return 0;
}

