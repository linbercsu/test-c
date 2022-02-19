#ifndef __TEST__H__
#define __TEST__H__

typedef struct httpRequest {
    char *url;
    char *response;
    int responseSize;

    int (*fetchResponse)(struct httpRequest *);
} HttpRequest;

void httpInit(HttpRequest *request, const char *url);

void httpRelease(HttpRequest *request);

#endif