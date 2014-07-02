#include "urlconnection.h"

#define USERAGENT "libcurl-agent/1.0"

#define PRINTERR(...) fprintf(stderr, __VA_ARGS__)
#define SET_CURL_USERAGENT(curl_handle) \
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, USERAGENT)

struct pagecontents
{
    char *data;
    size_t size;
};

/*
 * Callback function to write a URL to a pagecontents struct
 * contents: the contents of the URL
 * size: the number of items in the contents
 * nmemb: the size (in bytes) of each item in the contents
 * userp: a pointer to a pagecontents struct
 * return: the size (in bytes) of the page contents
 */
static
size_t WriteMemoryCallback(
        void *contents,
        size_t size,
        size_t nmemb,
        void *userp);

/*
 * Begin the URL connection session
 * This should only be called once
 */
void BeginConnectionSession(void)
{
    curl_global_init(CURL_GLOBAL_ALL);
}

/*
 * Shut down the URL connection session
 * This should only be called once
 */
void EndConnectionSession(void)
{
    curl_global_cleanup();
}

/*
 * Make an HTTP GET request to the specified URL
 * url: the url where the GET request will be made
 * return: the contents of the HTTP GET
 */
char *httpGet(char *url)
{
    CURL *curl_handle;
    CURLcode res;

    struct pagecontents page;
    page.data = malloc(1);
    page.size = 0;

    //Initialize the curl handle
    curl_handle = curl_easy_init();

    //Set user agent in case server requires it
    SET_CURL_USERAGENT(curl_handle);

    //Set URL and write callback
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&page);

    //Perform the HTTP GET
    res = curl_easy_perform(curl_handle);
    if (res != CURLE_OK)
    {
        PRINTERR("HTTP GET failed: %s\n", curl_easy_strerror(res));
    }

    //Clean up the curl handle and return the page contents
    curl_easy_cleanup(curl_handle);
    return page.data;
}

/*
 * Make an HTTP GET request to the specified URL
 * and return the results as JSON
 * url: the url where the GET request will be made
 * return: the contents of the HTTP GET as a cJSON object
 */
cJSON *httpGetJSON(char *url)
{
    char *contents = httpGet(url);
    cJSON *json = cJSON_Parse(contents);

    free(contents);
    return json;
}

/*
 * Make an HTTP POST request to the specified URL
 * url: the url where the POST request will be made
 * params: the parameters to POST
 * return: the contents of the HTTP POST
 */
char *httpPost(char *url, char **params)
{
    //TODO: Make HTTP POST request
    return NULL;
}

/*
 * Make an HTTP POST request to the specified URL
 * and return the results as JSON
 * url: the url where the POST request will be made
 * params: the parameters to POST
 * return: the contents of the HTTP POST as a cJSON object
 */
cJSON *httpPostJSON(char *url, char **params)
{
    char *contents = httpPost(url, params);
    cJSON *json = cJSON_Parse(contents);

    free(contents);
    return json;
}

/*
 * Callback function to write a URL to a pagecontents struct
 * contents: the contents of the URL
 * size: the number of items in the contents
 * nmemb: the size (in bytes) of each item in the contents
 * userp: a pointer to a pagecontents struct
 * return: the size (in bytes) of the page contents
 */
static
size_t WriteMemoryCallback(
        void *contents,
        size_t size,
        size_t nmemb,
        void *userp)
{
    size_t realsize = size * nmemb;
    struct pagecontents *page = (struct pagecontents *)userp;

    page->data = realloc(page->data, page->size + realsize + 1);
    memcpy(&(page->data[page->size]), contents, realsize);

    page->size += realsize;
    page->data[page->size] = '\0';

    return realsize;
}
