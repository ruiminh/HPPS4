#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char** argv){
  if(argc == 1){
    printf("Usage: %s ./queryfile [./queryfile ./queryfile etc...]\n", argv[0]);
    exit(0);
  }
  printf("\n -----COMPARISON OF TOTAL QUERY TIMES----- \n");
  for(int i = 1; i < argc; i++){
    printf("%s:\t", argv[i]);
    FILE* f = fopen(argv[i], "r");
    if(f == NULL){printf("file not found.\n");}
    else{
      char* line = NULL;
      size_t len = 0;
      ssize_t read;
      while((read = getline(&line,&len,f)) != -1){
	if(strstr(line, "Total") != NULL){
	  printf("%s\n", line);
	}
      }
    fclose(f);
    }
  }
}


