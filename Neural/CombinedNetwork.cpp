//
//  CombinedNetwork.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 04/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "CombinedNetwork.hpp"
#include "Trainer.hpp"
#include "Network.hpp"
#include "DataIterator.hpp"
#include <iostream>
#include <iomanip>

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
    std::string Estimate(const std::string& data_file_path, bool log) const;
    
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
    
    Data ConformData(const neural::Data &data) const;
    
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

std::string CombinedNetwork::Impl::Serialize() const {
    return m_network->Serialize();
}

std::string CombinedNetwork::Impl::Estimate(const std::string &data_file_path, bool log) const {
    
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
        
        std::vector<double> results = m_network->Feed(ConformData(test_iterator.Value()));
        
        size_t max_pos = 0;
        double max = 0.0;
        for (size_t results_index = 0 ; results_index < results.size() ; results_index++)
            if (results.at(results_index) > max) {
                max_pos = results_index;
                max = results.at(results_index);
            }
        
        output += std::to_string(max_pos) + '\n';
        
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

void CombinedNetwork::Impl::Train(const std::string &data_file_path, const std::string &key_file_path, bool log) {
    
    Trainer trainer;
    
    //Train for all given values
    trainer.Train(100,
                  data_file_path,
                  key_file_path,
                  [&](const Data& data, size_t key) {
                      
                      Data modified_result;
                      modified_result.content = std::vector<double>(10, 0.0);
                      modified_result.content[key] = 1.0;
                      
                      m_network->Train(ConformData(data), modified_result);
                      
                  }, [&](const Data& data) -> double {
                      
                      std::vector<double> results = m_network->Feed(ConformData(data));
                      
                      size_t max_pos = 0;
                      double max = 0.0;
                      for (size_t results_index = 0 ; results_index < results.size() ; results_index++)
                          if (results.at(results_index) > max) {
                              max_pos = results_index;
                              max = results.at(results_index);
                          }
                      
                      return max_pos;
                      
                  }, log);
}

Data CombinedNetwork::Impl::ConformData(const neural::Data &data) const {
    
    Data modified_data;
    modified_data.content.reserve(784);
    for (size_t i = 0 ; i < 784 ; i++)
        modified_data.content.push_back(data.content.at(i) > 50 ? 1.0 : 0.0);
    
    return modified_data;
}

#pragma mark - Combined Network functions

CombinedNetwork::CombinedNetwork() :
m_pimpl(new Impl())
{ }

CombinedNetwork::CombinedNetwork(const std::string& serialized) :
m_pimpl(new Impl(serialized))
{ }

CombinedNetwork::~CombinedNetwork() = default;

std::string CombinedNetwork::Serialize() const {
    return OperationalNetwork::Serialize() + m_pimpl->Serialize();
}

std::string CombinedNetwork::Estimate(const std::string &data_file_path, bool log) const {
    return m_pimpl->Estimate(data_file_path, log);
}

void CombinedNetwork::Train(const std::string &data_file_path, const std::string &key_file_path, bool log) {
    m_pimpl->Train(data_file_path, key_file_path, log);
}

enum OperationalNetwork::Type CombinedNetwork::Type() const {
    return OperationalNetwork::Type::kCombined;
}
