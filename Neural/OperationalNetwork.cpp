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
#include <fstream>

using namespace neural;

OperationalNetwork* OperationalNetwork::CreateNetwork(enum OperationalNetwork::Type type) {
    
    switch (type) {
        case Type::kCombined:   return new CombinedNetwork();
        case Type::kSeperated:  return new SeperatedNetwork();
    }
}

OperationalNetwork* OperationalNetwork::Deserialize(const std::string &serialized_file_path) {
    
    std::fstream file_stream(serialized_file_path);
    std::string type;
    std::getline(file_stream, type);
    
    std::string contents((std::istreambuf_iterator<char>(file_stream)),
                         (std::istreambuf_iterator<char>()));
    
    if (type == "Combined")         return new CombinedNetwork(contents);
    else if (type == "Seperated")   return new SeperatedNetwork(contents);
    
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

    return serialized;
}
