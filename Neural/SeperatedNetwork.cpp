//
//  SeperatedNetwork.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 04/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "SeperatedNetwork.hpp"
#include "Network.hpp"
#include "Trainer.hpp"
#include "DataIterator.hpp"
#include <iomanip>
#include <iostream>

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
    
    //The networks are seperated by the delimiter '!' - at least one is at the start with '\n'
    size_t network_start = 2;
    
    for (size_t index = 0 ; index < 10 ; index++) {
        
        size_t network_end = serialized.find_first_of('!', network_start);
        
        m_networks.push_back(std::unique_ptr<Network>(new Network(serialized.substr(network_start, network_end - network_start))));
        network_start = network_end + 2;
    }
}

std::string SeperatedNetwork::Impl::Serialize() const {

    std::string serialized;
    
    //Seperate the networks by a delimiter '!'
    for (size_t index = 0 ; index < 10 ; index++)
        serialized += '!' + m_networks[index]->Serialize();
    
    return serialized;
}

std::string SeperatedNetwork::Impl::Estimate(const std::string &data_file_path, bool log) const {
    
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
        
        size_t max_pos = 0;
        double max_value = 0.0;
        
        //Find maximal value by network index
        for (size_t network_index = 0 ; network_index < 10 ; network_index++) {
            
            double result = m_networks[network_index]->Feed(ConformData(test_iterator.Value())).front();
            if (result > max_value) {
                max_value = result;
                max_pos = network_index;
            }
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

void SeperatedNetwork::Impl::Train(const std::string &data_file_path, const std::string &key_file_path, bool log) {
    
    Trainer trainer;
    
    trainer.Train(100, data_file_path, key_file_path, [&](const Data& data, size_t key) {
        
        for (size_t network_index = 0 ; network_index < 10 ; network_index++) {
            
            //Get the network to identify the number based on it's index
            Data modified_result;
            modified_result.content = std::vector<double>(1, (key == network_index) ? 1.0 : 0.0);
            
            m_networks[network_index]->Train(data, modified_result);
        }
        
    }, [&](const Data& data) -> double {
        
        size_t max_pos = 0;
        double max_value = 0.0;
        
        //Find maximal value by network index
        for (size_t network_index = 0 ; network_index < 10 ; network_index++) {
            
            double result = m_networks[network_index]->Feed(data).front();
            if (result > max_value) {
                max_value = result;
                max_pos = network_index;
            }
        }
        
        //The winning network index is the number
        return max_pos;
        
    }, log);
}

Data SeperatedNetwork::Impl::ConformData(const neural::Data &data) const {
    
    Data modified_data;
    modified_data.content.reserve(784);
    for (size_t i = 0 ; i < 784 ; i++)
        modified_data.content.push_back(data.content.at(i) > 50 ? 1.0 : 0.0);
    
    return modified_data;
}

#pragma mark - Combined Network functions

SeperatedNetwork::SeperatedNetwork() :
m_pimpl(new Impl())
{ }

SeperatedNetwork::SeperatedNetwork(const std::string& serialized) :
m_pimpl(new Impl(serialized))
{ }

SeperatedNetwork::~SeperatedNetwork() = default;

std::string SeperatedNetwork::Serialize() const {
    return m_pimpl->Serialize();
}

std::string SeperatedNetwork::Estimate(const std::string &data_file_path, bool log) const {
    return m_pimpl->Estimate(data_file_path, log);
}

void SeperatedNetwork::Train(const std::string &data_file_path, const std::string &key_file_path, bool log) {
    m_pimpl->Train(data_file_path, key_file_path, log);
}

enum OperationalNetwork::Type SeperatedNetwork::Type() const {
    return OperationalNetwork::Type::kSeperated;
}
