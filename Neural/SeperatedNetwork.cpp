//
//  SeperatedNetwork.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 04/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "SeperatedNetwork.hpp"
#include "Network.hpp"

using namespace neural;

/**
 * Implementation.
 */
class SeperatedNetwork::Impl {
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
    
    std::vector<std::unique_ptr<Network>> m_networks;
    
};

#pragma mark - Implementation

SeperatedNetwork::Impl::Impl() {
    
    //Output layer will have only 1 neuron
    for (size_t index = 0 ; index < 10 ; index++) {
        
        Network* new_network = new Network(80);
        new_network->AddNetwork(19);
        new_network->AddNetwork(1);
        
        m_networks.push_back(std::unique_ptr<Network>(new_network));
    }
}

SeperatedNetwork::Impl::Impl(const std::string& serialized) {
    
}

#pragma mark - Combined Network functions

SeperatedNetwork::SeperatedNetwork() :
m_pimpl(new Impl())
{ }

SeperatedNetwork::SeperatedNetwork(const std::string& serialized) :
m_pimpl(new Impl(serialized))
{ }

std::string SeperatedNetwork::Serialize() const {
    return m_pimpl->Serialize();
}

std::string SeperatedNetwork::Estimate(const std::string &data_file_path) const {
    return m_pimpl->Estimate(data_file_path);
}

void SeperatedNetwork::Train(const std::string &data_file_path, const std::string &key_file_path, bool log) {
    m_pimpl->Train(data_file_path, key_file_path, log);
}

enum OperationalNetwork::Type SeperatedNetwork::Type() const {
    return OperationalNetwork::Type::kSeperated;
}
