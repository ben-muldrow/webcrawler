#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "crawler.h" // include Crawler struct

#define DEBUG 1

// define constants
char * SEARCH_TERM = "Ben";
int DEPTH = 5;

int main( int argc, char ** argv ) {

  if (DEBUG) printf("Started...\n");

  // define search term as first arg
  // if no search term is defined look up Ben
  if (argc > 0) {
    SEARCH_TERM = argv[0];
  }

  // define Crawler from struct
  Crawler myCrawler;
  myCrawler.searchTerm = SEARCH_TERM;
  myCrawler.url = "http://cofc.edu/";

  printf("crawling %s\n", myCrawler.url);

}
