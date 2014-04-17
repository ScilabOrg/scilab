%module curleasy

%{
#include <curl/curl.h>
%}

#define CURL_ISOCPP 1

%import <curl/curl.h>


extern CURL *curl_easy_init(void);

extern CURLcode curl_easy_setopt(CURL *curl, CURLoption option, long optionValue);
extern CURLcode curl_easy_setopt(CURL *curl, CURLoption option, char* optionValue);

extern CURLcode curl_easy_perform(CURL *curl);
extern void curl_easy_cleanup(CURL *curl);
extern CURLcode curl_easy_getinfo(CURL *curl, CURLINFO info, ...);
extern CURL* curl_easy_duphandle(CURL *curl);
extern void curl_easy_reset(CURL *curl);
extern CURLcode curl_easy_recv(CURL *curl, void *buffer, size_t buflen, size_t *n);
extern CURLcode curl_easy_send(CURL *curl, const void *buffer, size_t buflen, size_t *n);
