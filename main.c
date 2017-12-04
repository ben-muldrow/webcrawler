#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "crawler.c" // includer Crawler function implementations

// define default values
char * searchTerm = "Ben";
int depth = 5;

int main( int argc, char ** argv ) {

  printf("Started...\n");

  // define search term as first arg
  // if no search term is defined look up Ben
  if (argc > 0) {
    searchTerm = argv[0];
  }

  // define Crawler from struct
  Crawler myCrawler = {
    searchTerm,
    "http://cofc.edu",
    NULL,
    NULL
  };

  getContent(myCrawler.url, myCrawler.searchTerm);
  // crawl(myCrawler.url);

}
