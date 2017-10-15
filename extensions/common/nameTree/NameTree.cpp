//
// Created by lahmer on 05/10/17.
//

#include "NameTree.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
NameTree::NameTree() {
    ndn::Name::Component name;
    m_root = new Node(nullptr,name);
}

LeafNode *NameTree::insertEntry(const ndn::Name &name) {
    Node* current = m_root;

    for(auto it = name.begin(); it != name.end(); it++){
        uint64 hashValue = CityHash64((const char *) it->wire(), it->size());
        Node* index = current->lookupNode(*it, hashValue);
        if(index == nullptr){
            //if this is the last component, we insert a leafNode, Otherwise a normal Node is used
            if(it + 1 == name.end()){
                LeafNode* leaf = new LeafNode(current, *it);
                current->insertNode(leaf,hashValue);
                m_leafs.insert(std::make_pair(hashValue,leaf));
                return leaf;
            }else{
                Node* node = new Node(current,*it);
                current->insertNode(node, hashValue);
                current = node;
            }
        }else{
            current = index;
            if(it + 1 == name.end()){
                return (LeafNode*) current;
            }
        }

    }
    return nullptr;
}

LeafNode *NameTree::lookupEntry(const ndn::Name &name) {
    Node* current = m_root;
    for(auto it = name.begin(); it != name.end(); it++){
        uint64 hashValue = CityHash64((const char *) it->wire(), it->size());
        Node* index = current->lookupNode(*it, hashValue);
        if(index == nullptr)
            return nullptr;
        current = index;
    }

    if(current == m_root)
        return nullptr;

    return (LeafNode*)current;

}

NameTree::NameTree(const std::string &serilizedNameTree) {

}

std::string NameTree::serialize() {
    return std::string();
}

void NameTree::eraseEntry(const ndn::Name &name) {


}

std::string NameTree::getJsonTreeRepresentation(bool hasCDT, bool hasCIT) {
    boost::property_tree::ptree tree;

    for(auto& it : m_leafs){
        auto node = &tree;
        //we now insert the name into the boost tree
        if(it.second->hasCDTEntry() && hasCDT or it.second->hasCITEntry() && hasCIT){
            auto name = it.second->getEntryName();
            //for each componenet of the name we add an entry
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
    }

    std::ostringstream buf;
    boost::property_tree::write_json (buf, tree, false);
    return buf.str();
}
