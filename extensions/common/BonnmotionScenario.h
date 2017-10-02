//
// Created by lahmer on 9/27/17.
//

#ifndef SCENARIO_BONNMOTIONSCENARIO_H
#define SCENARIO_BONNMOTIONSCENARIO_H
#include "GenericScenario.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"



class BonnmotionScenario : public GenericScenario{
public:

    BonnmotionScenario(int argc, char* argv[]);

    void setup();

private:

    void parseFile();

private:



    std::string                 m_model;
    double                      m_width;
    double                      m_height;
    double                      m_minSpeed;
    double                      m_maxSpeed;
    double                      m_maxPause;
    std::string                 m_ns2MobilityTraceFilePath ="/home/lahmer/tutorial/ndnSIM/scenario/temp/aascbkqtzqqnjurnmhnt.ns_movements";
    uint32_t                    m_numberOfNodes;


    uint32_t                    m_numberOfClients = 5;
    uint32_t                    m_numberOfProducers = 1;
    double                      m_bufferSize = 100;
    std::string                 m_contenCatalogPath = "/home/lahmer/tutorial/ndnSIM/scenario/temp/xrltpxoclisdrpplsnom";
    int                         m_numberOfRequestedFiles = 100;
    double                      m_QParameter = 0.7;
    double                      m_SParameter = 0.7;
    std::string                 m_dataBaseLocation = "/home/lahmer/ndnSIM/scenario/results/datasize";



    double                      m_dataSize;


};


#endif //SCENARIO_BONNMOTIONSCENARIO_H
