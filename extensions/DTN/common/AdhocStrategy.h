//
// Created by lahmer on 9/27/17.
//

#ifndef SCENARIO_ADHOCSTRATEGY_H
#define SCENARIO_ADHOCSTRATEGY_H

#include <fw/strategy.hpp>
#include "ns3/random-variable-stream.h"
#include "AbstractShaper.h"


namespace nfd
{
    namespace fw {
/**
 *      This is the base class for all DTN strategies, it provides the simple interface for all the strategies
 *      -   OnContact is called when a new neighbor is dicovered
 *      -   OnInterest is called when a new interest is received
 *      -   OnData is called when a new Data packet is received
 */
        class AdhocStrategy : public Strategy {
        public:
            explicit
            AdhocStrategy(Forwarder &forwarder, const Name &name);


            virtual void
            afterReceiveInterest(const Face &inFace, const Interest &interest,
                                 const shared_ptr<pit::Entry> &pitEntry) override;

            virtual void
            beforeSatisfyInterest(const shared_ptr<pit::Entry> &pitEntry,
                                  const Face &inFace, const Data &data) override;

            virtual void onContact(const Face &face);

            virtual void onContactLost(const Face &face);

            virtual void onInterest(const Interest &interest, const Face& face);

            virtual void onData(const Data &data, const Face& face);

            virtual void onAppInterest(const Data &data, const Face& face);

            virtual void
            beforeExpirePendingInterest(const shared_ptr<pit::Entry>& pitEntry);

            Forwarder& getForwarder();

            std::string getNodeName();

            void log(std::string msg);

            std::string getMAcAddress();

            std::pair<bool,Face*> canTheInterestBeSatisfiedByLocalApp(const Interest& interest);

        protected:

            ns3::Ptr<ns3::UniformRandomVariable> m_rand;

            AbstractShaper* m_shaper;

        private:
            std::unordered_map<ndn::Name, nfd::Face*>* m_appPrefixes;
        };
    }
}

#endif //SCENARIO_ADHOCSTRATEGY_H
