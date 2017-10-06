//
// Created by lahmer on 05/10/17.
//

#include "LeafNode.h"

LeafNode::LeafNode(Node *parent, const ndn::Name::Component &component) : Node(parent, component) {

}

ndn::Name LeafNode::getEntryName() {
    Node* currentNode = this;
    ndn::Name name;
    while(currentNode->m_parent != nullptr){
        name.append(currentNode->m_component);
        currentNode = currentNode->m_parent;
    }

    return name;
}
