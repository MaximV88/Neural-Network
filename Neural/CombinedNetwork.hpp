//
//  CombinedNetwork.hpp
//  Neural
//
//  Created by Maxim Vainshtein on 04/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef CombinedNetwork_hpp
#define CombinedNetwork_hpp
#include "OperationalNetwork.hpp"
#include <memory>
NAMESPACE_NEURAL_BEGIN

class CombinedNetwork : public OperationalNetwork {
public:
    
    /**
     * Constructor.
     * This will create a new default combined network.
     */
    CombinedNetwork();
    
    /**
     * Constructor.
     * This will recreate the network given in the input.
     *
     * @param serialized The serialized form of the combined network.
     */
    CombinedNetwork(const std::string& serialized);
    
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
    std::string Estimate(const std::string& data_file_path) const;
    
    /**
     * Trains the network against known data.
     *
     * @param data_file_path    The path to the file containing the pixel data.
     * @param key_file_path     The path to the file containing the results of the data file.
     * @param log               Flag if to output progress to the consule.
     */
    void Train(const std::string& data_file_path,
               const std::string& key_file_path,
               bool log = true);
    
    /**
     * Returns the type of the network as an enum.
     *
     * @return An enum that represents the type of the network.
     */
    enum OperationalNetwork::Type Type() const;
    
    /**
     * Destructor.
     */
    ~CombinedNetwork();
    
private:
    
    class Impl;
    std::unique_ptr<Impl> m_pimpl;
    
};

NAMESPACE_NEURAL_END
#endif /* CombinedNetwork_hpp */
