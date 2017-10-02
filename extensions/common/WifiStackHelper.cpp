//
// Created by lahmer on 9/27/17.
//

#include "WifiStackHelper.h"

#include <ns3/wifi-module.h>
#include "WifiStackHelper.h"
#include "ns3/ndnSIM/NFD/daemon/face/generic-link-service.hpp"
#include "model/ndn-net-device-transport.hpp"
#include "utils/ndn-time.hpp"
#include "utils/dummy-keychain.hpp"
#include "WifiNetDeviceTransport.h"
#include "AdditionalLayer.h"

namespace ns3 {
    namespace ndn {


        WifiStackHelper::WifiStackHelper() {
            ::ndn::time::setCustomClocks(std::make_shared<ns3::ndn::time::CustomSteadyClock>(),
                                         std::make_shared<ns3::ndn::time::CustomSystemClock>());


            m_ndnFactory.SetTypeId("ns3::ndn::L3Protocol");
            m_contentStoreFactory.SetTypeId("ns3::ndn::cs::Lru");

        }

        WifiStackHelper::~WifiStackHelper() {

        }

        void WifiStackHelper::Install(NodeContainer container) {
            for(auto& node : container){
                Ptr<L3Protocol> ndn = m_ndnFactory.Create<L3Protocol>();

                //we aren't using the built-in content store so it doesn't matter
                ndn->getConfig().put("tables.cs_max_packets", 1);
                ndn->getConfig().put("ndnSIM.disable_strategy_choice_manager", false);
                ndn->AggregateObject(m_contentStoreFactory.Create<ContentStore>());
                node->AggregateObject(ndn);
                //the node should have only one wifiNetDevice
                if(node->GetNDevices() != 1){
                    std::cout<<" Error, only one NetDevice is allowed "<<std::endl;
                    std::terminate();
                }

                //test if it's a WifiNetDevice
                Ptr<WifiNetDevice> device = DynamicCast<WifiNetDevice>(node->GetDevice(0));
                if(device == 0){
                    std::cout<<" Error, only wifi net devices are allowed"<<std::endl;
                    std::terminate();
                }

                //create the additional layer that will take care of the rest
                auto additionalLayer = new AdditionalLayer(node,device);
            }
        }

        std::string WifiStackHelper::constructFaceUri(Ptr<NetDevice> device) {
            std::string uri = "netdev://";
            Address address = device->GetAddress();
            if (Mac48Address::IsMatchingType(address)) {
                uri += "[" + boost::lexical_cast<std::string>(Mac48Address::ConvertFrom(address)) + "]";
            }
            return uri;
        }

    }
}