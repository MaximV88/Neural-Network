//
//  SeperatedNetworkImplementation.hpp
//  Neural
//
//  Created by Maxim Vainshtein on 05/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef SeperatedNetworkImplementation_hpp
#define SeperatedNetworkImplementation_hpp
#include "OperationalNetworkImplementation.h"
#include "Network.hpp"
#include <memory>
NAMESPACE_NEURAL_BEGIN
class Data;

class SeperatedNetworkImplementation : public OperationalNetwork::Impl {
public:
    
    /**
     * Constructor.
     * This will create a new default seperated network.
     */
    SeperatedNetworkImplementation();
    
    /**
     * Constructor.
     * This will recreate the network given in the input.
     *
     * @param serialized The serialized form of the seperated network.
     */
    SeperatedNetworkImplementation(const std::string& serialized);
    
    /**
     * Destructor.
     */
    ~SeperatedNetworkImplementation();
    
    /**
     * Returns the type of the network as an enum.
     *
     * @return An enum that represents the type of the network.
     */
    OperationalNetwork::Type Type() const;
    
    /**
     * This will serialize the network into a form that can be saved and
     * later construct an identical network to the current one.
     *
     * @return A string that contains the serialized form of the network.
     */
    std::string Serialize() const;
    
protected:
    
    /**
     * Trains the network with given input and it's answer.
     *
     * @param data  The data to train on.
     * @param key   The answer to the data.
     */
    virtual void Train(const Data& data, size_t key);
    
    /**
     * Estimates the result to the given input.
     *
     * @return The estimation about the answer.
     */
    virtual double Estimate(const Data& input) const;
    
private:
    
    /**
     * Conforms the data to a form that can be understood by the network.
     *
     * @param data  The data to conform
     * @return Data that contains the values conformed to the network.
     */
    Data ConformData(const Data &data) const;
    
    ///Stores the networks.
    std::vector<std::unique_ptr<Network> > m_networks;
};

NAMESPACE_NEURAL_END
#endif /* SeperatedNetworkImplementation_hpp */
