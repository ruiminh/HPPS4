#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

int main(int argc, char** argv){

  if(argc != 2){
    printf("Usage: %s ./input_text_file \n", argv[0]);
    exit(0);
  }
  FILE* fp = fopen(argv[1], "r");
  assert(fp != NULL);
  char* line = NULL;
  size_t len;
  ssize_t read = 0;

  while((read = getline(&line,&len,fp)) != -1){
    if(strstr(line, "Query") == NULL
    && strstr(line, "Reading") == NULL
    && strstr(line, "Building") == NULL
    && strstr(line, "Total") == NULL){
      printf("%s", line);
    }
  }
  fclose(fp);
  if(line){free(line);}
}

