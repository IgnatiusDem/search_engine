#include <iostream>
#include <vector>
#include <map>
#include "include/converter_json.h"
#include "include/inverted_index.h"
#include "include/search_server.h"

int main() {
    ConverterJSON converterJson;
    // Create and save to file requests
    std::vector<std::string> request = {"milk water", "sugar"};
    converterJson.addRequests(request);
    std::vector<std::string> newFiles = {"/resources/file001.txt",
                                         "/resources/file002.txt",
                                         "/resources/file003.txt",
                                         "/resources/file004.txt"};
    converterJson.addFiles(newFiles);
    // Load request from file request.json
    request=converterJson.GetRequests();
    // Get text from documents
    const std::vector<std::string> docs = converterJson.GetTextDocuments();
    // Get inverted index from documents
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    // Search by request
    SearchServer srv(idx);
    int max_responses=converterJson.GetResponsesLimit();
    std::vector<std::vector<RelativeIndex>> result = srv.search(request,max_responses);
    //Expected file for check
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
    // Print results
    std::cout<<"My result"<<std::endl;
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
    //Save answers to answers.json
    converterJson.putAnswers(srv.transformRelativeIndex(result));
}
