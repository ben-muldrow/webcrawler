#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <tidy/tidy.h>
#include <tidy/buffio.h>

#include "crawler.h"
#include "io.c"

#define MAX_LINKS 10

// init vars
ctmbstr href = "href";
ctmbstr a = "a";

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
void parse(TidyNode node) {

  TidyNode child;

  // for each child, recursively parse all of their children
  for (child = tidyGetChild(node); child != NULL; child = tidyGetNext(child)) {

    // if href exists, output it
    TidyAttr hrefAttr = tidyAttrGetById(child, TidyAttr_HREF);
    if (hrefAttr) {
      // TODO output to struct var
      printf("url found: %s\n", tidyAttrValue(hrefAttr));
    }

    // recursive call for tree traversing
    parse(child);
  }

}

// get content of a website and store it in a buffer
int getContent(char * url, char * searchTerm) {

  // if crawler exists
  if (url) {

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
      curl_easy_setopt(handle, CURLOPT_URL, url); // set URL
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
        printf("successful crawl of %s\n", url);

        // parse webpage so it is readable by Tidy
        tidyParseBuffer(parseDoc, &tidyBuffer);

        parse(tidyGetBody(parseDoc)); // parse results
      } else {
        printf("crawl failed for %s\n", url);
        return 0; // failure
      }

      // clean up, close connections
      curl_easy_cleanup(handle);
      tidyBufFree(&tidyBuffer);
      tidyBufFree(&tidyErrBuff);
      tidyRelease(parseDoc);

      return 1; // success

    }
    return 0;

  }
  return 0; // failure

}
