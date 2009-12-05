#include "cudanbody.h"
#include <cuda.h>
#include <stdio.h>


__global__ void nBodyCompute(struct cosmos * c);

int main(int argc, char ** argv)
{
  
  int num_steps;
  if(argc <= 1){
    num_steps = 200;
  } else {
    num_steps = atoi(argv[1]);
  }

  struct cosmos* c = read_cosmos("test.dat");

  //copy bodies to the device
  struct cosmos * c_d;
  struct body * b_d;
  struct body * temp = c->body_list;
  cudaMalloc((void **) &b_d, sizeof(struct body) * c->num_bodies);
  print_cosmos(c);
  c->body_list = b_d;
  for(int i = 0; i < c->num_bodies; i++){
    cudaMemcpy(b_d, temp, sizeof(struct body) * c->num_bodies, cudaMemcpyHostToDevice);
    
  }

  //copy cosmos to device
  cudaMalloc((void **) &c_d, sizeof(struct cosmos));
  cudaMemcpy(c_d, c, sizeof(struct cosmos), cudaMemcpyHostToDevice);
  

  printf("beginning cuda n-body...\n");

  nBodyCompute<<<num_steps, c->num_bodies>>>(c_d);

  
  return 0;
}

__global__ void nBodyCompute(struct cosmos * c){

  //get the appropriate body
  //since there is a thread launched for each body, this means that 
  //the id of our particular body is equivalent to the thread id. 
  struct body * b = get_body(c, threadIdx.x);

  //compare with all higher bodies
  for(int i = 0; i < 1; i++){
    point_update_compute(b, get_body(c,i) , GRAV_CONSTANT );
  }
  
  //make sure all threads are synchronized before continuing
  __syncthreads();
  calc_movement(b, c->time_step);
}
