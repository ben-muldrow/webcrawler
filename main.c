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

  pthread_t threads[MAX_LINKS];

  // define search term as first arg
  // if no search term is defined look up Ben
  if (argc > 0) {
    searchTerm = argv[0];
  }

  // allocate output array
  char ** output = (char **) malloc(MAX_LINKS * (sizeof(char *)));
  for (int i = 0; i < MAX_LINKS; i ++) {
    output[i] = (char *) malloc(MAX_URL_LEN * sizeof(char *));
  }

  // define Crawler from struct
  Crawler * myCrawler = malloc(sizeof(Crawler));;
  (*myCrawler).url = argv[1];
  (*myCrawler).parsedUrls = output;

  // crawl website and write to output file
  getContent(myCrawler);
  write(myCrawler->parsedUrls);

  // now, create a thread for each link crawled and crawl again.
  for (int i = 0; myCrawler->parsedUrls[i] || i < MAX_LINKS - 1; i ++) {
    if (myCrawler->parsedUrls[i]){

      // alloc output array
      char ** output = (char **) malloc(MAX_LINKS * (sizeof(char *)));
      for (int i = 0; i < MAX_LINKS; i ++) {
        output[i] = (char *) malloc(MAX_URL_LEN * sizeof(char *));
      }

      // init crawlers for threads
      Crawler * crawler;
      crawler = malloc(sizeof(Crawler));
      (*crawler).url = myCrawler->parsedUrls[i];
      (*crawler).parsedUrls = output;

      // create thread
      int error = pthread_create(
        &threads[i],
        NULL,
        getContent,
        (void *) crawler
      );

      printf("Thread now crawling: %s\n", myCrawler->parsedUrls[i]);

      // check for thread create error
      if (error != 0) {
        printf("error making thread\n");
        exit(0);
      }
    }
  }

  // join threads
  for (int i = 0; i < MAX_LINKS - 1; i ++) {
    if (threads[i]) {
      printf("thread %d\n", i);
      wait(0);
      pthread_join(threads[i], NULL);
    }
  }

  printf("finished.\n");

}
