//
//  CombinedNetwork.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 04/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "CombinedNetwork.hpp"
#include "Network.hpp"

using namespace neural;

/**
 * Implementation.
 */
class CombinedNetwork::Impl {
public:
    
    /**
     * Constructor.
     * This will create a new default combined network.
     */
    Impl();
    
    /**
     * Constructor.
     * This will recreate the network given in the input.
     *
     * @param serialized The serialized form of the combined network.
     */
    Impl(const std::string& serialized);
    
    /**
     * This will serialize the network into a form that can be saved and
     * later construct an identical network to the current one.
     *
     * @return A string that contains the serialized form of the network.
     */
    std::string Serialize() const;
    
    /**
     * Runs the network against the input data and outputs the
     * results as a string with each line containing the estimated
     * result of the corresponding data in the file.
     *
     * @param data_file_path    The path to the data file.
     * @return A string that contains the estimated results.
     */
    std::string Estimate(const std::string& data_file_path) const;
    
    /**
     * Trains the network against known data.
     *
     * @param data_file_path    The path to the file containing the pixel data.
     * @param key_file_path     The path to the file containing the results of the data file.
     * @param log               Flag if to output progress to the consule.
     */
    void Train(const std::string& data_file_path,
               const std::string& key_file_path,
               bool log);
    
private:
    
    std::unique_ptr<Network> m_network;
    
};

#pragma mark - Implementation

CombinedNetwork::Impl::Impl() :
m_network(new Network(301)){
    
    //Add the rest of the layers
    m_network->AddNetwork(200);
    m_network->AddNetwork(200);
    m_network->AddNetwork(180);
    m_network->AddNetwork(80);
    m_network->AddNetwork(10);
}

CombinedNetwork::Impl::Impl(const std::string& serialized) :
m_network(new Network(serialized))
{ }

#pragma mark - Combined Network functions

CombinedNetwork::CombinedNetwork() :
m_pimpl(new Impl())
{ }

CombinedNetwork::CombinedNetwork(const std::string& serialized) :
m_pimpl(new Impl(serialized))
{ }

std::string CombinedNetwork::Serialize() const {
    return m_pimpl->Serialize();
}

std::string CombinedNetwork::Estimate(const std::string &data_file_path) const {
    return m_pimpl->Estimate(data_file_path);
}

void CombinedNetwork::Train(const std::string &data_file_path, const std::string &key_file_path, bool log) {
    m_pimpl->Train(data_file_path, key_file_path, log);
}

enum OperationalNetwork::Type CombinedNetwork::Type() const {
    return OperationalNetwork::Type::kCombined;
}
