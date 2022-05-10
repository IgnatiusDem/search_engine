#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <thread>
#include <mutex>
//#include "gtest/gtest.h"
#include "include/inverted_index.h"

// For multi-reading of read and record freq_dictionary
std::mutex dict_access;

/** Methods for Entry class **/

Entry::Entry(size_t _doc_id,size_t _count):doc_id(_doc_id),count(_count){}

// This operator is necessary for conducting test scenarios
bool Entry::operator==(const Entry &other) const {
     return (doc_id == other.doc_id && count == other.count);
}

/** Methods for InvertedIndex class **/

//InvertedIndex::InvertedIndex()=default;

// Method of adding or update word to dictionary
void InvertedIndex::addWordToDictionary(size_t doc_id, std::string &word) {
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        bool check=false;
        for(int i=0;i<it->second.size();i++){
            if (it->second[i].doc_id == doc_id) {
                dict_access.lock();
                it->second[i].count++;
                dict_access.unlock();
                check=true;
                i=(int)it->second.size();
            }
        }
        if(!check) {
            dict_access.lock();
            it->second.emplace_back(doc_id,1);
            dict_access.unlock();
        }
    } else {
        std::vector<Entry> inputVector;
        inputVector.emplace_back(doc_id,1);
        dict_access.lock();
        freq_dictionary[word]=inputVector;
        dict_access.unlock();
    }
}

// Method of find words in text of document
void InvertedIndex::findWords(size_t doc_id, std::string& text){
    std::string word;
    for (int i = 0; i < text.size(); i++) {
        if (word.empty()) {
            word = text[i];
        } else if (text[i] != ' ') {
            word += text[i];
        } else if (text[i] == ' ') {
            addWordToDictionary(doc_id,word);
            word.clear();
        }

        if(i==text.size()-1&&!word.empty())
            addWordToDictionary(doc_id,word);
    }
}

// * Update or fill in database of documents for which
// we will search
// * @param texts_input document contents
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs=std::move(input_docs);
    std::vector<std::thread> threads;
    threads.resize(docs.size());
    for(int i=0;i<docs.size();i++) {
        threads[i]=std::thread(&InvertedIndex::findWords,this,i,std::ref(docs[i]));
    }
    for(auto& t:threads){
        t.join();
    }
}

// * The method determines the number of occurrences of word in
// downloaded database of documents
// * @param word whose frequency of occurrences must be determined
// * @return returns a prepared list with frequency of words
std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    auto it = freq_dictionary.find(word);
    if (it == freq_dictionary.end()) return {};
    return it->second;
}

/** TestInvertedIndexFunctionality class for tests **/
/*
void TestInvertedIndexFunctionality(
        const std::vector<std::string>& docs,
        const std::vector<std::string>& requests,
        const std::vector<std::vector<Entry>>& expected
) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for(auto& request : requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }

    std::cout<<"My result: "<<std::endl;
    for(auto& element:result){
        for (auto& secondElement:element){
            std::cout<<secondElement.doc_id<<" "<<secondElement.count<<std::endl;
        }
    }

    std::cout<<"Expected: "<<std::endl;
    for(auto& element:expected){
        for (auto& secondElement:element){
            std::cout<<secondElement.doc_id<<" "<<secondElement.count<<std::endl;
        }
    }

    ASSERT_EQ(result, expected);
}
*/
/** TESTS **/
/*
//Test #1
TEST(TestCaseInvertedIndex, TestBasic) {
    const std::vector<std::string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock"
    };
    const std::vector<std::string> requests = {"london", "the"};
    const std::vector<std::vector<Entry>> expected = {
            {
                    {0, 1}
            },
            {
                    {0, 1}, {1, 3}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

//Test #2
TEST(TestCaseInvertedIndex, TestBasic2) {
    const std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "Americano Cappuccino"
    };
    const std::vector<std::string> requests = {"milk", "water", "Cappuccino"};
    const std::vector<std::vector<Entry>> expected = {
            {
                    {0, 4}, {1, 1}, {2, 5}
            }, {
                    {0, 3}, {1, 2}, {2, 5}
            }, {
                    {3, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

//Test #3
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const std::vector<std::string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    const std::vector<std::string> requests = {"m", "statement"};
    const std::vector<std::vector<Entry>> expected = {
            {
                    {}
            }, {
                    {1, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
*/
