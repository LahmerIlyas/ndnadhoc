//
// Created by lahmer on 9/27/17.
//

#ifndef SCENARIO_WIFISTACKHELPER_H
#define SCENARIO_WIFISTACKHELPER_H
#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/ptr.h"
#include "ns3/object-factory.h"
#include "ns3/node.h"
#include "ns3/node-container.h"

#include "ns3/ndnSIM/helper/ndn-face-container.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-strategy-choice-helper.hpp"

namespace ns3 {
    namespace ndn {
/**
 * This class is responsible for installing the ndn stack on all nodes
 *
 */
        class WifiStackHelper {
        public:
            WifiStackHelper();

            ~WifiStackHelper();

            void Install(NodeContainer container);

            std::string constructFaceUri(Ptr<NetDevice> device);

        private:
            ObjectFactory m_ndnFactory;
            ObjectFactory m_contentStoreFactory;
        };

    }
}
#endif //SCENARIO_WIFISTACKHELPER_H
