//
//  OperationalNetwork.hpp
//  Neural
//
//  Created by Maxim Vainshtein on 04/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef OperationalNetwork_hpp
#define OperationalNetwork_hpp
#include "Definitions.h"
#include <string>
#include <memory>
NAMESPACE_NEURAL_BEGIN
class Data;

class OperationalNetwork {
public:
    
    enum class Type {
        kCombined,
        kSeperated
    };
    
    /**
     * This will create the network by given type in the input.
     *
     * @param type The type of network to create.
     */
    OperationalNetwork(enum OperationalNetwork::Type type);
    
    /**
     * This will recreate the network given in the input.
     *
     * @param serialized_file_path The serialized form of the network.
     */
    OperationalNetwork(const std::string& serialized_file_path);

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
     * @param log               Flag that indicates to print progress to consule.
     * @return A string that contains the estimated results.
     */
    std::string Estimate(const std::string& data_file_path, bool log = true) const;
    
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
     * Destructor.
     */
    ~OperationalNetwork();

    //Represents the implementation
    class Impl;

private:
    
    ///Stores the implementation
    std::unique_ptr<Impl> m_pimpl;
    
};

NAMESPACE_NEURAL_END
#endif /* OperationalNetwork_hpp */
