#include "tests.h"

#define GET_URL     "http://httpbin.org/get"
#define POST_URL    "http://httpbin.org/post"
#define POST_DATA   "query=test"
#define POST_DATA2  "query=test&moredata=true"

#define JSON(json, field) \
    cJSON_GetObjectItem(json, field)
#define JSON_STRING(json, field) \
    JSON(json, field)->valuestring

TestResult *TestURLConnection(void)
{
    int numtests = 0;
    int failed = 0;
    char *response;
    cJSON *json;
    cJSON *form;

    response = httpGet(GET_URL);
    if (!response)
    {
        fprintf(stderr, "Failed basic HTTP GET\n");
        failed++;
    }
    numtests++;
    free(response);

    response = httpPost(POST_URL, POST_DATA);
    if (!response)
    {
        fprintf(stderr, "Failed basic HTTP POST\n");
        failed++;
    }
    numtests++;
    free(response);

    response = httpPost(POST_URL, POST_DATA2);
    if (!response)
    {
        fprintf(stderr, "Failed complex HTTP POST\n");
        failed++;
    }
    numtests++;
    free(response);

    json = httpGetJSON(GET_URL);
    if (strcmp(JSON_STRING(json, "url"), GET_URL))
    {
        fprintf(stderr, "Failed basic HTTP GET to JSON\n");
        failed++;
    }
    numtests++;
    cJSON_Delete(json);

    json = httpPostJSON(POST_URL, POST_DATA);
    form = JSON(json, "form");
    if (strcmp(JSON_STRING(form, "query"), "test"))
    {
        fprintf(stderr, "Failed basic HTTP POST to JSON\n");
        failed++;
    }
    numtests++;
    cJSON_Delete(json);

    fprintf(stderr, "[URLCONNECTION]\t\tpassed %d/%d\n", (numtests - failed), numtests);
    return CreateResult(failed, numtests);
}
