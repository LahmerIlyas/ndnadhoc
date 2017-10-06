//
// Created by lahmer on 05/10/17.
//

#ifndef SCENARIO_LEAFNODE_H
#define SCENARIO_LEAFNODE_H


#include "Node.h"
#include "../../DTN/common/tables/citEntry.h"

/**
 * Only leaf nodes are allowed to contain entries of CDT and CIT
 */

class LeafNode : public Node{
public:
    LeafNode(Node *parent, const ndn::Name::Component &component);

    ndn::Name getEntryName();

private:


};


#endif //SCENARIO_LEAFNODE_H
