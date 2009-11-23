#include "serial.c"

int test_sign(){
  int error = 0;
  if( sign(5.0) != 1.0 ){
    error++;
  }
  if( sign(0000.01 != 1.0 )){
    error++;
  }
  if( sign( -575.0) != -1.0 ){
    error++;
  }
  if( sign( 0.0 ) != 0.0){
    error++;
  }
  if(error = 0){
    printf( "sign function passes all tests\n");
  } else {
    printf( "sign function tests with %i errors\n", error);
  }
    
  return error;
}


int test_min(){
  int error;
  if( min(0,0) != 0){
    error++;
  }
  if( min( -1, 0) != 0){
    error++;
  }
  if( min(3, 5) != 3){
    error++;
  }
  if(error = 0){
    printf( "min function passes all tests\n");
  } else {
    printf( "min function tests with %i errors\n", error);
  }
  return error;

}
