//
// Created by lahmer on 09/08/17.
//

#include "ProducerDtn.h"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

#include "model/ndn-l3-protocol.hpp"
#include <ns3/ndnSIM/helper/ndn-fib-helper.hpp>
#include <ns3/ndnSIM/ndn-cxx/lp/tags.hpp>

namespace ns3{
    namespace ndn{
        NS_OBJECT_ENSURE_REGISTERED(ProducerDtn);

        TypeId ProducerDtn::GetTypeId() {
            static TypeId tid =
                    TypeId("ns3::ndn::ProducerDtn")
                            .SetGroupName("Ndn")
                            .SetParent<App>()
                            .AddConstructor<ProducerDtn>()
                            .AddAttribute("Freshness", "Freshness of data packets, if 0, then unlimited freshness",
                                          TimeValue(Seconds(0)), MakeTimeAccessor(&ProducerDtn::m_freshness),
                                          MakeTimeChecker())
                            .AddAttribute(
                                    "Signature",
                                    "Fake signature, 0 valid signature (default), other values application-specific",
                                    UintegerValue(0), MakeUintegerAccessor(&ProducerDtn::m_signature),
                                    MakeUintegerChecker<uint32_t>())

                            .AddAttribute("KeyLocator",
                                          "Name to be used for key locator.  If root, then key locator is not used",
                                          NameValue(), MakeNameAccessor(&ProducerDtn::m_keyLocator), MakeNameChecker())

                            .AddAttribute("ContentCatalogPath"," The path to the content catalog path",StringValue(""),
                                          MakeStringAccessor(&ProducerDtn::m_contentCatalogPath),MakeStringChecker())

            ;
            return tid;
        }
        void ProducerDtn::OnInterest(shared_ptr<const Interest> interest) {
            App::OnInterest(interest); // tracing inside
            Name dataName(interest->getName());

            auto data = make_shared<Data>();
            data->setName(dataName);
            data->setFreshnessPeriod(::ndn::time::milliseconds(m_freshness.GetMilliSeconds()));
            //get the data size
            for(auto it = m_catalog->entries.begin(); it != m_catalog->entries.end();it++){
                if(it->prefix.compare(interest->getName().getPrefix(interest->getName().size() - 1).toUri()) == 0){
                    m_dataSize = it->sizeOfChunk;
                    //std::cout<<" The data size is "<<m_dataSize<<std::endl;
                    break;
                }
                if(it + 1 == m_catalog->entries.end()){
                    std::cout<<" Error the Data size wasn't found "<<std::endl;
                    std::terminate();
                }
            }

            data->setContent(make_shared< ::ndn::Buffer>(m_dataSize));

            Signature signature;
            SignatureInfo signatureInfo(static_cast< ::ndn::tlv::SignatureTypeValue>(255));

            if (m_keyLocator.size() > 0) {
                signatureInfo.setKeyLocator(m_keyLocator);
            }

            signature.setInfo(signatureInfo);
            signature.setValue(::ndn::makeNonNegativeIntegerBlock(::ndn::tlv::SignatureValue, m_signature));

            data->setSignature(signature);
            data->setTag(std::make_shared<nfd::lp::HopCountTag>(0));

            // to create real wire encoding
            data->wireEncode();

            m_transmittedDatas(data, this, m_face);
            m_appLink->onReceiveData(*data);
        }

        void ProducerDtn::OnData(shared_ptr<const Data> data) {
            App::OnData(data);
        }

        void ProducerDtn::StartApplication() {
            App::StartApplication();
            m_catalog = new ContentCatalog(m_contentCatalogPath);
            for(auto& it : m_catalog->entries){
                //std::cout<<" Adding fib entry " <<it.prefix<<std::endl;
                ndn::Name name(it.prefix);
                ns3::ndn::FibHelper::AddRoute(GetNode(),name.getPrefix(1),m_face,0);
            }
        }

        ProducerDtn::~ProducerDtn() {

        }

        ProducerDtn::ProducerDtn() {

        }

        void ProducerDtn::StopApplication() {
            App::StopApplication();
        }
    }
}
