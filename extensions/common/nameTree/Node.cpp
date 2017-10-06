//
// Created by lahmer on 04/10/17.
//

#include "Node.h"
#include<ns3/ndn-all.hpp>
#include <core/city-hash.hpp>

Node *Node::lookupNode(const ndn::Name::Component &c, const uint64& hashValue) {
    auto component = c.toUri();
    auto range = m_sons.equal_range(hashValue);
    for(auto it = range.first; it != range.second; it++){
        if(it->second->getComponenetName() == component)
            return it->second;
    }
    return nullptr;
}

void Node::insertNode(Node *node, uint64 hashValue) {
    m_sons.insert(std::make_pair(hashValue,node));
}

std::string& Node::getComponenetName() {
    return m_component;
}

Node::Node(Node *parent, const ndn::Name::Component &component) {
    m_component = component.toUri();
    m_parent = parent;
}
