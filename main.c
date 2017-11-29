#include <stdio.h>
#include <pthread.h>

#include "crawler.h"

int main( int argc, char ** argv ) {
  printf("Started...\n");
  Crawler myCrawler;
  myCrawler.url = "http://cofc.edu/";
  printf("crawling %s\n", myCrawler.url);
}
