#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

int main(int argc, char** argv){
  assert(argc == 2);
  FILE* fp = fopen(argv[1], "r");
  assert(fp != NULL);

  char* line = NULL;
  size_t len = 0;
  ssize_t read;

  int errcount = 0;

  while((read = getline(&line, &len, fp)) != -1){
    if(strstr(line, "not found") != NULL){
      errcount++;
    }
  }
  if(errcount > 0){
    printf("A total of %d queries failed.\n", errcount);
  }
  else{
    printf("No queries failed.\n");
  }
  if(line){free(line);}
  fclose(fp);
  return 0;
}
