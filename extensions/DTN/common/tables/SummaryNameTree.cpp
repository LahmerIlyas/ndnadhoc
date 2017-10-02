//
// Created by lahmer on 9/14/17.
//

#include "SummaryNameTree.h"
#include <boost/property_tree/json_parser.hpp>

void SummaryNameTree::pushName(const ndn::Name& name){
    //for each compenent of the name
    //std::cout<<"        Inserting name "<<name<<std::endl;
    auto node = &m_tree;
    for(auto componenet : name){
        std::string c = componenet.toUri();
        //check if the componenet exists if so update the 'node' pointer, otherwise create a componenet and increment
        if(!node->get_child_optional(c)){
            node->push_back(std::make_pair(c,boost::property_tree::ptree()));
            node = &(node->get_child(c));
        }else{
            node = &(node->get_child(c));
        }
    }
}

SummaryNameTree::SummaryNameTree() {

}

SummaryNameTree::SummaryNameTree(const std::string &serializedTree) {
    std::istringstream ss(serializedTree);
    boost::property_tree::json_parser::read_json(ss,m_tree);
}

bool SummaryNameTree::lookUpName(const ndn::Name &name) {
    auto node = &m_tree;
    //TODO there is a bug here fix it (working if all names are same in length)
    for(auto& componenet : name){
        std::string c = componenet.toUri();
        if(!node->get_child_optional(c)){
            return false;
        }
        node = &(node->get_child(c));
    }
    return true;
}

std::string SummaryNameTree::serialize() {
    std::ostringstream buf;
    boost::property_tree::write_json (buf, m_tree, false);
    return buf.str();
}
