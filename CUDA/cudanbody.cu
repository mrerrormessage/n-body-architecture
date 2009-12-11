#include "cudanbody.h"
#include <cuda.h>
#include <stdio.h>

__global__ void nBodyCompute(struct cosmos * c, struct body * b);

int main(int argc, char ** argv)
{
  
  cudaSetDevice(0);
  cudaSetDeviceFlags(cudaDeviceMapHost);
  int num_steps;
  if(argc <= 1){
    num_steps = 200;
  } else {
    num_steps = atoi(argv[1]);
  }

/*  struct cosmos * c, * c_d;
  struct body * b, * b_d;
  cudaHostAlloc((void **)&c, sizeof(struct cosmos), cudaHostAllocMapped);
  c = read_cosmos("test.dat");
  cudaHostAlloc((void **)&b, sizeof(struct body) * c->num_bodies, cudaHostAllocMapped);
  for(int i = 0; i < c->num_bodies; i++){
    memcpy(b, c->body_list, sizeof(struct body) * c->num_bodies);
  }

  print_cosmos(c);

  cudaHostGetDevicePointer((void **)&b_d, (void *)b, 0);
  cudaHostGetDevicePointer((void **)&c_d, (void *)c, 0);

*/
  //copy bodies to the device

  struct cosmos * ret_c = (struct cosmos *) malloc(sizeof(struct cosmos));
  struct cosmos * c = read_cosmos("test.dat");
  struct body * ret_b = (struct body *) malloc(sizeof(struct body) * c->num_bodies);
  struct cosmos * c_d; 
  struct body * b_d;
  

  cudaMalloc((void **) &b_d, sizeof(struct body) * c->num_bodies);
  cudaMalloc((void **) &c_d, sizeof(struct cosmos));
  print_cosmos(c);
/*
  const float test_f[] = {1.0};
  float * test_f_p = (float *) malloc(sizeof(float));
  float * test_f_d; 
  cudaMalloc((void **) &test_f_d, sizeof(float));
  cudaMemcpy((void *)test_f_d, (void *)test_f, sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy((void *)test_f_p, (void *)test_f_d, sizeof(float), cudaMemcpyDeviceToHost);
  printf("test returned: %f\n", test_f_p[0]);
*/

//  for(int i = 0; i < c->num_bodies; i++){
  cudaMemcpy(b_d, c->body_list, sizeof(struct body) * c->num_bodies, cudaMemcpyHostToDevice);    
//  }

  c->body_list = b_d;
  //copy cosmos to device
  cudaMemcpy(c_d, c, sizeof(struct cosmos), cudaMemcpyHostToDevice);  
  
  printf("beginning cuda n-body...\n");

  nBodyCompute<<<num_steps, c->num_bodies>>>(c_d, b_d);
//  cudaThreadSynchronize();
  printf("after computation...\n");

  //once we return, we need to copy the memory back and examine it. 
  cudaMemcpy(ret_c, c_d, sizeof(struct cosmos), cudaMemcpyDeviceToHost);
  printf("copied cosmos, %i bodies\n", ret_c->num_bodies);

  cudaMemcpy(ret_b, b_d, sizeof(struct body) * ret_c->num_bodies, cudaMemcpyDeviceToHost);
  printf("copied bodies\n");
  ret_c->body_list = ret_b;
  print_cosmos(ret_c);


/*
  printf("after computation\n");

  //once we return, we need to copy the memory back and examine it. 
  cudaMemcpy(ret_c, c_d, sizeof(struct cosmos), cudaMemcpyDeviceToHost);
  printf("copied cosmos, %i bodies\n", ret_c->num_bodies);

  cudaMemcpy(ret_b, b_d, sizeof(struct body) * ret_c->num_bodies, cudaMemcpyDeviceToHost);
  printf("copied bodies\n");

  ret_c->body_list = ret_b; */
  cudaFree(b_d);
  cudaFree(c_d);
  cudaThreadExit();

  return 0;
}

__global__ void nBodyCompute(struct cosmos * c, struct body * blist){
 
  int i_am = threadIdx.x;
  struct body * b = &(blist[i_am]);

  //compare with all higher bodies
  for(int i = i_am + 1; i < c->num_bodies; i++){
    point_update_compute(b, &(blist[i]) , GRAV_CONST );
  }
  
  //make sure all threads are synchronized before continuing
  __syncthreads();
  calc_movement(b, c->time_step); 
}


