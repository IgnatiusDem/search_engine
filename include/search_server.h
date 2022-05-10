#pragma once

struct RelativeIndex{
    // id of document
    size_t doc_id;
    // rank of result search by query
    float rank;

    RelativeIndex(size_t _doc_id, float _rank);
    // This operator is necessary for conducting test scenarios
    bool operator==(const RelativeIndex& other) const;
};

class SearchServer {
private:
    InvertedIndex _index;
public:
    // * @param idx reference to the class is passed to class constructor
    // InvertedIndex
    // * for SearchServer can find out frequency of words encountered
    // in query
    SearchServer(InvertedIndex& idx);

    // Method of sorting relevant responses by rank (highest priority)
    // and by doc_id
    void sortWordRelativeIndex(const int& indexRequest,
                               const float& rank,
                               const size_t& doc_id,
                               std::vector<std::vector<RelativeIndex>>&
                               search);

    // Method of adding by word from query to relevant responses
    void addWordRelevantIndex(const int& indexRequest,
                              const std::string& word,
                              int& maxRelevant,
                              std::vector<std::vector<RelativeIndex>>&
                              search);

    // * Method of processing search queries
    // * @param queries_input search queries taken from requests.json
    // * @return returns a sorted list of relevant responses for
    // the specified queries
    std::vector<std::vector<RelativeIndex>> search(const
                            std::vector<std::string>& queries_input,
                                                   int max_responses);

    // * Method to transform to format for save in JSON and returns
    // sorted list of relevant responses
    std::vector<std::vector<std::pair<int, float>>> transformRelativeIndex(
            std::vector<std::vector<RelativeIndex>> search);
};
