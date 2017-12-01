// This is the webcrawler header file
// A web crawler will be able to:
// - write to the output
// - crawl down one layer

#define CrawlerTag
typedef struct CrawlerTag {

  char * searchTerm;
  char * url;
  char ** parsedUrls;

  int * write;
  void * getContent;
  char * ** parse;

} Crawler;

// 1 = success
// 0 = failure
int write(char ** content);

// get the website content with libcurl
void getContent(char * url);

// return an array of links
// take page content in HTML
char ** parse(char * content);
