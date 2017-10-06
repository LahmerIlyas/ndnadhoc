//
// Created by lahmer on 04/10/17.
//

#ifndef SCENARIO_NODE_H
#define SCENARIO_NODE_H



#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <map>
#include <unordered_map>
#include <ns3/ndn-all.hpp>
#include <core/city-hash.hpp>

class Node {
public:
    Node(Node* parent, const ndn::Name::Component& component);

    Node* lookupNode(const ndn::Name::Component& component, const uint64& hash);

    std::string& getComponenetName();

    void insertNode(Node* node, uint64 hashValue);



public:
    Node* m_parent;

    std::string m_component;

    std::unordered_multimap<uint64_t, Node*> m_sons;

};


#endif //SCENARIO_NODE_H
