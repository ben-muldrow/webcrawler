#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <tidy/tidy.h>
#include <tidy/buffio.h>

#include "crawler.h"
#include "io.c"

#define MAX_LINKS 10
#define MAX_URL_LEN 512

// init vars
int currentIndex = 0;

// this function is used to write website contents to an output buffer
// built from bufferStruct
size_t bufferCallback(
  char * buffer,
  size_t size,
  size_t nmemb,
  TidyBuffer * tidyBuffer) {

    // append response to the tidyBuffer
    size_t newSize = size * nmemb;
    tidyBufAppend(tidyBuffer, buffer, newSize);

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
void parse(TidyNode node, char ** output) {
  if (currentIndex < 10) {
    TidyNode child;

    // for each child, recursively parse all of their children
    for (child = tidyGetChild(node); child != NULL; child = tidyGetNext(child)) {

      // if href exists, output it
      TidyAttr hrefAttr = tidyAttrGetById(child, TidyAttr_HREF);
      if (hrefAttr) {
        // TODO output to struct var
        if (strlen(tidyAttrValue(hrefAttr)) < MAX_URL_LEN) {
          strcpy(output[currentIndex], tidyAttrValue(hrefAttr));
          currentIndex ++;
          printf("index: %d\n", currentIndex);
        }
      }

      // recursive call for tree traversing
      parse(child, output);
    }
  }
}

// get content of a website and store it in a buffer
int getContent(Crawler crawler) {

  // if crawler exists
  if (crawler.url) {

    // intitialize cURL vars
    CURL *handle;
    handle = curl_easy_init();
    char errBuff[CURL_ERROR_SIZE];
    int res;
    TidyDoc parseDoc;
    TidyBuffer tidyBuffer = {0};
    TidyBuffer tidyErrBuff = {0};

    // if initialized correctly
    if (handle) {

      // set up cURL options
      curl_easy_setopt(handle, CURLOPT_URL, crawler.url); // set URL
      curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, bufferCallback); // set output callback function
      curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errBuff);

      // set up Tidy Buffer and Tidy Doc
      parseDoc = tidyCreate();
      tidyBufInit(&tidyBuffer);
      tidyOptSetInt(parseDoc, TidyWrapLen, 2048); // set max length
      tidyOptSetBool(parseDoc, TidyForceOutput, yes); // force output

      curl_easy_setopt(handle, CURLOPT_WRITEDATA, &tidyBuffer); // identify buffer to store data in

      // execute request, return status code to res
      res = curl_easy_perform(handle);

      // check success
      if (res == CURLE_OK) {
        printf("successful crawl of %s\n", crawler.url);

        // parse webpage so it is readable by Tidy
        tidyParseBuffer(parseDoc, &tidyBuffer);

        // alloc output array
        for (int i = 0; i < MAX_LINKS; i ++) {
          crawler.parsedUrls[i] = (char *) malloc(MAX_URL_LEN * sizeof(char *));
        }
        parse(tidyGetBody(parseDoc), crawler.parsedUrls); // parse results
        crawler.parsedUrls = crawler.parsedUrls;
      } else {
        printf("crawl failed for %s\n", crawler.url);
        return 0; // failure
      }

      // clean up, close connections
      curl_easy_cleanup(handle);
      tidyBufFree(&tidyBuffer);
      tidyBufFree(&tidyErrBuff);
      tidyRelease(parseDoc);

      return 1; // success

    }
    return 0; // failure

  }
  return 0; // failure

}
