//
// Created by lahmer on 06/10/17.
//

#include <iostream>
#include <ns3/ndn-all.hpp>
#include "../extensions/common/nameTree/NameTree.h"
#include "../extensions/common/DataBaseUtils.h"

using namespace std;


int main(){
    DataBaseUtils util("fzdgvgre");
    util.addCouple("hello", " fujczdf");
    util.addCouple("drop", 1651651);

    std::cout<<util.constructInsertQuery()<<std::endl;
    return 0;
}
