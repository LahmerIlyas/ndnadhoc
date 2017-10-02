//
// Created by lahmer on 01/08/17.
//

#ifndef SCENARIO_CONSUMERDTN_H
#define SCENARIO_CONSUMERDTN_H
#include "ns3/ndnSIM/model/ndn-common.hpp"
#include <ns3/ndnSIM/ndn-cxx/lp/tags.hpp>

#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/random-variable-stream.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/utils/ndn-rtt-estimator.hpp"

#include <set>
#include <map>
#include<boost/algorithm/string.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <fstream>


namespace ns3{
    namespace ndn {
        struct ClientMetricsEntry{
            std::string filePrefix;
            double startTime;
            double endTime = -1.0;
            std::map<std::string, std::tuple<double, double,uint32_t >> entries;       //Interest name, sent, received
        };

        struct ClientMetrics{
            void logInterestSent(const Interest& interest){
                auto name = interest.getName();
                auto filePrefix = name.getPrefix(name.size() - 1 ).toUri();

                if(entries.find(filePrefix) == entries.end()){
                    //this is a new file, we need to add it
                    ClientMetricsEntry entry;
                    entry.filePrefix = filePrefix;
                    entry.startTime = ns3::Simulator::Now().GetSeconds();
                    entries[filePrefix] = entry;
                }
                //test if the Interest is already sent
                if(entries[filePrefix].entries.find(name.toUri()) == entries[filePrefix].entries.end()){
                    entries[filePrefix].entries[name.toUri()] = std::make_tuple<double, double,uint32_t>(ns3::Simulator::Now().GetSeconds(),-1.0,0);
                }else{
                    std::cout<<" Error this interest was already sejt "<<std::endl;
                    std::terminate();
                }
            }

            void logDataReieved(const Data& data){
                //test if there is an Interest already sent
                auto name = data.getName();
                auto filePrefix = name.getPrefix(name.size() - 1).toUri();

                if(entries[filePrefix].entries.find(name.toUri()) == entries[filePrefix].entries.end()){
                    std::cout<<" Error received a data packet that wasn't requested "<<std::endl;
                    std::terminate();
                }else{
                    std::get<1>(entries[filePrefix].entries[name.toUri()]) = ns3::Simulator::Now().GetSeconds();
                    std::get<2>(entries[filePrefix].entries[name.toUri()]) = data.getTag<nfd::lp::HopCountTag>()->get();
                }
            }

            std::map<std::string, ClientMetricsEntry> entries;      //<fileName>
        };



        struct ContentCatalogEntry{
            std::string prefix;
            size_t numberOfChunks;
            size_t sizeOfChunk;
        };

        struct ContentCatalog{
            ContentCatalog(const std::string& path){
                std::ifstream infile(path);
                std::string line;
                while(std::getline(infile,line)){
                    std::vector<std::string> parameters;
                    boost::split(parameters,line,boost::is_any_of(" "));
                    if(parameters.size() == 0 )
                        continue;
                    ContentCatalogEntry entry;
                    entry.prefix            = parameters[0];
                    entry.numberOfChunks    = std::stoi(parameters[1]);
                    entry.sizeOfChunk       = std::stoi(parameters[2]);
                    entries.push_back(entry);
                }
                std::cout<<" Parsing Done "<<std::endl;

            }
            std::vector<ContentCatalogEntry> entries;
        };

        class ConsumerDtn : public App{
        public:
            static TypeId
            GetTypeId();

            ConsumerDtn();

            virtual ~ConsumerDtn();

            virtual void
            OnInterest(shared_ptr<const Interest> interest);

            virtual void
            OnData(shared_ptr<const Data> data);

            virtual void
            StartApplication();

            virtual void
            StopApplication();

            unsigned int selectFileToRequest();

            void requestAFile(unsigned int index);

        private:
            double m_frequency = 1.0;           //how much interests per seconds
            Time m_interestLifeTime;
            std::string m_contentCatalogPath ;//= "/home/lahmer/ndnSIM/scenario/catalog.txt";   //my default file
            Ptr<UniformRandomVariable> m_rand;
            ContentCatalog* m_catalog;
            int m_numberOfRequestedFiles;// = 1;
            std::vector<int> m_requestedFiles;

            //these are the parameters of the Zipf distrubtion See ndn-consumer-zipf-mandelbrot.cpp
            uint32_t m_N;               // number of the contents
            double m_q;                 // q in (k+q)^s
            double m_s;                 // s in (k+q)^s
            std::vector<double> m_Pcum; // cumulative probability

        public:
            ClientMetrics m_metrics;


        };

    }
}

#endif //SCENARIO_CONSUMERDTN_H
