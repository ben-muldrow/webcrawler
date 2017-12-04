## README
A web crawler written in C

### What is a web crawler
A bot or program which browses the web for the purpose of web indexing.


#### Dependencies
* [libcurl](http://curl.haxx.se/libcurl/c/): A library built for making HTTP requests.
* [TidyLib](http://api.html-tidy.org/tidy/tidylib_api_5.4.0/tidylib.html): A library built for cleaning HTML pages. We used it to parse HTML and extract links.

### How to run
Assuming libcurl and TidyLib are installed, simply run `Make` and then you can execute `./main <url>` to start crawling.
