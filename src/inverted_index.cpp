#include <algorithm>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "inverted_index.h"

// For multi-reading of read and record freq_dictionary
std::mutex dict_access;

/** Methods for Entry class **/

Entry::Entry(size_t _doc_id, size_t _count) : doc_id(_doc_id), count(_count) {}

// This operator is necessary for conducting test scenarios
bool Entry::operator==(const Entry &other) const {
  return (doc_id == other.doc_id && count == other.count);
}
// This operator is necessary for sorting by doc_id
bool Entry::operator<(const Entry &other) const {
  return (doc_id < other.doc_id);
}

/** Methods for InvertedIndex class **/

// InvertedIndex::InvertedIndex()=default;

// Method of adding or update word to dictionary
void InvertedIndex::addWordToDictionary(size_t doc_id, std::string &word) {
  dict_access.lock();
  auto it = freq_dictionary.find(word);
  if (it != freq_dictionary.end()) {
    bool check = false;
    for (int i = 0; i < it->second.size(); i++) {
      if (it->second[i].doc_id == doc_id) {
        it->second[i].count++;
        check = true;
        i = (int)it->second.size();
      }
    }
    if (!check) {
      it->second.push_back(Entry(doc_id, 1));
    }
  } else {
    std::vector<Entry> inputVector;
    inputVector.push_back(Entry(doc_id, 1));
    freq_dictionary[word] = inputVector;
  }
  dict_access.unlock();
}

// Method of find words in text of document
void InvertedIndex::findWords(size_t doc_id, const std::string &text) {
  std::string word;
  for (int i = 0; i < text.size(); i++) {
    if (word.empty()) {
      word = text[i];
    } else if (text[i] != ' ') {
      word += text[i];
    } else {
      addWordToDictionary(doc_id, word);
      word.clear();
    }

    if (i == text.size() - 1 && !word.empty())
      addWordToDictionary(doc_id, word);
  }
}

// * Update or fill in database of documents for which
// we will search
// * @param texts_input document contents
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
  docs = std::move(input_docs);
  std::vector<std::thread> threads;
  threads.resize(docs.size());
  for (int i = 0; i < docs.size(); i++) {
    threads[i] =
        std::thread(&InvertedIndex::findWords, this, i, std::ref(docs[i]));
  }
  for (auto &t : threads) {
    t.join();
  }
  // To exclude an error when a thread with a smaller doc_id ended earlier
  for (auto &it : freq_dictionary) {
    std::sort(it.second.begin(), it.second.end());
  }
}

// * The method determines the number of occurrences of word in
// downloaded database of documents
// * @param word whose frequency of occurrences must be determined
// * @return returns a prepared list with frequency of words
std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
  auto it = freq_dictionary.find(word);
  if (it == freq_dictionary.end())
    return {};
  return it->second;
}
