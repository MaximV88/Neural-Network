//
//  main.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 03/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "OperationalNetwork.hpp"

using namespace neural;

char* GetOption(char ** begin, char ** end, const std::string& option) {
    
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
        return *itr;
    
    return 0;
}

int main(int argc, char * argv[]) {

    //Show instructions
    if (argc == 1) {
        
        std::cerr << "Welcome to the NeuralNetworker(TM), probably the only C++ implementation around.\n\n"
        << "Usage:\n"
        << "-i\tSpecifies the input data file that has the raw data as 784 pixels per each read\n"
        << "-k\tSpecifies the key file that holds the answers for the given data file\n"
        << "-o\tSpecifies the name of the output file\n"
        << "-n\tSpecifies the type of network to use: 1 stands for 10 different networks, 2 will run with a single network\n"
        << "-t\tActivates the test mode. The flag specifies name of the output file from a previous run. The -i file will be used as the test file and will output the results to the -o file\n\n\n";
    }
    else {
        
        //Handle program lifecycle with C
        char* data_file         = GetOption(argv, argv + argc, "-i");
        char* key_file          = GetOption(argv, argv + argc, "-k");
        char* output_file       = GetOption(argv, argv + argc, "-o");
        char* serialized_file   = GetOption(argv, argv + argc, "-t");
        char* type              = GetOption(argv, argv + argc, "-u");
        
        //Check that the data is valid
        if (!type && !serialized_file) {
            std::cerr << "Network type must be specified via -u";
            return 0;
        }
        
        if (serialized_file) {
            
            if (!data_file || !output_file) {
                std::cerr << "In order to create an output file that contains the results, -i and -o must be specified.";
                return 0;
            }
        }
        else {
            
            if (!data_file || !output_file || !key_file) {
                std::cerr << "In order to create a network file that contains the trained network, -i, -o and -k must be specified.";
                return 0;
            }
            
        }
    
        if (!serialized_file) {
            
            //Output a serialized network into a file according to type
            std::ofstream output(output_file);
            OperationalNetwork::Type network_type;
            
            if (*type == '1')       network_type = OperationalNetwork::Type::kSeperated;
            else if (*type == '2')  network_type = OperationalNetwork::Type::kCombined;
            
            OperationalNetwork network(network_type);
            network.Train(data_file, key_file);
            output << network.Serialize();
            
            output.close();
            std::cout << "The network was successfully serialized and saved to " << output_file << '\n';
            return 0;
        }
        else {
            
            //Convert the serialized file by type, and run the test file
            std::ofstream output(output_file);
            OperationalNetwork network(serialized_file);
            output << network.Estimate(data_file);
            
            output.close();
            std::cout << "The results have been saved to " << output_file << '\n';
            return 0;
        }
    }
    
    return 0;
}
