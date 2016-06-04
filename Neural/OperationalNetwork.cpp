//
//  OperationalNetwork.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 04/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "OperationalNetwork.hpp"
#include "CombinedNetwork.hpp"
#include "SeperatedNetwork.hpp"
#include <sstream>

using namespace neural;

OperationalNetwork* OperationalNetwork::CreateNetwork(enum OperationalNetwork::Type type) {
    
    switch (type) {
        case Type::kCombined:   return new CombinedNetwork();
        case Type::kSeperated:  return new SeperatedNetwork();
    }
}

OperationalNetwork* OperationalNetwork::Deserialize(const std::string &serialized) {
    
    std::stringstream string_stream(serialized);
    std::string type;
    std::getline(string_stream, type);
    
    if (type == "Combined")         return new CombinedNetwork(serialized.substr(string_stream.tellg()));
    else if (type == "Seperated")   return new SeperatedNetwork(serialized.substr(string_stream.tellg()));
    
    return nullptr;
}

OperationalNetwork::~OperationalNetwork() = default;

std::string OperationalNetwork::Serialize() const {
    
    std::string serialized;
    
    //Add the prefix of the network by type
    switch (Type()) {
        case Type::kCombined: serialized = "Combined\n";    break;
        case Type::kSeperated: serialized = "Seperated\n";  break;
    }
    
    //Add content of network
    serialized += Serialize();
    
    return serialized;
}
