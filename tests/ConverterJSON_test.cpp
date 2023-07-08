#include "../include/ConverterJSON.h"
#include "gtest/gtest.h"

// Проверка метода GetResponsesLimit()
TEST(ConverterJSON, getResponsesLimit) {
    ConverterJSON c;
    int responsesLimit = c.GetResponsesLimit();  // read config

    EXPECT_EQ(responsesLimit, 5);  // "maxResponses": 5    - in config.json
}

TEST(ConverterJSON, getConfig) {
    ConverterJSON c;
    c.GetConfig();

    EXPECT_EQ(c.SearchServerName, "SkillboxSearchEngine");  //  in config.json
}

// Проверка метода GetRequests()
TEST(ConverterJSON, getRequests) {
    ConverterJSON c;
    auto queries = c.GetRequests();

    EXPECT_EQ(queries.at(0), "some words terms");  //  first in request.json
}