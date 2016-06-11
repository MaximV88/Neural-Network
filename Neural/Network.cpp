//
//  Network.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 03/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "Network.hpp"
#include "Perceptron.hpp"
#include "Data.hpp"
#include <vector>
#include <string>
#include <sstream>

using namespace neural;

/**
 * Implementation.
 */
class Network::Impl {
public:
    
    
    /**
     * Constructor.
     *
     * @param perceptrons   Number of perceptrons in the starting layer.
     * @param previous      The previous network that the new one is connected to.
     */
    Impl(size_t perceptrons, Network::Impl* previous = NULL);
    
    /**
     * Constructor.
     *
     * @param The serialized string that was given from a network.
     */
    Impl(const std::string& serialized);
    
    /**
     * Adds a network to the last network in the chained networks.
     *
     * @param perceptrons   The number of perceptrons that the added network
     *                      will have.
     */
    void AddNetwork(size_t perceptrons);
    
    /**
     * Gets a data to process and returns the result.
     *
     * @param data  The data to process.
     * @return A vector containing all of the last layer's results
     */
    std::vector<double> Feed(const Data& data) const;
    
    /**
     * Gets results from a previous run to continue calculating
     *
     * @param data  The data to process.
     * @return A vector containing all of the layer's results
     */
    std::vector<double> Feed(const std::vector<double>& data) const;

    /**
     * Trains the neural network to comply to a given result.
     *
     * @param data      The data to practice on.
     * @param target    The values that the network should reach.
     */
    void Train(const Data& data, const Data& target);
    
    /**
     * Trains the neural network and returns the delta's that were created.
     */
    std::vector<double> Train(const std::vector<double>& data, const Data& target);
    
    /**
     * Recieves the delta results from the next layer and
     * is called only in the hidden layers. The call will
     * then update the weights according to the back propogation
     * algorithm
     *
     * @param deltas    The delta results per each perceptron from the next layer.
     */
    void BackPropogate(std::vector<double>& deltas);
    
    /**
     * Return the result for the given data.
     *
     * @param data  The data to work on.
     * @return The result from every perceptron.
     */
    std::vector<double> Sum(const std::vector<double>& data) const;
    
    /**
     * Outputs the network into a format that can later
     * be loaded to recreate the setup and weights (in
     * case it was trained).
     *
     * @return The serialized version of the network.
     */
    std::string Serialize() const;
    
    /**
     * Destructor.
     */
    ~Impl();
    
private:
    
    ///Stores all the perceptrons in the network
    std::vector<std::unique_ptr<Perceptron>> m_perceptrons;
    
    ///Stores the next network to propogate signals to
    Network::Impl* m_next;
    
    ///Stores the previous network to back propogate errors to
    Network::Impl* m_previous;
    
};

#pragma mark - Implementation

Network::Impl::Impl(size_t perceptrons, Network::Impl* previous) :
m_next(NULL),
m_previous(previous) {
    

    //Create the perceptrons that are going to handle the data
    for (size_t index = 0 ; index < perceptrons ; index++) {
        
        //Have weight for every 'pixel' in the data or be able to process all the output from previous layer
        size_t number_of_connections = (m_previous) ? m_previous->m_perceptrons.size() : 784;
        
        m_perceptrons.push_back(std::unique_ptr<Perceptron>(new Perceptron(number_of_connections, 0.25)));
    }
}

Network::Impl::Impl(const std::string& serialized) :
m_next(NULL),
m_previous(NULL) {
    
    //Deserialize the input manually for networks
    std::stringstream string_stream(serialized);
    
    std::string read_line;
    std::getline(string_stream, read_line);
    size_t network_size = std::stoul(read_line);
    
    for (size_t index = 0 ; index < network_size ; index++) {
        
        std::getline(string_stream, read_line);
        m_perceptrons.push_back(std::unique_ptr<Perceptron>(new Perceptron(read_line)));
        
    }
    
    /*
     * Cut the serialized string away and send the rest to the next network.
     * A good implementation should use string iterators to avoid copying
     * but not done since this is outside of the home work's scope.
     */
    std::string next_network = serialized.substr(string_stream.tellg());
    
    if (!next_network.empty()) {
        
        m_next = new Network::Impl(next_network);
        m_next->m_previous = this;
    }
}

Network::Impl::~Impl() {
    
    //Propogate deletion across the linked networks
    if (m_next)
        delete m_next;
}

void Network::Impl::AddNetwork(size_t perceptrons) {
    
    if (m_next)     m_next->AddNetwork(perceptrons);
    else            m_next = new Impl(perceptrons, this);
}


std::vector<double> Network::Impl::Feed(const Data& data) const {
    return Feed(data.content);
}

std::vector<double> Network::Impl::Feed(const std::vector<double> &data) const {
    
    /*
     * This function is invoked from an outside call only on the first
     * layer. Thus, the input mush be a read from file. The contents must
     * be processed and then sent to the next layer.
     */
    
    if (m_next)
        return m_next->Feed(Sum(data));
    
    //Otherwise return the original results
    return Sum(data);
}

std::vector<double> Network::Impl::Train(const std::vector<double>& data, const neural::Data &target) {
    
    if (m_next) {
        
        //Hidden layer
        std::vector<double> deltas = m_next->Train(Sum(data), target);
        std::vector<double> current_deltas;
        current_deltas.reserve(m_perceptrons.size());
        
        for (size_t index = 0, total = m_perceptrons.size() ; index < total ; index++) {
            
            double output = m_perceptrons[index]->Feed(data);
            
            //Find the sum of the deltas multiplied by their relative weights
            double delta_sum = 0.0;
            for (size_t delta_index = 0, delta_total = deltas.size(); delta_index < delta_total ; delta_index++)
                delta_sum += deltas[delta_index] * m_next->m_perceptrons[delta_index]->m_weights[index];
            
            double delta = output * (1.0 - output) * delta_sum;
            m_perceptrons[index]->Train(delta, data);
            current_deltas.push_back(delta);
        }
   
        return current_deltas;
        
    }
    else if (m_previous) {
        
        //Output layer
        std::vector<double> deltas;
        deltas.reserve(m_perceptrons.size());
        
        for (size_t index = 0, total = m_perceptrons.size(); index < total ; index++) {
            
            double output = m_perceptrons[index]->Feed(data);
            double delta = output * (1.0 - output) * (output - target.content[index]);
            m_perceptrons[index]->Train(delta, data);
            deltas.push_back(delta);
        }
        
        return deltas;
    }
    
    return { };
}

void Network::Impl::Train(const neural::Data &data, const neural::Data &target) {
    Train(data.content, target);
}

std::vector<double> Network::Impl::Sum(const std::vector<double>& data) const {
    
    std::vector<double> results;
    results.reserve(m_perceptrons.size());
    
    //Calculate results for the input data
    for (size_t index = 0, total = m_perceptrons.size() ; index < total ; index++)
        results.push_back(m_perceptrons[index]->Feed(data));
    
    return results;
}

std::string Network::Impl::Serialize() const {
    
    /*
     * The current network needs to be serialized before it's
     * next layer. In this way it is possible to deserialize 
     * according to construction order.
     */
    std::string serialized = std::to_string(static_cast<unsigned long long>(m_perceptrons.size())) + '\n';
    for (size_t index = 0, total = m_perceptrons.size() ; index < total ; index++) {
        serialized += m_perceptrons[index]->Serialize() + '\n';
    }
    
    //Add the next layer
    if (m_next)
        serialized += m_next->Serialize();
    
    return serialized;
}

#pragma mark - Network functions

Network::Network(size_t perceptrons) :
m_pimpl(new Impl(perceptrons))
{ }

Network::Network(const std::string& serialized) :
m_pimpl(new Impl(serialized))
{ }

Network::~Network() { };

void Network::AddNetwork(size_t perceptrons) {
    m_pimpl->AddNetwork(perceptrons);
}

std::vector<double> Network::Feed(const Data& data) const {
    return m_pimpl->Feed(data);
}

void Network::Train(const neural::Data &data, const neural::Data &target) {
    m_pimpl->Train(data, target);
}

std::string Network::Serialize() const {
    return m_pimpl->Serialize();
}
