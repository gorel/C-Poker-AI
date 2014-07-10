#include "tests.h"

#define ARR_LEN     7

#define HIGH_CARD        "{\"cards\" : [\"2H\", \"3C\", \"QD\", \"TS\", \"9C\", \"8D\", \"7D\"]}"
#define HIGH_CARD2       "{\"cards\" : [\"AH\", \"3C\", \"QD\", \"TS\", \"9C\", \"8D\", \"7D\"]}"
#define PAIR             "{\"cards\" : [\"2H\", \"2C\", \"3D\", \"6S\", \"7S\", \"9D\", \"TC\"]}"
#define PAIR2            "{\"cards\" : [\"AH\", \"AC\", \"3D\", \"6S\", \"7S\", \"9D\", \"TC\"]}"
#define TWO_PAIR         "{\"cards\" : [\"2H\", \"2C\", \"3D\", \"3S\", \"4C\", \"5D\", \"7D\"]}"
#define TWO_PAIR2        "{\"cards\" : [\"AH\", \"AC\", \"3D\", \"3S\", \"4C\", \"5D\", \"7D\"]}"
#define THREE_OF_A_KIND  "{\"cards\" : [\"2H\", \"2C\", \"2D\", \"3S\", \"4C\", \"5D\", \"7D\"]}"
#define THREE_OF_A_KIND2 "{\"cards\" : [\"AH\", \"AC\", \"AD\", \"3S\", \"4C\", \"5D\", \"7D\"]}"
#define STRAIGHT         "{\"cards\" : [\"2H\", \"3C\", \"4D\", \"5S\", \"6C\", \"7D\", \"AH\"]}"
#define STRAIGHT2        "{\"cards\" : [\"2H\", \"3C\", \"4D\", \"5S\", \"6C\", \"7D\", \"8H\"]}"
#define FLUSH            "{\"cards\" : [\"2H\", \"3H\", \"4H\", \"5H\", \"7H\", \"8H\", \"AD\"]}"
#define FLUSH2           "{\"cards\" : [\"2H\", \"3H\", \"4H\", \"5H\", \"7H\", \"8H\", \"AH\"]}"
#define FULL_HOUSE       "{\"cards\" : [\"2H\", \"2C\", \"2D\", \"3H\", \"3C\", \"4D\", \"5D\"]}"
#define FULL_HOUSE2      "{\"cards\" : [\"AH\", \"AC\", \"AD\", \"3H\", \"3C\", \"4D\", \"5D\"]}"
#define FOUR_OF_A_KIND   "{\"cards\" : [\"2H\", \"2C\", \"2D\", \"2S\", \"3H\", \"3D\", \"3S\"]}"
#define FOUR_OF_A_KIND2  "{\"cards\" : [\"AH\", \"AC\", \"AD\", \"AS\", \"3H\", \"3D\", \"3S\"]}"
#define STRAIGHT_FLUSH   "{\"cards\" : [\"2H\", \"3H\", \"4H\", \"5H\", \"6H\", \"7H\", \"9D\"]}"
#define STRAIGHT_FLUSH2  "{\"cards\" : [\"2H\", \"3H\", \"4H\", \"5H\", \"6H\", \"7H\", \"8H\"]}"

#define JSON(json, field)\
    cJSON_GetObjectItem(json, field)

int *cards;

static inline
int scoreJSON(char *jsontext)
{
    cJSON *json = cJSON_Parse(jsontext);
    GetCardArray(cards, JSON(json, "cards"));
    cJSON_Delete(json);

    return GetHandValue(cards, ARR_LEN);
}

TestResult *TestEvaluator(void)
{
    int numtests = 0;
    int failed = 0;
    cards = malloc(sizeof(int) * ARR_LEN);

    if (scoreJSON(HIGH_CARD) > scoreJSON(HIGH_CARD2))
    {
        fprintf(stderr, "[EVALUATOR] Failed BETTER HIGH CARD\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(HIGH_CARD) > scoreJSON(PAIR))
    {
        fprintf(stderr, "[EVALUATOR] Failed HIGH CARD VS PAIR\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(PAIR) > scoreJSON(PAIR2))
    {
        fprintf(stderr, "[EVALUATOR] Failed BETTER PAIR\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(PAIR) > scoreJSON(TWO_PAIR))
    {
        fprintf(stderr, "[EVALUATOR] Failed PAIR VS TWO PAIR\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(TWO_PAIR) > scoreJSON(TWO_PAIR2))
    {
        fprintf(stderr, "[EVALUATOR] Failed BETTER TWO PAIR\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(TWO_PAIR) > scoreJSON(THREE_OF_A_KIND))
    {
        fprintf(stderr, "[EVALUATOR] Failed TWO PAIR VS THREE OF A KIND\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(THREE_OF_A_KIND) > scoreJSON(THREE_OF_A_KIND2))
    {
        fprintf(stderr, "[EVALUATOR] Failed BETTER THREE OF A KIND\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(THREE_OF_A_KIND) > scoreJSON(STRAIGHT))
    {
        fprintf(stderr, "[EVALUATOR] Failed THREE OF A KIND VS STRAIGHT\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(STRAIGHT) > scoreJSON(STRAIGHT2))
    {
        fprintf(stderr, "[EVALUATOR] Failed BETTER STRAIGHT\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(STRAIGHT) > scoreJSON(FLUSH))
    {
        fprintf(stderr, "[EVALUATOR] Failed STRAIGHT VS FLUSH\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(FLUSH) > scoreJSON(FLUSH2))
    {
        fprintf(stderr, "[EVALUATOR] Failed BETTER FLUSH\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(FLUSH) > scoreJSON(FULL_HOUSE))
    {
        fprintf(stderr, "[EVALUATOR] Failed FLUSH VS FULL HOUSE\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(FULL_HOUSE) > scoreJSON(FULL_HOUSE2))
    {
        fprintf(stderr, "[EVALUATOR] Failed BETTER FULL HOUSE\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(FULL_HOUSE) > scoreJSON(FOUR_OF_A_KIND))
    {
        fprintf(stderr, "[EVALUATOR] Failed FULL HOUSE VS FOUR OF A KIND\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(FOUR_OF_A_KIND) > scoreJSON(FOUR_OF_A_KIND2))
    {
        fprintf(stderr, "[EVALUATOR] Failed BETTER FOUR OF A KIND\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(FOUR_OF_A_KIND) > scoreJSON(STRAIGHT_FLUSH))
    {
        fprintf(stderr, "[EVALUATOR] Failed FOUR OF A KIND VS STRAIGHT FLUSH\n");
        failed++;
    }
    numtests++;

    if (scoreJSON(STRAIGHT_FLUSH) > scoreJSON(STRAIGHT_FLUSH2))
    {
        fprintf(stderr, "[EVALUATOR] Failed BETTER STRAIGHT FLUSH\n");
        failed++;
    }
    numtests++;

    free(cards);
    fprintf(stderr, "[EVALUATOR]\t\tpassed %d/%d\n", (numtests - failed), numtests);
    return CreateResult(failed, numtests);
}
