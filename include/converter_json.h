#pragma once
#include "config.h"
/** Exceptions **/

class ConfigFileIsMissingException : public std::exception {
  const char *what() const noexcept override;
};

class ConfigFileIsEmptyException : public std::exception {
  const char *what() const noexcept override;
};

class TextFileIsMissingException : public std::exception {
  const char *what() const noexcept override;
};

class DifferentVersionException : public std::exception {
  const char *what() const noexcept override;
};

class RequestFileIsMissingException : public std::exception {
  const char *what() const noexcept override;
};

class RequestFileIsEmptyException : public std::exception {
  const char *what() const noexcept override;
};

class AnswersFileIsMissingException : public std::exception {
  const char *what() const noexcept override;
};

/** Functions for use in several classes **/

bool repeat();

/** Classes for work with JSON files **/

struct ConfigJSON {
  std::string name = NAME_APP;
  std::string version = VERSION_APP;
  int max_responses = _MAX_RESPONSES;
  std::vector<std::string> files;

  ConfigJSON(){
    saveToFile();
  };
  void addMaxResponses();
  void addMaxResponses(int newMaxResponses);
  void addFiles();
  void addFiles(std::vector<std::string> &newFiles);
  void saveToFile();
};

struct RequestsJSON {
  std::vector<std::string> requests;

  void addRequests();
  void addRequests(std::vector<std::string> &newRequests);
  void saveToFile();
};

// Class for working with JSON files
class ConverterJSON {
  ConfigJSON configJSON;
  RequestsJSON requestJSON;

public:
  ConverterJSON();

  // * Method of add requests by input from command to requests.json
  void addRequests();

  // * Method of add requests by load exist data to requests.json
  void addRequests(std::vector<std::string> &newRequests);

  // * Method of add addresses files to config.json
  void addFiles();

  // * Method of add addresses files to config.json
  void addFiles(std::vector<std::string> &newFiles);

  // * Method of getting the contents of files
  // * @return Returns a list with contents of files listed
  // in config.json
  std::vector<std::string> GetTextDocuments();

  // * The method reads max_responses field to determine maximum
  // number of responses per request
  // * @return max_responses
  int GetResponsesLimit();

  // * Method of getting requests from requests.json
  // * @return returns a list of requests from a file requests.json
  std::vector<std::string> GetRequests();

  // Put in answers.json search query results
  void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
};
