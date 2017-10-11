//
// Created by lahmer on 09/10/17.
//

#ifndef SCENARIO_STRINGTAG_H
#define SCENARIO_STRINGTAG_H


#include <string>
#include <ns3/ndnSIM/ndn-cxx/tag.hpp>

namespace ndn{
    namespace lp{
        typedef SimpleTag<std::string, 14> NameTreeTag;
    }
}

#endif //SCENARIO_STRINGTAG_H
