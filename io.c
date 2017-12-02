/*
 * Author: John-Michael Baldy
 * Description: Write string to txt file
 */
#include <stdio.h>


int main(){
  printf("Word\n" );
  char path[32] = "output/out.txt";
  char url[32] = "http://www.google.com/";

  openFileAndWrite(path, url);


}
// openFileAndWrite opens a file based on a path string passed in
// and writes the url which has a match to the search term to the
// file.
int openFileAndWrite(char *path, char *url){
  FILE * fp;
  // Open file based on path in append mode
  fp = fopen(path,"a");
  // If fp = NULL, can't find||open FILE
  if(fp==NULL){
    perror("Could not open output file\n");
    fclose(fp);
    return -1;
  }
  // Write url to file
  fprintf(fp, "%s\n", url);
  // Close the file
  fclose(fp);
  return 0;
}
