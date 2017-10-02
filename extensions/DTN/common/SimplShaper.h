//
// Created by lahmer on 29/09/17.
//

#ifndef SCENARIO_SIMPLSHAPER_H
#define SCENARIO_SIMPLSHAPER_H



#include "ns3/ndnSIM/model/ndn-common.hpp"

#include <ns3/queue.h>
#include "AbstractShaper.h"
#include <queue>
/**
 * This is a simple Shaper that sends the current packet and delays sending the next packet
 *  for a duration of:
 *      delay to send next packet = current packet size / available wifi phy rate
 */
namespace SimpleShaper {
    typedef std::pair<int, std::shared_ptr<ndn::Interest>>   InterestQueueEntry;
    typedef std::pair<int, std::shared_ptr<ndn::Data>>       DataQueueEntry;

    class SimpleShaper : public AbstractShaper {
    public:
        SimpleShaper(ns3::Ptr<ns3::WifiNetDevice> device, nfd::Forwarder& f);

        virtual void Enqueue(std::shared_ptr<ndn::Interest> interest, const nfd::Face &ouFace);

        virtual void Enqueue(std::shared_ptr<ndn::Data> data, const nfd::Face &outFace);

        virtual void onContact(const nfd::Face &face);

        virtual void onContactLost(const nfd::Face &face);

        virtual void startInterestSendingLoop();

        virtual void startDataSendingLoop();

    private:
        std::vector<InterestQueueEntry> interestQueue;

        std::vector<DataQueueEntry> dataQueue;

        double rate = 6750000;

        double interestRateFraction = 0.1;

        double dataRateFraction = 0.3;


    };

}
#endif //SCENARIO_SIMPLSHAPER_H
