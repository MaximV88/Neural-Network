//
//  OperationalNetwork.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 04/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "OperationalNetwork.hpp"
#include "CombinedNetworkImplementation.hpp"
#include "SeperatedNetworkImplementation.hpp"
#include "DataIterator.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <math.h>

#define SHOW_PROGRESS 1

using namespace neural;

OperationalNetwork::OperationalNetwork(enum OperationalNetwork::Type type) {
    
    switch (type) {
        case Type::kCombined:   m_pimpl.reset(new CombinedNetworkImplementation());
        case Type::kSeperated:  m_pimpl.reset(new SeperatedNetworkImplementation());
    }
}

OperationalNetwork::OperationalNetwork(const std::string &serialized_file_path) {
    
    std::fstream file_stream(serialized_file_path);
    std::string type;
    std::getline(file_stream, type);
    
    std::string contents((std::istreambuf_iterator<char>(file_stream)),
                         (std::istreambuf_iterator<char>()));
    
    if (type == "Combined")         m_pimpl.reset(new CombinedNetworkImplementation());
    else if (type == "Seperated")   m_pimpl.reset(new SeperatedNetworkImplementation());
}

OperationalNetwork::~OperationalNetwork() { };

std::string OperationalNetwork::Serialize() const {
    
    std::string serialized;
    
    //Add the prefix of the network by type
    switch (Type()) {
        case Type::kCombined: serialized = "Combined\n";    break;
        case Type::kSeperated: serialized = "Seperated\n";  break;
    }

    return serialized;
}

std::string OperationalNetwork::Estimate(const std::string &data_file_path, bool log) const {
    
    size_t index = 0;
    size_t all_values = RecordsInFile(data_file_path);
    
    std::string output;
    
    //Reserve the values themselfs and another '\n' character
    output.reserve(all_values * 2);
    
    //Set attribute for logging
    if (log) { std::cout << std::fixed; }
    
    for (DataIterator test_iterator(data_file_path) ;
         test_iterator.Valid() ;
         test_iterator.Next(), ++index) {
        
        
        output += std::to_string(m_pimpl->Estimate(test_iterator.Value())) + '\n';
        
        //Logging
        if (log && index % (all_values / 100) == 0)
            std::cout
            << std::setprecision(3)
            << "overall progress: "
            << index / static_cast<double>(all_values) * 100
            << "%\n";
    }
    
    return output;
}

void OperationalNetwork::Train(const std::string &data_file_path, const std::string &key_file_path, bool log) {
        
    //Train for all given values
    //Train all networks
    size_t index = 0;
    
    size_t all_records = RecordsInFile(key_file_path);
    size_t train_limit = all_records;
    
#ifdef SHOW_ACCURACY
    
    size_t correct = 0;
    size_t validate_limit = all_records - train_limit;
    
#endif
    
    //Set attribute for logging
    if (log) { std::cout << std::fixed; }
    
    for (DataIterator data(data_file_path), results(key_file_path) ;
         data.Valid() && results.Valid() ;
         data.Next(), results.Next(), index++) {
        
        size_t real_value = static_cast<size_t>(lround(results.Value().content.front()));
        
        if (index < train_limit) {
            
            //Training session
            m_pimpl->Train(data.Value(), real_value);
            
            if (log && index % (train_limit / 100) == 0)
                std::cout
                << std::setprecision(3)
                << "overall progress: "
                << index / static_cast<double>(train_limit) * 100
                << "%\n";
            
        }
        
#ifdef SHOW_ACCURACY
        
            //Validation session
            if (real_value == static_cast<size_t>(lround(Estimate(data.Value()))))
                ++correct;
            
            if (log && (index - train_limit) % (validate_limit / 100) == 0)
                std::cout
                << std::setprecision(3)
                << "correct: "
                << correct / static_cast<double>(index - train_limit) * 100
                << "%\n";
        
#endif
        
    }
}
