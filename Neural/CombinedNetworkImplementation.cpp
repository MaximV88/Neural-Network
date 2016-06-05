//
//  CombinedNetworkImplementation.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 05/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "CombinedNetworkImplementation.hpp"
#include "Data.hpp"
#include <string>

using namespace neural;

CombinedNetworkImplementation::CombinedNetworkImplementation() :
m_network(new Network(301)){
    
    //Add the rest of the layers
    m_network->AddNetwork(200);
    m_network->AddNetwork(200);
    m_network->AddNetwork(180);
    m_network->AddNetwork(80);
    m_network->AddNetwork(10);
}

CombinedNetworkImplementation::CombinedNetworkImplementation(const std::string& serialized) :
m_network(new Network(serialized))
{ }

CombinedNetworkImplementation::~CombinedNetworkImplementation() { };

OperationalNetwork::Type CombinedNetworkImplementation::Type() const {
    return OperationalNetwork::Type::kCombined;
}

std::string CombinedNetworkImplementation::Serialize() const {
    return m_network->Serialize();
}

double CombinedNetworkImplementation::Estimate(const Data& input) const {
    
    std::vector<double> results = m_network->Feed(ConformData(input));
    
    size_t max_pos = 0;
    double max = 0.0;
    for (size_t results_index = 0 ; results_index < results.size() ; results_index++)
        if (results.at(results_index) > max) {
            max_pos = results_index;
            max = results.at(results_index);
        }
    
    return max_pos;
}

void CombinedNetworkImplementation::Train(const neural::Data &data, size_t key) {
    
    Data modified_result;
    modified_result.content = std::vector<double>(10, 0.0);
    modified_result.content[key] = 1.0;
    
    m_network->Train(ConformData(data), modified_result);
}

Data CombinedNetworkImplementation::ConformData(const neural::Data &data) const {
    
    Data modified_data;
    modified_data.content.reserve(784);
    for (size_t i = 0 ; i < 784 ; i++)
        modified_data.content.push_back(data.content.at(i) > 50 ? 1.0 : 0.0);
    
    return modified_data;
}