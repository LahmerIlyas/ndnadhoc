//
// Created by lahmer on 09/08/17.
//

#ifndef SCENARIO_PRODUCERDTN_H
#define SCENARIO_PRODUCERDTN_H

#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/random-variable-stream.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/utils/ndn-rtt-estimator.hpp"
#include "ConsumerDtn.h"

#include <set>
#include <map>
#include<boost/algorithm/string.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <fstream>

namespace ns3{
    namespace ndn{
        class ProducerDtn : public App {
        public:
            static TypeId
            GetTypeId();

            ProducerDtn();

            ~ProducerDtn
                    ();

            virtual void
            OnInterest(shared_ptr<const Interest> interest);

            virtual void
            OnData(shared_ptr<const Data> data);

            virtual void
            StartApplication();

            virtual void
            StopApplication();

        private:

            std::string m_contentCatalogPath;// = "/home/lahmer/ndnSIM/scenario/catalog.txt";   //my default file
            Ptr<UniformRandomVariable> m_rand;
            ContentCatalog* m_catalog;
            size_t m_dataSize = 100;

            Time m_freshness;
            uint32_t m_signature;
            Name m_keyLocator ;

        };

    }
}


#endif //SCENARIO_PRODUCERDTN_H
