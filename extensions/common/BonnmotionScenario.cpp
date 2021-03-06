//
// Created by lahmer on 9/27/17.
//

#include "BonnmotionScenario.h"
#include "DataBaseUtils.h"
#include"../DTN/apps/ConsumerDtn.h"
#include "../DTN/common/tables/NodeContactTable.h"

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

BonnmotionScenario::BonnmotionScenario(int argc, char* argv[]) {
    ns3::CommandLine cmd;
    cmd.AddValue("traceFile","NS2 movement trace file",m_ns2MobilityTraceFilePath);
    cmd.AddValue("contentCatalogPath","The path to the content catalog path",m_contenCatalogPath);
    cmd.AddValue("Qparamater","The Q parameter of the zipf distribution",m_QParameter);
    cmd.AddValue("Sparamater","The S parameter of the zipf distribution",m_SParameter);
    cmd.AddValue("database","The Sqlite database location",m_dataBaseLocation);

    cmd.AddValue("numberOfClients","The number of clients ",m_numberOfClients);
    cmd.AddValue("numberOfProducers","The number of producers",m_numberOfProducers);
    cmd.AddValue("numberOfRequestedFiles","The number of requested files by each client",m_numberOfRequestedFiles);
    cmd.AddValue("strategy"," The used trategy", m_strategy);
    cmd.AddValue("rate"," data rate",m_wifiMode);
    cmd.AddValue("bufferSize"," The size of the cdt table",m_bufferSize);
    cmd.AddValue("standard"," The wifi standard to use",m_standard);



    cmd.Parse(argc, argv);



    std::cout<<" traceFile "<<m_ns2MobilityTraceFilePath<<std::endl;
    std::cout<<" content Catalog path       "<<m_contenCatalogPath<<std::endl;
    std::cout<<" Qparameter "<<m_QParameter<<std::endl;
    std::cout<<" Sparamter  "<<m_SParameter<<std::endl;
    std::cout<<" database   "<<m_dataBaseLocation<<std::endl;
    std::cout<<" number of client "<<m_numberOfClients<<std::endl;
    std::cout<<" producers "<<m_numberOfProducers<<std::endl;
    std::cout<<" requested files "<<m_numberOfRequestedFiles<<std::endl;
    std::cout<<" strategy "<<m_strategy<<std::endl;
    std::cout<<" rate"<<m_wifiMode<<std::endl;
    std::cout<<" buffer size"<<m_bufferSize<<std::endl;
    std::cout<<" the wifi standard is "<<m_standard<<std::endl;
    ContentCatalog catalog(m_contenCatalogPath);
    m_dataSize = catalog.entries.at(0).sizeOfChunk;


    parseFile();
}

void BonnmotionScenario::parseFile() {
    //looking for the parameter file
    std::vector<std::string> componenets;
    boost::split(componenets, m_ns2MobilityTraceFilePath, boost::is_any_of("/"));

    std::vector<std::string> pair;
    if (componenets.size() == 0) {
        boost::split(pair, m_ns2MobilityTraceFilePath, boost::is_any_of("."));
    } else {
        boost::split(pair, *componenets.rbegin(), boost::is_any_of("."));
    }
    std::string mobilityParametersFilePath;
    for (auto it = componenets.begin(); it != componenets.end() && it + 1 != componenets.end(); it++) {
        mobilityParametersFilePath += *it +"/";
    }
    mobilityParametersFilePath += "/" + pair[0] + ".params";
    std::ifstream file(mobilityParametersFilePath.c_str());
    if (!file.good()) {
        std::cerr << " Error, can't find the mobility parameter file " << mobilityParametersFilePath;
        std::terminate();
    }
    std::map<std::string, std::string> parameters;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> p;
        boost::split(p, line, boost::is_any_of("="));
        if (p.size() != 2)
            continue;
        parameters[p[0]] = p[1];
        //std::cout << "     " << p[0] << "    " << p[1] << std::endl;
    }

    m_model                 = parameters["model"];
    m_initialSeed           = (uint32_t) (std::stoul(parameters["randomSeed"])%999999);
    m_width                 = std::stod(parameters["x"]);
    m_height                = std::stod(parameters["y"]);
    m_simulationDuration    = std::stod(parameters["duration"]);
    m_numberOfNodes         = (uint32_t) std::stol(parameters["nn"]);
    m_minSpeed              = std::stod(parameters["minspeed"]);
    m_maxSpeed              = std::stod(parameters["maxspeed"]);
    m_maxPause              = std::stod(parameters["maxpause"]);

}

void BonnmotionScenario::setup() {
    ns3::SeedManager::SetSeed(m_initialSeed);

    m_nodes.Create(m_numberOfNodes);
    for(uint32_t i = 0 ; i < m_nodes.GetN(); i++){
        ns3::Names::Add(std::string("Node_") + std::to_string(i),m_nodes.Get(i));
    }
    if(m_standard.compare("802.11a") == 0){
        setup802_11_a();
    }else if(m_standard.compare("802.11g") == 0){
        setup802_11_g();
    }else{
        std::cout<<" The standard you are trying to installl is not aviable "<<std::endl;
        std::terminate();
    }
    //setupWifiParameters();
    installEnergySources();

    setupTracers();

    ns3::Ns2MobilityHelper ns2mobility = ns3::Ns2MobilityHelper(m_ns2MobilityTraceFilePath);
    ns2mobility.Install();

    installNDNStack();

    //installing apps
    if(m_numberOfNodes < m_numberOfClients + m_numberOfProducers)
        throw "Number of producers and clients bigger then number of nodes";


    ns3::ndn::AppHelper consumerHelper("ns3::ndn::ConsumerDtn");
    consumerHelper.SetAttribute("ContentCatalogPath",ns3::StringValue(m_contenCatalogPath));
    consumerHelper.SetAttribute("Qparameter",ns3::DoubleValue(m_QParameter));
    consumerHelper.SetAttribute("Sparameter",ns3::DoubleValue(m_SParameter));
    consumerHelper.SetAttribute("NumberOfRequestedFile",ns3::IntegerValue(m_numberOfRequestedFiles));

    for(unsigned int i = 0 ;i < m_numberOfClients; i++){
        m_clients.Add(consumerHelper.Install(m_nodes.Get(i)));
    }

    ns3::ndn::AppHelper producerHelper("ns3::ndn::ProducerDtn");
    producerHelper.SetAttribute("ContentCatalogPath",ns3::StringValue(m_contenCatalogPath));

    for(unsigned int i = m_numberOfClients; i < m_numberOfClients + m_numberOfProducers; i++){
        m_producers.Add(producerHelper.Install(m_nodes.Get(i)));
    }





    ns3::Simulator::Stop(ns3::Seconds(m_simulationDuration));
    ns3::Simulator::Run();

    ns3::Simulator::Destroy();
    printTracersParameters();
    //now we log the results
    DataBaseUtils db(m_dataBaseLocation);
    db.addCouple("seed",m_initialSeed);
    db.addCouple("Qparamater",m_QParameter);
    db.addCouple("Sparamater",m_SParameter);
    db.addCouple("numberofClients",m_numberOfClients);
    db.addCouple("numberofProducers",m_numberOfProducers);
    db.addCouple("numberofNodes",m_numberOfNodes);
    db.addCouple("NumberOfRequestedFile",m_numberOfRequestedFiles);
    db.addCouple("strategy",m_strategy);
    db.addCouple("wifiStandard",m_standard);
    db.addCouple("rate",m_wifiMode);
    db.addCouple("bufferSize",m_bufferSize);
    db.addCouple("dataSize", m_dataSize);       //TODO::check this value
    db.addCouple("simulationDuration",m_simulationDuration);
    db.addCouple("macTxRtsFailed", m_macTxRtsFailed);
    db.addCouple("macTxPacketFailed", m_macTxPacketFailed);

    db.addCouple("rtsExceededLimits", m_rtsExceededLimits);
    db.addCouple("packetRetransmissioExceededLimits", m_packetRetransmissioExceededLimits);
    db.addCouple("phyTxDrop", m_phyTxDrop);
    db.addCouple("phyRxDrop", m_phyRxDrop);
    db.addCouple("macTxDrop", m_macTxDrop);
    db.addCouple("macRxDrop", m_macRxDrop);

//compute the average delay and the delivery ratio
    double accumulatedAverageDelay = 0.0;
    double accumulatedHopCount = 0.0;
    uint64_t totalNumberofSentInterests = 0;
    uint64_t totalNumberOfReceivedData = 0;

    for(int i = 0; i < m_numberOfClients; i++){
        auto client = ns3::DynamicCast<ns3::ndn::ConsumerDtn>(m_clients.Get(i));
        //for each file requested by the client
        for(auto& it : client->m_metrics.entries){
            for(auto& chunk : it.second.entries){
                totalNumberofSentInterests++;
                if(std::get<1>(chunk.second) != -1.0){
                    totalNumberOfReceivedData++;
                    accumulatedAverageDelay += std::get<1>(chunk.second) - std::get<0>(chunk.second);
                    accumulatedHopCount += std::get<2>(chunk.second);
                }
            }
        }
    }

    double averageDelay = accumulatedAverageDelay /(double) totalNumberOfReceivedData;
    double averageHopCount = accumulatedHopCount /(double) totalNumberOfReceivedData;
    double averageDeliveryRatio = totalNumberOfReceivedData /(double) totalNumberofSentInterests;
    db.addCouple("averageDelay", averageDelay);
    db.addCouple("averageHopCount", averageHopCount);
    db.addCouple("averageDeliveryRatio", averageDeliveryRatio);

    //concerns the contact informations
    uint64_t totalTranssferredBytes = NodeContactTable::getFullTransferedBytes();
    double averageTransfferedBytesPerContact = NodeContactTable::getAverageTransferredBytesPerContact();
    double totalContactDuration = NodeContactTable::getFullContactDuration();
    double averageContactDuration = NodeContactTable::getAverageContactDuration();
    double fractionOfSuccessfullTransmissionOfInterests = (double) NodeContactTable::getNumberOfSuccessfullyTransferedInterests() / (double)NodeContactTable::getNumberOfAllSentInterests();
    double fractionOfSuccessfullyTransmissionOfData = (double) NodeContactTable::getNumberOfSuccessfullyTranssferredData() / (double)NodeContactTable::getNumberOfAllSentData();

    db.addCouple("totalTransferredBytes",totalTranssferredBytes);
    db.addCouple("averageTranssferedBytesPerContact", averageTransfferedBytesPerContact);
    db.addCouple("totalContactDuration", totalContactDuration);
    db.addCouple("averageContactDuration", averageContactDuration);



    std::cout<<fractionOfSuccessfullTransmissionOfInterests<<std::endl;
    std::cout<<fractionOfSuccessfullyTransmissionOfData<<std::endl;
    db.addCouple("fractionOfSuccessfullyTransmissionOfData", fractionOfSuccessfullyTransmissionOfData);

    db.addCouple("fractionOfSuccessfullTransmissionOfInterests", fractionOfSuccessfullTransmissionOfInterests);


    std::cout<<" The query : "<<db.constructInsertQuery()<<std::endl;
    db.executeQuery(db.constructInsertQuery().c_str());


}