// This is the webcrawler header file
// A web crawler will be able to:
// - write to the output
// - crawl down one layer

#define CrawlerTag
typedef struct CrawlerTag {

  char * url;
  char ** parsedUrls;

} Crawler;

// 1 = success
// 0 = failure
int write(char ** content);

void duplicate(char ** state);

// return an array of links
// take page content in HTML
char ** parse(char * content);
