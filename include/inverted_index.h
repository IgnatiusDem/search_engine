#pragma once

struct Entry{
    // id of document
    size_t doc_id;
    // count of frequency of use words in document
    size_t count;

    Entry(size_t _doc_id,size_t _count);
    // This operator is necessary for conducting test scenarios
    bool operator==(const Entry& other) const;
    // This operator is necessary for sorting by doc_id
    bool operator<(const Entry& other) const;
};

class InvertedIndex{
private:
    // Storage for text documents
    std::vector<std::string> docs;
    // Storage for frequency of use words in text
    std::map<std::string,std::vector<Entry>>freq_dictionary;

public:
    InvertedIndex()=default;

    // Method of adding or update word to dictionary
    void addWordToDictionary(size_t doc_id, std::string& word);

private:
    void findWords(size_t doc_id, std::string& text);
public:
    // * Update or fill in database of documents for which
    // we will search
    // * @param texts_input document contents
    void UpdateDocumentBase(std::vector<std::string> input_docs);

    // * The method determines the number of occurrences of word in
    // downloaded database of documents
    // * @param word whose frequency of occurrences must be determined
    // * @return returns a prepared list with frequency of words
    std::vector<Entry> GetWordCount(const std::string& word);
};

void TestInvertedIndexFunctionality(
        const std::vector<std::string>& docs,
        const std::vector<std::string>& requests,
        const std::vector<std::vector<Entry>>& expected
);
