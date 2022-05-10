#include <iostream>
#include <vector>
#include <map>
#include "include/converter_json.h"
#include "include/inverted_index.h"
//#include "gtest/gtest.h"
#include "include/search_server.h"

/** Methods for RelativeIndex struct **/

RelativeIndex::RelativeIndex(size_t _doc_id, float _rank):
                            doc_id(_doc_id), rank(_rank) {}

// This operator is necessary for conducting test scenarios
bool RelativeIndex::operator==(const RelativeIndex &other) const {
    return (doc_id == other.doc_id && rank == other.rank);
}

/** Methods for SearchServer class **/

// * @param idx reference to the class is passed to class constructor
// InvertedIndex
// * for SearchServer can find out frequency of words encountered
// in query
SearchServer::SearchServer(InvertedIndex& idx) : _index(idx){}

// Method of sorting relevant responses by rank (highest priority)
// and by doc_id
void SearchServer::sortWordRelativeIndex(const int &indexRequest,
                                         const float &rank,
                                         const size_t &doc_id,
                                         std::vector
                                         <std::vector<RelativeIndex>>&
                                         search) {
    int i=(int)search[indexRequest].size()/2;
    bool isOver=false;

    while(!isOver){
        if (rank < search[indexRequest][i].rank) {
            i++;
        }else if (rank == search[indexRequest][i].rank &&
                  doc_id > search[indexRequest][i].doc_id) {
            i++;
        } else if ((i - 1) >= 0 && rank >
                                   search[indexRequest][i - 1].rank) {
            i--;
        } else if ((i - 1) >= 0 &&
                   rank == search[indexRequest][i-1].rank &&
                   doc_id < search[indexRequest][i - 1].doc_id) {
            i--;
        } else if (i < search[indexRequest].size() - 1) {
            search[indexRequest].insert(search[indexRequest].begin() + i,
                                        RelativeIndex(doc_id, rank));
            isOver = true;
        } else {
            search[indexRequest].push_back(RelativeIndex(doc_id, rank));
            isOver = true;
        }
    }
}

// Method of adding by word from query to relevant responses
void SearchServer::addWordRelevantIndex(const int& indexRequest,
                                        const std::string& word,
                                        int& maxRelevant,
                                        std::vector<std::vector
                                        <RelativeIndex>>&
                                        search){
    //the frequency of word
    std::vector<Entry> wordCount=_index.GetWordCount(word);

    if(indexRequest+1>search.size())
        search.resize(indexRequest+1);

    for(auto wordCountElement:wordCount) {
        bool isExist=false;
        // adding for existing documents
        for(auto it=search[indexRequest].begin();
            it!=search[indexRequest].end();it++){
            if(wordCountElement.doc_id==it->doc_id){
                it->rank+=(float)wordCountElement.count;
                if(it->rank>(float)maxRelevant){
                    maxRelevant = (int) it->rank;
                    float rank = it->rank;
                    size_t doc_id = it->doc_id;
                    search[indexRequest].erase(it);
                    search[indexRequest].insert(
                            search[indexRequest].begin(),
                            RelativeIndex(doc_id, rank));
                    it=search[indexRequest].end()-1;
                }else{
                    float rank = it->rank;
                    size_t doc_id = it->doc_id;
                    search[indexRequest].erase(it);
                    sortWordRelativeIndex(indexRequest, rank,
                                          doc_id, search);
                    it=search[indexRequest].end()-1;
                }
                isExist=true;
            }
        }
        // adding for NOT existing documents
        if (!isExist&&wordCountElement.count > maxRelevant) {
            maxRelevant = (int)wordCountElement.count;
            search[indexRequest].insert(
                    search[indexRequest].begin(),
                    RelativeIndex (wordCountElement.doc_id,
                                   (float)wordCountElement.count));
        } else if(!isExist) {
            sortWordRelativeIndex(indexRequest,
                                  (float)wordCountElement.count,
                                  wordCountElement.doc_id,
                                  search);
        }
    }
}

// * Method of processing search queries
// * @param queries_input search queries taken from requests.json
// * @return returns a sorted list of relevant responses for
// the specified queries
std::vector<std::vector<RelativeIndex>> SearchServer::search
            (const std::vector<std::string> &queries_input,
             int max_responses) {
    // maximum absolute relevant number from all documents
    int maxRelevant=0;
    // temporary storage of data returned by the method
    std::vector<std::vector<RelativeIndex>> search;

    for(int i=0;i<queries_input.size();i++){
        // temporary storage for word from query
        std::string word;
        // get words from queries and after create relevant index
        for(int j=0;j<queries_input[i].size();j++){
            if(word.empty()) {
                word = queries_input[i][j];
            }else if(queries_input[i][j] != ' ') {
                word += queries_input[i][j];
            }else if(queries_input[i][j] == ' ') {
                addWordRelevantIndex(i,word,maxRelevant,search);
                word.clear();
            }

            if(j==queries_input[i].size()-1&&!word.empty()) {
                addWordRelevantIndex(i,word,maxRelevant,search);
            }
        }
    }

    //Transform sum absolute relevant numbers for document
    //to relative relevance
    for(auto& firstSearch:search){
        for(int i=firstSearch.size()-1;i>=0;i--){
            if(i>=max_responses) {
                firstSearch.erase(firstSearch.begin()+i);
            }else {
                firstSearch[i].rank/=maxRelevant;
            }
        }
    }

    return search;
}

// * Method to transform to format for save in JSON and returns
// sorted list of relevant responses
std::vector<std::vector<std::pair<int, float>>>SearchServer::
transformRelativeIndex(std::vector<std::vector<RelativeIndex>> search){
    std::vector<std::vector<std::pair<int, float>>> result;
    int i=0;
    for(auto first:search){
        result.resize(i+1);
        for (auto second:first){
            result[i].push_back(std::pair<int, float>(second.doc_id,second.rank));
        }
        i++;
    }
    return result;
}

/**TESTS**/
/*
//Test #1
TEST(TestCaseSearchServer, TestSimple) {
    const std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "Americano Cappuccino"
    };
    const std::vector<std::string> request = {"milk water", "sugar"};
    const std::vector<std::vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.7},
                    {1, 0.3}
            },
            {
                    {}
            }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(request,5);
    std::cout<<"My"<<std::endl;
    int i=0;
    for(auto& element:result){
        std::cout<<i<<": "<<std::endl;
        for (auto& secondElement:element){
            if(!element.empty()) std::cout<<secondElement.doc_id<<" "<<secondElement.rank<<std::endl;
        }
        i++;
    }
    std::cout<<"Correct"<<std::endl;
    i=0;
    for(auto& element:expected){
        std::cout<<i<<": "<<std::endl;
        for (auto& secondElement:element){
            if(!element.empty()) std::cout<<secondElement.doc_id<<" "<<secondElement.rank<<std::endl;
        }
        i++;
    }
    ConverterJSON converter;
    converter.putAnswers(srv.transformRelativeIndex(result));
    ASSERT_EQ(result, expected);
}

//Test #2 (expected file not correct)
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
            {
                    {7, 1},
                    {14, 1},
                    {0, 0.4},
                    {1, 0.4},
                    {2, 0.4}
            }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(request,5);
    std::cout<<"My result: "<<std::endl;
    int i=0;
    for(auto& element:result){
        std::cout<<i<<": "<<std::endl;
        for (auto& secondElement:element){
            if(!element.empty()) std::cout<<secondElement.doc_id<<" "<<secondElement.rank<<std::endl;
        }
        i++;
    }

    std::cout<<"Expected: "<<std::endl;
    i=0;
    for(auto& element:expected){
        std::cout<<i<<": "<<std::endl;
        for (auto& secondElement:element){
            if(!element.empty()) std::cout<<secondElement.doc_id<<" "<<secondElement.rank<<std::endl;
        }
        i++;
    }

    ASSERT_EQ(result, expected);
}
*/