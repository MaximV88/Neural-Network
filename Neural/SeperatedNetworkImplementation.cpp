//
//  SeperatedNetworkImplementation.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 05/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "SeperatedNetworkImplementation.hpp"
#include "Data.hpp"
#include <string>

using namespace neural;

SeperatedNetworkImplementation::SeperatedNetworkImplementation() {
    
    //Output layer will have only 1 neuron
    for (size_t index = 0 ; index < 10 ; index++) {
        
        Network* new_network = new Network(80);
        new_network->AddNetwork(19);
        new_network->AddNetwork(1);
        
        m_networks.push_back(std::unique_ptr<Network>(new_network));
    }
}

SeperatedNetworkImplementation::SeperatedNetworkImplementation(const std::string& serialized)  {

    //The networks are seperated by the delimiter '!'
    size_t network_start = 1;
    
    for (size_t index = 0 ; index < 10 ; index++) {
        
        size_t network_end = serialized.find_first_of('!', network_start);
        
        m_networks.push_back(std::unique_ptr<Network>(new Network(serialized.substr(network_start, network_end - network_start))));
        network_start = network_end + 1;
    }
}

OperationalNetwork::Type SeperatedNetworkImplementation::Type() const {
    return OperationalNetwork::Type::kSeperated;
}

SeperatedNetworkImplementation::~SeperatedNetworkImplementation() { };

std::string SeperatedNetworkImplementation::Serialize() const {

    std::string serialized;
    
    //Seperate the networks by a delimiter '!'
    for (size_t index = 0 ; index < 10 ; index++)
        serialized += '!' + m_networks[index]->Serialize();
    
    return serialized;
}

double SeperatedNetworkImplementation::Estimate(const Data& input) const {
    
    size_t max_pos = 0;
    double max_value = 0.0;
    
    //Find maximal value by network index
    for (size_t network_index = 0 ; network_index < 10 ; network_index++) {
        
        double result = m_networks[network_index]->Feed(ConformData(input)).front();
        if (result > max_value) {
            max_value = result;
            max_pos = network_index;
        }
    }
    
    return max_pos;
}

void SeperatedNetworkImplementation::Train(const neural::Data &data, size_t key) {
    
    for (size_t network_index = 0 ; network_index < 10 ; network_index++) {
        
        //Get the network to identify the number based on it's index
        Data modified_result;
        modified_result.content = std::vector<double>(1, (key == network_index) ? 1.0 : 0.0);
        
        m_networks[network_index]->Train(ConformData(data), modified_result);
    }
}

Data SeperatedNetworkImplementation::ConformData(const neural::Data &data) const {
    
    Data modified_data;
    modified_data.content.reserve(784);
    for (size_t i = 0 ; i < 784 ; i++)
        modified_data.content.push_back(data.content.at(i) > 50 ? 1.0 : 0.0);
    
    return modified_data;
}
