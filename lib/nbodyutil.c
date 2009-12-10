#include "nbodyutil.h"

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
  res[0][ch_cnt] = '\0';
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

int min( int x, int y ){
  if (x < y){
    return x; 
  }
  return y;
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


