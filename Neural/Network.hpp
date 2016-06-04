//
//  Network.hpp
//  Neural
//
//  Created by Maxim Vainshtein on 03/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef Network_hpp
#define Network_hpp
#include "Definitions.h"
#include <stdio.h>
#include <vector>
#include <memory>
NAMESPACE_NEURAL_BEGIN
class Data;

/**
 * The network class is essentialy a linked list
 * of networks that communicate and handle data
 * and training via percetrons that each network has.
 */
class Network {
public:
    
    /**
     * Constructor.
     *
     * @param perceptrons  Number of perceptrons in the starting layer.
     */
    Network(size_t perceptrons);
    
    /**
     * Constructor.
     *
     * @param The serialized string that was given from a network.
     */
    Network(const std::string& serialized);
    
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
     * Trains the neural network to comply to a given result.
     *
     * @param data      The data to practice on.
     * @param target    The values that the network should reach.
     */
    void Train(const Data& data, const Data& target);
    
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
    ~Network();
    
private:
    
    class Impl;
    std::unique_ptr<Impl> m_pimpl;
    
};


NAMESPACE_NEURAL_END
#endif /* Network_hpp */
