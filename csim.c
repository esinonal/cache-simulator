#include "cachelab.h"
#include "unistd.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Step0: make our parameters for hit, miss, evictions so we can increment them along way.
int hits=0;
int misses=0;
int evictions=0;
int globalClock=0;

typedef struct theline {
  int valid;
  unsigned long long tag;
  int time;
}line;

typedef struct theset {
  line * lines;
}set;

void accessCache(unsigned long long tagnum, long setnum, int numLines, set* cache, int s){

  set *currentSet;
  line *currentLine;

  int tagmatch;
  int valid;
  int found;
  int currentMin;
  //Go to the set.
  tagmatch=0;
  valid=0;

  currentSet= &cache[setnum];
  currentLine= &((*currentSet).lines[0]);

  for (int i=0; i<numLines; i++){ //Go through all the lines in the set.
    if ((*currentSet).lines[i].tag == tagnum){
      tagmatch=1;
      if ((*currentSet).lines[i].valid == 1){
        valid=1;
        currentLine= &((*currentSet).lines[i]);
      }
    }
  }

  if ((tagmatch==1)&&(valid==1)){ //Case 1: it is found and it is valid.
    hits= hits+1; //Increment hit.
    (*currentLine).time= globalClock; //Say when we accessed the line.
  }
  else {
    found=0;
    for(int i=0; i<numLines; i++){
      if((*currentSet).lines[i].valid == 0){
        currentLine= &((*currentSet).lines[i]);
        found=1;
      }
    }
    if (found==1){ //Case2: there is one with valid bit=0 that we can use.
      misses= misses+1;
      (*currentLine).valid=1;
      (*currentLine).tag=tagnum;
      (*currentLine).time=globalClock;
    }
    else if (found==0){ //Case3: we have to evict.
      misses= misses+1;
      evictions= evictions+1;

      //Get line to evict:
      currentMin= (*currentSet).lines[0].time;
      for(int i=0; i<numLines; i++){
        if ((*currentSet).lines[i].time < currentMin){
          currentMin= (*currentSet).lines[i].time;
        }
      }
      for(int i=0; i<numLines; i++){
        if((*currentSet).lines[i].time==currentMin){
          currentLine= &((*currentSet).lines[i]);
        }
      }
      (*currentLine).tag=tagnum;
      (*currentLine).time=globalClock;
    }
  }
}

void dealWithInstruction(char* string, set* cache, int s, int e, int b){
  if (string[0]!= 'I'){
    printf("\n");
    //Step0: read items from string
    char mode;
    unsigned long long address;
    int d;
    sscanf(string, " %c %10llx,%d", &mode, &address, &d);

    int t = 64-s-b;

    unsigned long long mask= (unsigned long long)(pow(2,t));

    mask= mask-1;
    unsigned long long tagnum= ((address>>b)>>s) & mask;

    long tagAndSetMask= (-1 << b);
    long splusb= s+b;
    long setAndBMask= (pow(2,splusb)-1);
    long setMaskInMiddle= tagAndSetMask&setAndBMask;
    long pickupaddress= setMaskInMiddle&address;
    long setnum= pickupaddress>>b;

    int numLines= e;

    //Step1: deal with the item
    if ((mode == 'L')||(mode == 'S')){ //LOAD/STORE_____________________________

      accessCache(tagnum, setnum, numLines, cache, s);
    }
    if (mode == 'M'){ //MODIFY_________________________________________________
      accessCache(tagnum, setnum, numLines, cache, s);
      accessCache(tagnum, setnum, numLines, cache, s);
    }
  }
}

int main(int argc, char *argv[]){

  //Step1: Read the given arguments
  int s;
  int e;
  int b;
  char *filename;
  if (argc==9){
    s= atoi(argv[2]); //put to int
    e= atoi(argv[4]);
    b= atoi(argv[6]);
    filename= argv[8];
  }
  else { //argc==10
    s= atoi(argv[3]);
    e= atoi(argv[5]);
    b= atoi(argv[7]);
    filename= argv[9];
  }

  //Step2: Make cache
  int bigS= pow(2,s);

  set* cache=(set*)malloc(sizeof(set)*bigS); //First, initialize the cache. Cache is an array of sets.
  for(int i=0;i<bigS;i++) {

    cache[i].lines = (line*) malloc(sizeof(line) *e); //Set is array of lines.
    for (int j=0; j<e; j++){
      cache[i].lines[j].tag = 0;
      cache[i].lines[j].time = 0;
      cache[i].lines[j].valid = 0;
    }
  }

  //Step3: Read the file and put read it into cache line by line.
  FILE *fptr;
  if ((fptr = fopen(filename, "r")) == NULL){
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
  }

  char string[100];
  while (fgets(string, sizeof(string), fptr) != 0){
    dealWithInstruction(string, cache, s, e, b);
    globalClock++;
  }
  printSummary(hits, misses, evictions);

  return 0;
}
