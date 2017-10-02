//
// Created by lahmer on 9/27/17.
//

#ifndef SCENARIO_WIFINETDEVICETRANSPORT_H
#define SCENARIO_WIFINETDEVICETRANSPORT_H

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/NFD/daemon/face/transport.hpp"

#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"

#include "ns3/point-to-point-net-device.h"
#include "ns3/channel.h"

namespace ns3 {
    namespace ndn {


        class WifiNetDeviceTransport : public nfd::face::Transport {
        public:
            WifiNetDeviceTransport(Ptr<Node> node, const Ptr<NetDevice> &netDevice,
                                   const std::string &localUri,
                                   const std::string &remoteUri,
                                   ::ndn::nfd::FaceScope scope = ::ndn::nfd::FACE_SCOPE_NON_LOCAL,
                                   ::ndn::nfd::FacePersistency persistency = ::ndn::nfd::FACE_PERSISTENCY_PERSISTENT,
                                   ::ndn::nfd::LinkType linkType = ::ndn::nfd::LINK_TYPE_MULTI_ACCESS);

            ~WifiNetDeviceTransport();

            void receivePacket(Ptr<const ns3::Packet> p);

            const Address &getAdreess() const;

            void setAdreess(const Address &adreess);

        protected:
            virtual void beforeChangePersistency(::ndn::nfd::FacePersistency newPersistency) override;

            virtual void doClose() override;

        private:
            virtual void doSend(Packet &&packet) override;

        private:
            Address m_adreess;                                  //Thsi is the remote node MAC address
            Ptr<NetDevice> m_netDevice;
            Ptr<Node> m_node;
            int m_sent = 0;
        };


    }
}

#endif //SCENARIO_WIFINETDEVICETRANSPORT_H
