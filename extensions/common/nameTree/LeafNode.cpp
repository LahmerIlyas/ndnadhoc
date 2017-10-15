//
// Created by lahmer on 05/10/17.
//

#include "LeafNode.h"

LeafNode::LeafNode(Node *parent, const ndn::Name::Component &component) : Node(parent, component) {

}

ndn::Name LeafNode::getEntryName() {
    Node* currentNode = this;
    std::vector<std::string> componenets;

    ndn::Name name;
    while(currentNode->m_parent != nullptr){
        componenets.push_back(currentNode->m_component);
        currentNode = currentNode->m_parent;
    }

    for(auto it = componenets.rbegin(); it != componenets.rend(); it++){
        name.append(*it);
    }

    return name;
}

bool LeafNode::hasCITEntry() {
    return m_citEntry.get() != nullptr;
}

bool LeafNode::hasCDTEntry() {
    return m_cdtEntry.get() != nullptr;
}

cdtEntry &LeafNode::getCDTEntry() {
    return *m_cdtEntry;
}

citEntry &LeafNode::getCITEntry() {
    return *m_citEntry;
}

void LeafNode::eraseCITEntry() {
    m_citEntry.reset();
}

void LeafNode::eraseCDTEntry() {
    m_cdtEntry.reset();
}

void LeafNode::addCITEntry(std::shared_ptr<citEntry> entry) {
    m_citEntry = entry;
}

void LeafNode::addCDTEntry(std::shared_ptr<cdtEntry> entry) {
    m_cdtEntry = entry;
}
