<h1 align="center">search_engine</h1>

# Description
**The search engine is a console application
that performs a search and has the ability to configure through JSON format
files**

### The principles of the search engine:

- In the configuration file **(config.json)**, before launching the application, the names of the files that the engine will search for are set.
- The search engine crawls all files and indexes them so that it can then find the most relevant documents for any search query.
- The user sets a request via the **requests.json** file. A query is a set of words for which you need to find documents.
- The query is transformed into a list of words.
- The index searches for those documents on which all these words occur.
- The search results are ranked, sorted and given to the user, the maximum number of possible documents in the response is set in the configuration file **(config.json)**.
- At the end, the program generates the **answers.json** file, which records the search results.

## Required files for working
**JSON files (put to directory with project)**
- config.json
- requests.json

**Libraries**

- nlohmann_json

**Included classes**

- include/converter_json.h
- include/inverted_index.h
- include/search_server.h
- src/converter_json.cpp
- src/inverted_index.cpp
- src/search_server.cpp

tests for check right working project in directory "test"

## How to use

### Converter JSON

Include header in main.cpp

```
#include "include/converter_json.h"
```
or if you include directories "src" and "include" in CMakeList.txt:
```
target_include_directories(search_engine PRIVATE ${MY_INCLUDE_DIR} ${MY_SOURCE_DIR})
```
you can include like:
```
#include "converter_json.h" 
```
Create a class in project

```
ConverterJSON converterJson;
```
**Class ConverterJSON have next methods:**

- Method of add requests by input from command to requests.json

  ```
    void addRequests();
  ```
- Method of add requests by load exist data to requests.json

  ```
    void addRequests(std::vector<std::string>& newRequests);
  ```
- Method of add addresses files by input from command to config.json

  ```
    void addFiles();
  ```
- Method of add addresses files to config.json

  ```
    void addFiles(std::vector<std::string>& newFiles);
  ```
- Method of getting the contents of files . Returns a list with contents of files liste in config.json

  ```
    std::vector<std::string> GetTextDocuments();
  ```
- The method reads max_responses field to determine maximum number of responses per request. Return max_responses

  ```
    int GetResponsesLimit();
  ```
- Method of getting requests from requests.json. Returns a list of requests from a file requests.json

  ```
    std::vector<std::string> GetRequests();
  ```
- Put in answers.json search query results

  ```
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
  ```
  
### InvertedIndex

Include header in main.cpp

```
#include "include/inverted_index.h"
```
or if you include directories "src" and "include" in CMakeList.txt:
```
target_include_directories(search_engine PRIVATE ${MY_INCLUDE_DIR} ${MY_SOURCE_DIR})
```
you can include like:
```
#include "inverted_index.h" 
```
Create a class in project

```
InvertedIndex idx;
```

**Class InvertedIndex have next methods:**

- Update in database of documents for which we will search. texts_input document contents.

  ```
    void UpdateDocumentBase(std::vector<std::string> input_docs);
  ```

### SearchServer 

Include header in main.cpp

```
#include "include/inverted_index.h" //Required
#include "include/search_server.h"
```
or if you include directories "src" and "include" in CMakeList.txt:
```
target_include_directories(search_engine PRIVATE ${MY_INCLUDE_DIR} ${MY_SOURCE_DIR})
```
you can include like:
```
#include "inverted_index.h" //Required
#include "search_server.h"
```
Create a class in project

```
InvertedIndex idx;
SearchServer srv(idx);
```

**Class SearchServer have next methods:**
- Method of processing search queries. @param queries_input search queries taken from requests.json. @return returns a sorted list of relevant responses for the specified queries
    
    ```
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input, int max_responses);
    ```
- Method to transform to format for save in JSON and returns -sorted list of relevant responses
    
    ```
    std::vector<std::vector<std::pair<int, float>>> transformRelativeIndex(std::vector<std::vector<RelativeIndex>> search);
    ```
    

