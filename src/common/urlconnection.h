#ifndef __URL_CONNECTION_H__
#define __URL_CONNECTION_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "cJSON.h"

/*
 * Begin the URL connection session
 * This should only be called once
 */
void BeginConnectionSession(void);

/*
 * Shut down the URL connection session
 * This should only be called once
 */
void EndConnectionSession(void);

/*
 * Make an HTTP GET request to the specified URL
 * url: the url where the GET request will be made
 * return: the contents of the HTTP GET
 */
char *httpGet(char *url);

/*
 * Make an HTTP GET request to the specified URL
 * and return the results as JSON
 * url: the url where the GET request will be made
 * return: the contents of the HTTP GET as a cJSON object
 */
cJSON *httpGetJSON(char *url);

/*
 * Make an HTTP POST request to the specified URL
 * url: the url where the POST request will be made
 * postfields: the POST data
 * return: the contents of the HTTP POST
 */
char *httpPost(char *url, char *postfields);

/*
 * Make an HTTP POST request to the specified URL
 * and return the results as JSON
 * url: the url where the POST request will be made
 * postfields: the POST data
 * return: the contents of the HTTP POST as a cJSON object
 */
cJSON *httpPostJSON(char *url, char *postfields);

#endif
