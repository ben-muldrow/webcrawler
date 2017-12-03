#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "crawler.h" // include Crawler struct

#define DEBUG 1

// define default values
char * searchTerm = "Ben";
int depth = 5;

int main( int argc, char ** argv ) {

  if (DEBUG) printf("Started...\n");

  // define search term as first arg
  // if no search term is defined look up Ben
  if (argc > 0) {
    searchTerm = argv[0];
  }

  // define Crawler from struct
  Crawler myCrawler;
  myCrawler.searchTerm = searchTerm;
  myCrawler.url = "http://cofc.edu/";

  printf("crawling %s\n", myCrawler.url);

}
