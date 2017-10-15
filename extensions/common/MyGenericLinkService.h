//
// Created by lahmer on 09/10/17.
//

#ifndef SCENARIO_MYGENERICLINKSERVICE_H
#define SCENARIO_MYGENERICLINKSERVICE_H

#include "core/common.hpp"
#include "core/logger.hpp"

#include <ns3/ndnSIM/NFD/daemon/face/generic-link-service.hpp>

/***
 * This class is here only because the NDN
 * to packets and because adding a field in the interest.hpp and interest.cpp files requires changing the code
 * of ndnSIM which violate the purpose of this package.
 */


namespace nfd {
    namespace face {

        class MyGenericLinkService : public GenericLinkService{

        };

    }
}
#endif //SCENARIO_MYGENERICLINKSERVICE_H
