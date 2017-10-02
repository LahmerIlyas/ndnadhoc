//
// Created by lahmer on 01/08/17.
//
#include "ns3/ndnSIM/model/ndn-common.hpp"
#include <ns3/ndnSIM/ndn-cxx/lp/tags.hpp>
#include "ConsumerDtn.h"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/integer.h"
#include "ns3/double.h"

NS_LOG_COMPONENT_DEFINE("ndn.ConsumerDtn");

namespace ns3{
    namespace ndn {
        NS_OBJECT_ENSURE_REGISTERED(ConsumerDtn);


        TypeId ConsumerDtn::GetTypeId() {
            static TypeId tid =
                    TypeId("ns3::ndn::ConsumerDtn")
                            .SetGroupName("Ndn")
                            .SetParent<App>()
                            .AddConstructor<ConsumerDtn>()
                            .AddAttribute("LifeTime", "LifeTime for interest packet", StringValue("10000s"),
                                                                        MakeTimeAccessor(&ConsumerDtn::m_interestLifeTime), MakeTimeChecker())
                            .AddAttribute("NumberOfRequestedFile","The number of requested files",IntegerValue(),MakeIntegerAccessor(&ConsumerDtn::m_numberOfRequestedFiles),ns3::MakeIntegerChecker<int>())
                            .AddAttribute("Qparameter","",DoubleValue(),MakeDoubleAccessor(&ConsumerDtn::m_q),MakeDoubleChecker<double>())
                            .AddAttribute("Sparameter","",DoubleValue(),MakeDoubleAccessor(&ConsumerDtn::m_s),MakeDoubleChecker<double >())
                            .AddAttribute("ContentCatalogPath"," The path to the content catalog path",StringValue(""),MakeStringAccessor(&ConsumerDtn::m_contentCatalogPath),MakeStringChecker())
            ;


            return tid;
        }

        ConsumerDtn::ConsumerDtn() : m_rand(CreateObject<UniformRandomVariable>()){

        }

        ConsumerDtn::~ConsumerDtn() {

        }

        void ConsumerDtn::OnInterest(shared_ptr<const Interest> interest) {

        }
        int n = 1;
        void ConsumerDtn::OnData(shared_ptr<const Data> data) {
            m_metrics.logDataReieved(*data);

            std::cout<<" Consumer received a Data packet *********"<<data->getTag<nfd::lp::HopCountTag>()->get()<<"   "<<++n<<std::endl;
            //std::terminate();
        }

        void ConsumerDtn::StartApplication() {
            App::StartApplication();
            //std::cout<<" number of requested files  "<<m_numberOfRequestedFiles<<std::endl;
            //std::cout<<" Qparameter "<<m_q<<std::endl;
            //std::cout<<" Sparameter "<<m_s<<std::endl;
            //std::cout<<" Content catalog path "<<m_contentCatalogPath<<std::endl;

            //std::cout<<" consumer Dtn just starte*************d"<<std::endl;
            m_catalog = new ContentCatalog(m_contentCatalogPath);

            m_N = m_catalog->entries.size();
            m_Pcum = std::vector<double>(m_N + 1);
            m_Pcum[0] = 0.0;
            for (uint32_t i = 1; i <= m_N; i++) {
                m_Pcum[i] = m_Pcum[i - 1] + 1.0 / std::pow(i + m_q, m_s);
            }

            for (uint32_t i = 1; i <= m_N; i++) {
                m_Pcum[i] = m_Pcum[i] / m_Pcum[m_N];
            }

            //auto index = selectFileToRequest();
            //requestAFile(index);
            if(m_numberOfRequestedFiles > m_catalog->entries.size()){
                std::cout<<" Error the number of requested files is bigger then the number of files in the catalog"<<std::endl;
                std::terminate();
            }
            for(int i = 0 ; i < m_numberOfRequestedFiles ; i++){
                auto index = selectFileToRequest();
                while(std::find(m_requestedFiles.begin(),m_requestedFiles.end(),index) != m_requestedFiles.end()){
                    index = selectFileToRequest();
                }
                m_requestedFiles.push_back(index);

                std::cout<<" The consumer selected to request the file with index "<<index<<std::endl;
                ns3::Simulator::Schedule(ns3::Seconds(5*i ),&ConsumerDtn::requestAFile,this,index);
                //requestAFile(index);
            }
               //std::terminate();
            //std::cout<<"#####################################"<<std::endl;
        }

        void ConsumerDtn::StopApplication() {
            App::StopApplication();
        }

        unsigned int ConsumerDtn::selectFileToRequest() {
            if(m_numberOfRequestedFiles > 0){
                //we generate a random number
                double rand = m_rand->GetValue(0.0,1.0);
                while(rand == 1.0)
                    rand = m_rand->GetValue(0.0,1.0);

                for(unsigned int j = 1; j <= m_N; j++){
                    if(m_Pcum[j]>= rand){
                        return j - 1;
                    }
                }
            }
            return m_N + 1;
        }

        void ConsumerDtn::requestAFile(unsigned int index) {
            if(index >= m_catalog->entries.size())
                return;
            std::cout<<ns3::Simulator::Now()<<"     The client is requestign the file "<<m_catalog->entries[index].prefix<<
                     "with a number of chunks of "<<m_catalog->entries[index].numberOfChunks<<std::endl;
            //constructing files interests
            for(int i = 0 ; i < m_catalog->entries[index].numberOfChunks;i++){
                Name name(m_catalog->entries[index].prefix);
                name.append(std::to_string(i));
                std::shared_ptr<Interest> interest = std::make_shared<Interest>();
                interest->setName(name);
                interest->setNonce(m_rand->GetValue(0, std::numeric_limits<uint32_t>::max()));

                time::milliseconds interestLifeTime(m_interestLifeTime.GetMilliSeconds());
                interest->setInterestLifetime(interestLifeTime);

                interest->wireEncode();
                m_appLink->onReceiveInterest(*interest);

                m_metrics.logInterestSent(*interest);
            }
        }
    }
}
