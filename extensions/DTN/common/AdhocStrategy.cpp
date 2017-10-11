//
// Created by lahmer on 9/27/17.
//

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include <ns3/ndnSIM/ndn-cxx/lp/tags.hpp>
#include "AdhocStrategy.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include"ns3/ndnSIM/NFD/daemon/table/fib-entry.hpp"
#include "../../common/GenericScenario.h"
#include"ns3/ndnSIM/model/ndn-app-link-service.hpp"
#include "ns3/ndnSIM/NFD/daemon/table/pit-entry.hpp"
#include "SimplShaper.h"

namespace nfd
{
    namespace fw {
        AdhocStrategy::AdhocStrategy(nfd::Forwarder &forwarder, const ndn::Name &name) : Strategy(forwarder,name),
                                                                                         m_rand(ns3::CreateObject<ns3::UniformRandomVariable>())
        {

            Name selectedStrategy(GenericScenario::m_strategy);
            if(!selectedStrategy.isPrefixOf(name))
                return ;

            m_shaper = new SimpleShaper::SimpleShaper(nullptr, forwarder);
            m_shaper->startDataSendingLoop();
            m_shaper->startInterestSendingLoop();

            //on contact is triggered if the face is not an app face
            afterAddFace.connect([this](Face& face){
                if(face.getPersistency() == ndn::nfd::FacePersistency::FACE_PERSISTENCY_PERSISTENT && face.getScope() == ndn::nfd::FACE_SCOPE_NON_LOCAL){
                    face.afterReceiveInterest.connect([this, &face] (const Interest& interest) {
                        this->onInterest(interest,face);
                    });
                    face.afterReceiveData.connect([this, &face] (const Data& data) {
                        this->onData(data,face);
                    });
                    m_shaper->onContact(face);

                    onContact(face);
                }else if(face.getLocalUri().toString().compare("appFace://") == 0){
                    face.afterReceiveData.connect([this, &face] (const Data& data) {
                        //this is only for data coming from an App
                        data.setTag(std::make_shared<nfd::lp::HopCountTag>(0));
                        this->onData(data,face);
                    });
                    face.afterReceiveInterest.connect([this, &face] (const Interest& interest) {
                        interest.setTag(std::make_shared<nfd::lp::HopCountTag>(0));
                        this->onInterest(interest,face);
                    });
                }
            });
            //afterAddFace

            beforeRemoveFace.connect([this](Face& face){
                if(face.getPersistency() == ndn::nfd::FacePersistency::FACE_PERSISTENCY_PERSISTENT && face.getScope() == ndn::nfd::FACE_SCOPE_NON_LOCAL){
                    m_shaper->onContactLost(face);
                    onContactLost(face);
                }
            });
        }

        void AdhocStrategy::afterReceiveInterest(const Face &inFace, const Interest &interest,
                                                 const shared_ptr<pit::Entry> &pitEntry) {

            if (inFace.getLocalUri().toString().compare("appFace://") == 0) {

            }else{
                //getForwarder().getPit().erase(&(*pitEntry));
                rejectPendingInterest(pitEntry);
            }
        }




        void AdhocStrategy::beforeSatisfyInterest(const shared_ptr<pit::Entry> &pitEntry, const Face &inFace,
                                                  const Data &data) {
            //log(" before satisfie interest is called ");
            //std::terminate();
        }

        void AdhocStrategy::onContact(const Face &face) {
            log(" Oncontact not implemented yet ");
            std::terminate();
        }

        void AdhocStrategy::onInterest(const Interest &interest, const Face& face) {
            log(" OnIn nterestot implemented yet ");
            std::terminate();

        }

        void AdhocStrategy::onData(const Data &data, const Face& face){
            log(" OnData not implemented yet ");
            std::terminate();
        }

        std::string AdhocStrategy::getNodeName() {
            ns3::Ptr<ns3::Node> node = ns3::NodeList::GetNode(ns3::Simulator::GetContext());
            std::string name = ns3::Names::FindName(node);
            return name;
        }

        void AdhocStrategy::log(std::string msg) {
            std::cout<<ns3::Simulator::Now().GetSeconds()<<"  "<<getNodeName()<<"  "<<msg<<std::endl;
        }

        void AdhocStrategy::onContactLost(const Face &face) {
            log(" OnDataDrop not implemented yet ");
            std::terminate();
        }


        std::string AdhocStrategy::getMAcAddress() {
            ns3::Ptr<ns3::Node> node = ns3::NodeList::GetNode(ns3::Simulator::GetContext());
            std::string address = boost::lexical_cast<std::string>(ns3::Mac48Address::ConvertFrom(node->GetDevice(0)->GetAddress()));
            return address;
        }

        void AdhocStrategy::onAppInterest(const Data &data, const Face &face) {

        }

        pair<bool, Face* > AdhocStrategy::canTheInterestBeSatisfiedByLocalApp(const Interest &interest) {
            /**
             * When profiling, this method was consuming lot of ressources, so we use caching
             * to make simulcation runs faster
             */
            /*if(m_appPrefixes != nullptr){
                auto it = m_appPrefixes->find(interest.getName());
                if(it == m_appPrefixes->end()){
                    return std::make_pair(false, nullptr);
                }
                return std::make_pair(true, it->second);
            }
            m_appPrefixes = new std::unordered_map<ndn::Name, nfd::Face*>();
            */


            for (nfd::Fib::const_iterator entry = getForwarder().getFib().begin(); entry != getForwarder().getFib().end(); entry++){
                //log(" testing for " + ((Name)entry->getPrefix()).toUri());
                if(((Name)entry->getPrefix()).toUri().size() > 1){
                    //checking if it's not a localhost face
                    if(((Name)entry->getPrefix()).get(0).toUri().compare("localhost") != 0){
                        const nfd::fib::NextHopList& nexthops = entry->getNextHops();
                        if(nexthops.size() == 1){
                            Name prefix = ((Name)entry->getPrefix());
                            Face* apFace = &nexthops.at(0).getFace();
                            if(prefix.isPrefixOf(interest.getName())){
                                return std::make_pair(true, apFace);
                            }
                        }
                    }
                }
            }
            return std::make_pair(false, nullptr);
        }

        Forwarder &AdhocStrategy::getForwarder() {
            ns3::Ptr<ns3::Node> node = ns3::NodeList::GetNode(ns3::Simulator::GetContext());
            auto ndnLayer = node->GetObject<ns3::ndn::L3Protocol>();
            return *ndnLayer->getForwarder();
        }

        void AdhocStrategy::beforeExpirePendingInterest(const shared_ptr<pit::Entry> &pitEntry) {
            log( " Before expire pending Interest is called " + pitEntry->getName().toUri());
        }

    }

}