// This is the webcrawler header file
// A web crawler will be able to:
// - write to the output
// - crawl down one layer
#include <tidy/tidy.h>
#include <tidy/buffio.h>

#define BUFFER_SIZE 2048;

#define CrawlerTag
typedef struct CrawlerTag {

  char * searchTerm;
  char * url;
  char ** parsedUrls;
  char ** pastUrls;

} Crawler;

// 1 = success
// 0 = failure
int write(char ** content);

// get the website content with libcurl
int getContent(char * url, char * content);

// return an array of links
// take page content in HTML
char ** parse(TidyDoc parseDoc, TidyNode node);
