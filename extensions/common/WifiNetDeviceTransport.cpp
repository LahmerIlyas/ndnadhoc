//
// Created by lahmer on 9/27/17.
//


#include "WifiNetDeviceTransport.h"
#include <ns3/ndnSIM/model/ndn-block-header.hpp>
#include <ns3/ndnSIM/model/ndn-l3-protocol.hpp>
#include <ns3/ptr.h>

namespace ns3 {
    namespace ndn {


        void WifiNetDeviceTransport::beforeChangePersistency(::ndn::nfd::FacePersistency newPersistency) {

        }

        void WifiNetDeviceTransport::doClose() {
            this->setState(::nfd::face::TransportState::CLOSED);
        }

        void WifiNetDeviceTransport::doSend(::nfd::face::Transport::Packet &&packet) {
            BlockHeader header(packet);
            Ptr<ns3::Packet> ns3Packet = Create<ns3::Packet>();
            ns3Packet->AddHeader(header);
            m_netDevice->Send(ns3Packet, m_adreess,
                              L3Protocol::ETHERNET_FRAME_TYPE);

        }

        WifiNetDeviceTransport::WifiNetDeviceTransport(Ptr<Node> node, const Ptr<NetDevice> &netDevice,
                                                       const std::string &localUri, const std::string &remoteUri,
                                                       ::ndn::nfd::FaceScope scope, ::ndn::nfd::FacePersistency persistency,
                                                       ::ndn::nfd::LinkType linkType):m_netDevice(netDevice),m_node(node) {

            m_adreess = m_netDevice->GetBroadcast();        //by default the address is set to the broadcast address
            this->setLocalUri(FaceUri(localUri));
            this->setRemoteUri(FaceUri(remoteUri));
            this->setScope(scope);
            this->setPersistency(persistency);
            this->setLinkType(linkType);
            this->setMtu(m_netDevice->GetMtu());
        }

        WifiNetDeviceTransport::~WifiNetDeviceTransport() {

        }

        void WifiNetDeviceTransport::receivePacket(Ptr<const ns3::Packet> p) {
            Ptr<ns3::Packet> packet = p->Copy();
            BlockHeader header;
            try{
                packet->RemoveHeader(header);

            }catch (boost::exception& e ){
                /**
                 * When the size of the data packet is big an exception is thrown sometimes, you need to debug its cause
                 */
                std::cout<<" An exception is thrown, check the wifi net device we abort "<<std::endl;
                return;
            }
            auto nfdPacket = Packet(std::move(header.getBlock()));
            this->receive(std::move(nfdPacket));
        }

        const Address &WifiNetDeviceTransport::getAdreess() const {
            return m_adreess;
        }

        void WifiNetDeviceTransport::setAdreess(const Address &adreess) {
            m_adreess = adreess;
        }


    }
}