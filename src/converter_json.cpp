#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "converter_json.h"

/** Exceptions **/

const char *ConfigFileIsMissingException::what() const noexcept {
  return "config file is missing";
}

const char *ConfigFileIsEmptyException::what() const noexcept {
  return "config file is empty";
}

const char *TextFileIsMissingException::what() const noexcept {
  return "text file is missing";
}

const char *DifferentVersionException::what() const noexcept {
  return "config file have another version or program name";
}

const char *RequestFileIsMissingException::what() const noexcept {
  return "request file is missing";
}

const char *RequestFileIsEmptyException::what() const noexcept {
  return "request file is empty";
}

const char *AnswersFileIsMissingException::what() const noexcept {
  return "answers file is missing";
}

/** Functions for use in several classes **/

bool repeat() {
  char answer;
  do {
    std::cout << "Repeat operation (Y/N)?" << std::endl;
    std::cin >> answer;
    if (answer == 'Y' || answer == 'y')
      return true;
    else if (answer == 'N' || answer == 'n')
      return false;
    else
      std::cerr << "Wrong command!" << std::endl;
  } while (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n');
  return false;
}

/** Classes for work with JSON files **/
/** Methods for ConfigJSON struct **/

void ConfigJSON::addMaxResponses() {
  bool check = false;
  while (!check) {
    std::cout << "Enter new max responses:" << std::endl;
    std::cin >> max_responses;
    check = max_responses > 0;
    if (!check)
      std::cerr << "Max responses "
                   "should be more 0"
                << std::endl;
  }
}

void ConfigJSON::addMaxResponses(int newMaxResponses) {
  bool check = false;
  check = newMaxResponses > 0;
  if (!check)
    std::cerr << "Max responses should be more 0" << std::endl;
  else
    max_responses = newMaxResponses;
}

void ConfigJSON::addFiles() {
  bool check = true;
  do {
    std::string address;
    std::cout << "Enter new address:" << std::endl;
    std::cin >> address;
    files.push_back(address);
    check = repeat();
  } while (check);
}

void ConfigJSON::addFiles(std::vector<std::string> &newFiles) {
  files = std::move(newFiles);
}

void ConfigJSON::saveToFile() {
  std::ofstream fileRecord("config.json");
  nlohmann::json configRecord;
  configRecord["config"]["name"] = name;
  configRecord["config"]["version"] = version;
  configRecord["config"]["max_responses"] = max_responses;
  configRecord["files"] = files;
  fileRecord << configRecord;
  fileRecord.close();
  configRecord.clear();
}

/** Methods for RequestsJSON struct **/

void RequestsJSON::addRequests() {
  bool check = true;
  do {
    std::string request;
    std::cout << "Enter new address:" << std::endl;
    std::getline(std::cin, request);
    requests.push_back(request);
    check = repeat();
  } while (check);
}

void RequestsJSON::addRequests(std::vector<std::string> &newRequests) {
  requests = std::move(newRequests);
}

void RequestsJSON::saveToFile() {
  std::ofstream fileRecord("requests.json");
  if (!fileRecord.is_open())
    throw ConfigFileIsMissingException();
  nlohmann::json requestsRecord;
  requestsRecord["requests"] = requests;
  fileRecord << requestsRecord;
  fileRecord.close();
  requestsRecord.clear();
}

/** Methods for ConverterJSON class **/

ConverterJSON::ConverterJSON() {
  std::cout << configJSON.name << " Version: " << configJSON.version
            << std::endl;
};

// * Method of add requests by input from command to requests.json
void ConverterJSON::addRequests() {
  requestJSON.addRequests();
  requestJSON.saveToFile();
}

// * Method of add requests by load exist data to requests.json
void ConverterJSON::addRequests(std::vector<std::string> &newRequests) {
  requestJSON.addRequests(newRequests);
  requestJSON.saveToFile();
}

// * Method of add addresses files to config.json
void ConverterJSON::addFiles() { configJSON.addFiles(); }

// * Method of add addresses files to config.json
void ConverterJSON::addFiles(std::vector<std::string> &newFiles) {
  configJSON.addFiles(newFiles);
}

// * Method of getting the contents of files
// * @return Returns a list with contents of files listed
// in config.json
std::vector<std::string> ConverterJSON::GetTextDocuments() {
  std::ifstream fileRead("config.json");
  try {
    if (!fileRead.is_open())
      throw ConfigFileIsMissingException();
    nlohmann::json configRead;
    fileRead >> configRead;
    if (configRead["config"].empty() || configRead["files"].empty()) {
      throw ConfigFileIsEmptyException();
    }
    if (configRead["config"]["version"].get<std::string>() !=
            configJSON.version ||
        configRead["config"]["name"].get<std::string>() != configJSON.name) {
      throw DifferentVersionException();
    }
    configJSON.files = configRead["files"].get<std::vector<std::string>>();
    fileRead.close();
    configRead.clear();
    std::vector<std::string> docs;
    for (auto &file : configJSON.files) {
      std::string text;
      std::ifstream fileReadText(file);
      if (!fileReadText.is_open())
        throw TextFileIsMissingException();
      while (!fileReadText.eof()) {
        std::string curWord;
        fileReadText >> curWord;
        text += curWord + " ";
      }
      docs.push_back(text);
      fileReadText.close();
    }
    return docs;
  } catch (const ConfigFileIsMissingException &x) {
    std::cerr << "config file is missing" << std::endl;
    return {};
  } catch (const ConfigFileIsEmptyException &x) {
    fileRead.close();
    std::cerr << "config file is empty" << std::endl;
    return {};
  } catch (const DifferentVersionException &x) {
    fileRead.close();
    std::cerr << "config file from different version or name of program another"
              << std::endl;
    return {};
  } catch (const TextFileIsMissingException &x) {
    std::cerr << "text file is missing" << std::endl;
    return {};
  }
}

// * The method reads max_responses field to determine maximum
// number of responses per request
// * @return max_responses
int ConverterJSON::GetResponsesLimit() {
  std::ifstream fileRead("config.json");
  try {
    if (!fileRead.is_open())
      throw ConfigFileIsMissingException();
    nlohmann::json configRead;
    fileRead >> configRead;
    if (configRead["config"]["max_responses"].empty()) {
      throw ConfigFileIsEmptyException();
    }
    if (configRead["config"]["version"].get<std::string>() !=
            configJSON.version ||
        configRead["config"]["name"].get<std::string>() != configJSON.name) {
      throw DifferentVersionException();
    }
    configJSON.max_responses = configRead["config"]["max_responses"].get<int>();
    fileRead.close();
    configRead.clear();
    return configJSON.max_responses;
  } catch (const ConfigFileIsMissingException &x) {
    std::cerr << "config file is missing" << std::endl;
    return configJSON.max_responses;
  } catch (const ConfigFileIsEmptyException &x) {
    fileRead.close();
    std::cerr << "config file is empty" << std::endl;
    return configJSON.max_responses;
  } catch (const DifferentVersionException &x) {
    fileRead.close();
    std::cerr << "config file from different version or name of program another"
              << std::endl;
    return configJSON.max_responses;
  }
}

// * Method of getting requests from requests.json
// * @return returns a list of requests from a file requests.json
std::vector<std::string> ConverterJSON::GetRequests() {
  std::ifstream fileRead("requests.json");
  try {
    if (!fileRead.is_open())
      throw RequestFileIsMissingException();
    nlohmann::json requestsRead;
    fileRead >> requestsRead;
    if (requestsRead["requests"].empty()) {
      throw RequestFileIsEmptyException();
    }
    requestJSON.requests =
        requestsRead["requests"].get<std::vector<std::string>>();
    fileRead.close();
    requestsRead.clear();
    return requestJSON.requests;
  } catch (const RequestFileIsMissingException &x) {
    std::cerr << "request file is missing" << std::endl;
    return {};
  } catch (const RequestFileIsEmptyException &x) {
    fileRead.close();
    std::cerr << "request file is empty" << std::endl;
    return {};
  }
}

// Put in answers.json search query results
void ConverterJSON::putAnswers(
    std::vector<std::vector<std::pair<int, float>>> answers) {
  std::ofstream fileRecord("answers.json");
  try {
    if (!fileRecord.is_open())
      throw AnswersFileIsMissingException();
    nlohmann::json answersRecord;
    for (int i = 0; i < answers.size(); i++) {
      // Create requests id
      std::string numberRequest;
      if ((i + 1) < 10)
        numberRequest = "00" + std::to_string(i + 1);
      else if ((i + 1) < 100)
        numberRequest = "0" + std::to_string(i + 1);
      else
        numberRequest = std::to_string(i + 1);

      // Record result of search true or false
      if (!answers[i].empty()) {
        answersRecord["answers"]["request" + numberRequest]["result"] = true;
      } else {
        answersRecord["answers"]["request" + numberRequest]["result"] = false;
      }

      // Record relevance
      for (auto &j : answers[i]) {
        nlohmann::json relevance;
        relevance["docid"] = j.first;
        relevance["rank"] = j.second;
        answersRecord["answers"]["request" + numberRequest]["relevance"]
            .push_back(relevance);
      }
    }
    fileRecord << answersRecord;
    fileRecord.close();
    answersRecord.clear();
  } catch (const AnswersFileIsMissingException &x) {
    std::cerr << "answers file is missing" << std::endl;
  }
}