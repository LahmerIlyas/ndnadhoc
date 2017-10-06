//
// Created by lahmer on 06/10/17.
//

#include <iostream>
#include <ns3/ndn-all.hpp>
#include "../extensions/common/nameTree/NameTree.h"

using namespace std;

struct ContentCatalogEntry{
    std::string prefix;
    size_t numberOfChunks;
    size_t sizeOfChunk;
};

struct ContentCatalog{
    ContentCatalog(const std::string& path){
        std::ifstream infile(path);
        std::string line;
        while(std::getline(infile,line)){
            std::vector<std::string> parameters;
            boost::split(parameters,line,boost::is_any_of(" "));
            if(parameters.size() == 0 )
                continue;
            ContentCatalogEntry entry;
            entry.prefix            = parameters[0];
            entry.numberOfChunks    = std::stoi(parameters[1]);
            entry.sizeOfChunk       = std::stoi(parameters[2]);
            entries.push_back(entry);
        }
        std::cout<<" Parsing Done "<<std::endl;

    }
    std::vector<ContentCatalogEntry> entries;
};


int main(){
    NameTree tree;
    ContentCatalog catalog("/home/lahmer/tutorial/ndnSIM/scenario/temp/bxrzvwyxjizftfvlullv");
    uint64 n = 0;
    //constructing all the names
    for(auto& it : catalog.entries){
        for(int i = 0; i < it.numberOfChunks; i++){
            ndn::Name name(it.prefix);
            name.append(std::to_string(i));
            tree.insertEntry(name);
            n++;
        }
    }
    std::cout<<n<<std::endl;

    /*int correct = 0;
    int wrong = 0;
    for(auto& it : names){
        if(tree.lookupEntry(it) == nullptr)
            wrong++;
        else
            correct++;
    }
    std::cout<<" coorect = "<<correct<<" wrong = "<<wrong<<"   leafs "<<tree.m_leafs.size()<<std::endl;


    for(auto& it : tree.m_leafs){
        std::cout<<it.second->getEntryName()<<std::endl;
    }
    //std::cout<<" We ended all"<<std::endl;
*/



    return 0;
}
