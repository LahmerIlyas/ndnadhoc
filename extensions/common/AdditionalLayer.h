//
// Created by lahmer on 9/27/17.
//

#ifndef SCENARIO_ADDITIONALLAYER_H
#define SCENARIO_ADDITIONALLAYER_H

#include <ns3/wifi-module.h>
#include "WifiNetDeviceTransport.h"

namespace ns3{
    namespace ndn {

        struct Neighbor {
            std::shared_ptr<Face> face;
            Time lastReceived;
        };

        class AdditionalLayer {
        public:
            AdditionalLayer(Ptr<Node> node, Ptr<WifiNetDevice> device);
        private:


            void receiveFromNetDevice(Ptr<NetDevice> device,
                                      Ptr<const ns3::Packet> p,
                                      uint16_t protocol,
                                      const Address& from, const Address& to,
                                      NetDevice::PacketType packetType);

        private:
            void addFace(Address address);

            void deleteFace(std::shared_ptr<Face> face);

            std::string constructFaceUri(Address address);

            void sendHelloMessage();

            void onHelloMessageReceived(Address address);

            void checkForNeigborExistence();

            void getFaceList();

        private:
            Ptr<WifiNetDevice> m_device;
            Ptr<Node>          m_node;
            std::map<Address, Neighbor> m_faces;
            double m_helloMessagesFrequency = 0.5;          //By default each 0.5 seconds a hello message is sent
            ns3::Ptr<ns3::UniformRandomVariable> m_rand;
        };


    }
}

#endif //SCENARIO_ADDITIONALLAYER_H
