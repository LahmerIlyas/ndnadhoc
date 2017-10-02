//
// Created by lahmer on 9/27/17.
//

#include "AdditionalLayer.h"
#include "ns3/ndnSIM/NFD/daemon/face/generic-link-service.hpp"
#include "model/ndn-net-device-transport.hpp"
#include <ns3/ndnSIM/model/ndn-l3-protocol.hpp>
#include <ns3/ndnSIM/helper/ndn-fib-helper.hpp>
#include<ns3/ndnSIM/NFD/daemon/fw/forwarder.hpp>
#include "AdditionalLayer.h"
namespace ns3{
    namespace ndn{

        void AdditionalLayer::receiveFromNetDevice(Ptr<NetDevice> device, Ptr<const ns3::Packet> p, uint16_t protocol,
                                                   const Address &from, const Address &to,
                                                   NetDevice::PacketType packetType) {
            //checks if it's a hello message or a normal message
            auto copy = p->Copy();
            //TODO:: this is a dirty hack
            if(p->GetSize() == 1){
                onHelloMessageReceived(from);
            }else{
                //checking if a face that cooresponds to the sender exists
                onHelloMessageReceived(from);

                if(to == device->GetAddress()){
                    ((WifiNetDeviceTransport*)(m_faces.find(from)->second.face->getTransport()))->receivePacket(p);
                }

            }
        }

        AdditionalLayer::AdditionalLayer(Ptr<Node> node, Ptr<WifiNetDevice> device):m_rand(ns3::CreateObject<ns3::UniformRandomVariable>()){
            m_node = node;
            m_device = device;
            m_node->RegisterProtocolHandler(MakeCallback(&AdditionalLayer::receiveFromNetDevice, this),
                                            L3Protocol::ETHERNET_FRAME_TYPE, m_device,
                                            true );

            //addFace(m_device->GetBroadcast());

            ns3::Simulator::Schedule(ns3::Seconds(m_helloMessagesFrequency + m_rand->GetValue(0,0.001)),
                                     &AdditionalLayer::sendHelloMessage,this);

            ns3::Simulator::Schedule(ns3::Seconds(1.0),
                                     &AdditionalLayer::checkForNeigborExistence,this);

        }

        void AdditionalLayer::addFace(Address address) {
            //test if a face with the same address already exists
            if(m_faces.find(address) != m_faces.end()){
                std::cout<<" Error, trying to add an already existing face "<<std::endl;
                std::terminate();
            }

            ::nfd::face::GenericLinkService::Options opts;
            //802.11 already have fragmentation and reassembly why we need it ??
            opts.allowFragmentation = true;
            opts.allowReassembly = true;
            auto linkService = make_unique<::nfd::face::GenericLinkService>(opts);
            auto transport = make_unique<WifiNetDeviceTransport>(m_node, m_device,
                                                                 constructFaceUri(m_device->GetAddress()),
                                                                 constructFaceUri(address));
            transport->setAdreess(address);

            auto face = std::make_shared<Face>(std::move(linkService),std::move(transport));
            Neighbor neighbor;
            neighbor.face = face;
            neighbor.lastReceived = ns3::Simulator::Now();
            m_faces[address] = neighbor;

            Ptr<L3Protocol> ndn = m_node->GetObject<L3Protocol>();
            face->setMetric(1);
            ndn->addFace(face);
            FibHelper::AddRoute(m_node, "/", face, std::numeric_limits<int32_t>::max());
        }

        std::string AdditionalLayer::constructFaceUri(Address address) {
            std::string uri = "netdev://";
            if (Mac48Address::IsMatchingType(address)) {
                uri += "[" + boost::lexical_cast<std::string>(Mac48Address::ConvertFrom(address)) + "]";
            }
            return uri;
        }

        void AdditionalLayer::deleteFace(std::shared_ptr<Face> face) {
            face->close();
        }

        void AdditionalLayer::sendHelloMessage() {
            //std::cout<<" Sending hello messages"<<std::endl;
            uint8_t* content = new uint8_t[1];
            content[0] = 'h';
            Ptr<ns3::Packet> ns3Hello = Create<ns3::Packet>(content,1);
            m_device->Send(ns3Hello, m_device->GetBroadcast(),
                           L3Protocol::ETHERNET_FRAME_TYPE);

            ns3::Simulator::Schedule(ns3::Seconds(m_helloMessagesFrequency),
                                     &AdditionalLayer::sendHelloMessage,this);

            delete content;

        }

        void AdditionalLayer::getFaceList() {
            auto ndn = m_node->GetObject<L3Protocol>();
            auto forwarder = ndn->getForwarder();
            std::cout<<"Face table:"<<std::endl;
            for(const auto& it : forwarder->getFaceTable() ){
                std::cout<<","<<it.getId();
            }
            std::cout<<std::endl;
            //printing the FIB content
            std::cout<<"FIB content:"<<std::endl;
            for(const auto& it : forwarder->getFib()){
                std::cout<<it.getPrefix()<<"       ";
                for(const auto& n : it.getNextHops()){
                    std::cout<<n.getFace().getId()<<"     ";
                }
                std::cout<<std::endl;
            }

            //printing the pit content
            std::cout<<"PIT content;"<<std::endl;
            for(const auto& it : forwarder->getPit()){
                std::cout<< it.getName()<< "  in=";
                for(const auto& in : it.getInRecords()){
                    std::cout<<","<<in.getFace().getId();
                }
                std::cout<<"  out=";
                for(const auto& out : it.getOutRecords()){
                    std::cout<<","<<out.getFace().getId();
                }
                std::cout<<std::endl;
            }
        }

        void AdditionalLayer::onHelloMessageReceived(Address address) {
            //we first check if the neighbor is knwon
            if(m_faces.find(address) != m_faces.end()){
                m_faces[address].lastReceived = ns3::Simulator::Now();
                //std::cout<<" Updating the neighbor  "<<address<< "   "<<ns3::Simulator::Now().GetSeconds()<<std::endl;
            }else{
                //we create a new neighbor
                addFace(address);
            }
        }

        void AdditionalLayer::checkForNeigborExistence() {
            std::vector<Address> facesToBeDeleted;

            for(auto& it : m_faces){
                if(it.second.lastReceived.GetSeconds() + 1.0 < Simulator::Now().GetSeconds())
                    facesToBeDeleted.push_back(it.first);
            }

            for(auto& it : facesToBeDeleted){
                m_faces[it].face->close();
                m_faces.erase(it);
            }

            ns3::Simulator::Schedule(ns3::Seconds(1.0),
                                     &AdditionalLayer::checkForNeigborExistence,this);

        }


    }
}