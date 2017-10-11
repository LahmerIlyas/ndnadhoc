//
// Created by lahmer on 05/10/17.
//

#include "NameTree.h"
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
