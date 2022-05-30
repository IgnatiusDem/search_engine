#include <iostream>
#include <map>
#include <vector>
#include "gtest/gtest.h"
#include "converter_json.h"
#include "inverted_index.h"
#include "search_server.h"

/**TESTS**/
// Test #1
TEST(TestCaseSearchServer, TestSimple2) {
  const std::vector<std::string> docs = {
      "milk milk milk milk water water water", "milk water water",
      "milk milk milk milk milk water water water water water",
      "Americano Cappuccino"};
  const std::vector<std::string> request = {"milk water", "sugar"};
  const std::vector<std::vector<RelativeIndex>> expected = {
      {{2, 1}, {0, 0.7}, {1, 0.3}}, {{}}};
  InvertedIndex idx;
  idx.UpdateDocumentBase(docs);
  SearchServer srv(idx);
  std::vector<std::vector<RelativeIndex>> result = srv.search(request, 5);
  std::cout << "My" << std::endl;
  int i = 0;
  for (auto &element : result) {
    std::cout << i << ": " << std::endl;
    for (auto &secondElement : element) {
      if (!element.empty())
        std::cout << secondElement.doc_id << " " << secondElement.rank
                  << std::endl;
    }
    i++;
  }
  std::cout << "Correct" << std::endl;
  i = 0;
  for (auto &element : expected) {
    std::cout << i << ": " << std::endl;
    for (auto &secondElement : element) {
      if (!element.empty())
        std::cout << secondElement.doc_id << " " << secondElement.rank
                  << std::endl;
    }
    i++;
  }

  bool check = result == expected;
  if (check)
    std::cout << "TRUE" << std::endl;
  else
    std::cout << "FALSE" << std::endl;

  ConverterJSON converter;
  converter.putAnswers(srv.transformRelativeIndex(result));
  ASSERT_EQ(result, expected);
}

// Test #2 (expected file not correct)
TEST(TestCaseSearchServer, TestTop5) {
  const std::vector<std::string> docs = {
      "london is the capital of great britain",
      "paris is the capital of france",
      "berlin is the capital of germany",
      "rome is the capital of italy",
      "madrid is the capital of spain",
      "lisboa is the capital of portugal",
      "bern is the capital of switzerland",
      "moscow is the capital of russia",
      "kiev is the capital of ukraine",
      "minsk is the capital of belarus",
      "astana is the capital of kazakhstan",
      "beijing is the capital of china",
      "tokyo is the capital of japan",
      "bangkok is the capital of thailand",
      "welcome to moscow the capital of russia the third rome",
      "amsterdam is the capital of netherlands",
      "helsinki is the capital of finland",
      "oslo is the capital of norway",
      "stockholm is the capital of sweden",
      "riga is the capital of latvia",
      "tallinn is the capital of estonia",
      "warsaw is the capital of poland",
  };
  const std::vector<std::string> request = {"moscow is the capital of russia"};
  const std::vector<std::vector<RelativeIndex>> expected = {
      {{7, 1}, {14, 1}, {0, 0.6}, {1, 0.6}, {2, 0.6}}};
  InvertedIndex idx;
  idx.UpdateDocumentBase(docs);

  SearchServer srv(idx);
  std::vector<std::vector<RelativeIndex>> result = srv.search(request, 5);
  std::cout << "My result: " << std::endl;
  int i = 0;
  for (auto &element : result) {
    std::cout << i << ": " << std::endl;
    for (auto &secondElement : element) {
      if (!element.empty())
        std::cout << secondElement.doc_id << " " << secondElement.rank
                  << std::endl;
    }
    i++;
  }

  std::cout << "Expected: " << std::endl;
  i = 0;
  for (auto &element : expected) {
    std::cout << i << ": " << std::endl;
    for (auto &secondElement : element) {
      if (!element.empty())
        std::cout << secondElement.doc_id << " " << secondElement.rank
                  << std::endl;
    }
    i++;
  }

  ASSERT_EQ(result, expected);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}