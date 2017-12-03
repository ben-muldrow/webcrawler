#include <stdio.h>
#include <curl/curl.h>
#include <tidy/tidy.h>
#include <tidy/buffio.h>

#include "crawler.h"
#include "io.c"

#define MAX_LINKS 10

// init vars
TidyDoc parseDoc;
TidyBuffer tidyBuffer;
ctmbstr href = "href";

// this function is used to write website contents to an output buffer
// built from bufferStruct
size_t bufferCallback(
  char * buffer,
  size_t size,
  size_t nmemb,
  TidyBuffer * tidyBuffer){

    // append response to the tidyBuffer
    size_t newSize = size * nmemb;
    tidyBufAppend(tidyBuffer, buffer, newSize);
    printf("hmm %zu\n", newSize);

    // return size of response
    return newSize;
};

// output data to file
int write(char ** output) {
  char * data = output[0];
  for (int i = 1; data; i ++) {
    openFileAndWrite(OUTPUT_PATH, data);
    data = output[i];
  }
}

// parse website content in Tidy form
char ** parse(TidyDoc parseDoc, TidyNode node) {

  printf("parsing...\n");

  ctmbstr * output = (ctmbstr *) malloc(sizeof(ctmbstr) * MAX_LINKS);
  int i = 0; // index in output array

  TidyNode child;
  child = tidyGetChild(node);

  printf("mallocced \n");
  printf("name: %s\n", tidyNodeGetName(node));

  while (child != NULL) { // for each node in the Tidy Doc
    if (tidyNodeGetName(node)) { // if the node is valid

      // check for href attribute
      TidyAttr attr;
      attr = tidyAttrFirst(child);
      while (attr) {
        if (tidyAttrName(attr) == href) {
          printf("found one\n");
          output[i] = tidyAttrValue(attr); // if found, put in output array
          i++;
        }
        attr = tidyAttrNext(attr);
      }

    }
    child = tidyGetNext(child);
  }

  return output;
}

// get content of a website and store it in a buffer
int getContent(char * url, char * searchTerm) {

  printf("getting content\n");

  // if crawler exists
  if (url && searchTerm) {

    // intitialize cURL vars
    CURL *handle;
    CURLcode res;
    handle = curl_easy_init();

    // if initialized correctly
    if (handle) {

      // set up Tidy Buffer and Tidy Doc
      tidyBufInit(&tidyBuffer);
      parseDoc = tidyCreate();
      tidyOptSetInt(parseDoc, TidyWrapLen, 2048); // set max length

      printf("tidy set mydude\n");

      // set up and execute cURL
      curl_easy_setopt(handle, CURLOPT_URL, url); // set URL
      curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, bufferCallback); // set output callback function
      curl_easy_setopt(handle, CURLOPT_WRITEDATA, &parseDoc); // identify struct containing buffer

      printf("curl opt set\n");

      // execute request, return status code to res
      res = curl_easy_perform(handle);

      // check success
      if (res == CURLE_OK) {
        printf("successful crawl of %s\n", url);
        tidyParseBuffer(parseDoc, &tidyBuffer);
        parse(parseDoc, tidyGetRoot(parseDoc)); // parse results
      } else {
        printf("crawl failed for %s\n", url);
      }

      // clean up, close connections
      curl_easy_cleanup(handle);
      tidyBufFree(&tidyBuffer);
      tidyRelease(parseDoc);


      return 1; // success

    }

  } else {
    return 0; // failure
  }

}
