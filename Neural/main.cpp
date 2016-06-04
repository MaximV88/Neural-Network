//
//  main.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 03/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Network.hpp"
#include "Trainer.hpp"
#include "Data.hpp"

using namespace neural;

std::string SeperatedNetworks(const std::string& train_file_path,
                              const std::string& key_file_path) {
    
    std::vector<Network*> networks;
    
    //Output layer will have only 1 neuron
    for (size_t index = 0 ; index < 10 ; index++) {
        
        Network* new_network = new Network(80);
        new_network->AddNetwork(19);
        new_network->AddNetwork(1);
        
        networks.push_back(new_network);
    }

    Trainer trainer;
    
    trainer.Train(100, train_file_path, key_file_path, [&](const Data& data, size_t key) {
        
        for (size_t network_index = 0 ; network_index < 10 ; network_index++) {
            
            //Get the network to identify the number based on it's index
            Data modified_result;
            modified_result.content = std::vector<double>(1, (key == network_index) ? 1.0 : 0.0);
            
            networks[network_index]->Train(data, modified_result);
        }
        
    }, [&](const Data& data) -> double {
        
        size_t max_pos = 0;
        double max_value = 0.0;
        
        //Find maximal value by network index
        for (size_t network_index = 0 ; network_index < 10 ; network_index++) {
            
            double result = networks[network_index]->Feed(data).front();
            if (result > max_value) {
                max_value = result;
                max_pos = network_index;
            }
        }
        
        //The winning network index is the number
        return max_pos;
    });
    
    std::string serialized;
    for (size_t index = 0 ; index < 10 ; index++) {
        
        serialized += std::to_string(index) + '\n';
        serialized += networks[index]->Serialize();
    }
        
    return serialized;
}

std::string CombinedNetworks(const std::string& train_file_path,
                             const std::string& key_file_path) {
    
    Trainer trainer;
    
    trainer.Train(100,
                  train_file_path,
                  key_file_path,
                  [&](const Data& data, size_t key) {
        
        Data modified_result;
        modified_result.content = std::vector<double>(10, 0.0);
        modified_result.content[key] = 1.0;
        
        neural_network.Train(data, modified_result);
        
    }, [&](const Data& data) -> double {
        
        std::vector<double> results = neural_network.Feed(data);
        
        size_t max_pos = 0;
        double max = 0.0;
        for (size_t results_index = 0 ; results_index < results.size() ; results_index++)
            if (results.at(results_index) > max) {
                max_pos = results_index;
                max = results.at(results_index);
            }
        
        return max_pos;
    });
    
    return neural_network.Serialize();
}

std::string DeserializeAndExecuteSeperated(const std::string& serialized_network

void LoadAndRun(const std::string& serialized_file_path, const std::string& test_file_path) {
    
    //Get the number of networks that compose the solution
    
    std::unique_ptr<Network> network(new Network(serialized_file_path));
    
    Trainer trainer;
    trainer.Test("digits_train.csv", "digits_train_key.csv", [&](const Data& data) -> double {
        
        std::vector<double> results = network->Feed(data);
        
        size_t max_pos = 0;
        double max = 0.0;
        for (size_t results_index = 0 ; results_index < results.size() ; results_index++)
            if (results.at(results_index) > max) {
                max_pos = results_index;
                max = results.at(results_index);
            }
        
        return max_pos;
    });
    
}

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
        << "-t\tActivates the test mode. The flag specifies name of the  output file from a previous run. The -i file will be used as the test file and will output the results to the -o file\n\n\n";
    }
    else {
        
        //Handle program lifecycle with C
        char* data_file         = GetOption(argv, argv + argc, "-i");
        char* key_file          = GetOption(argv, argv + argc, "-k");
        char* output_file       = GetOption(argv, argv + argc, "-o");
        char* serialized_file   = GetOption(argv, argv + argc, "-t");
        char* type              = GetOption(argv, argv + argc, "-u");
        
        //Check that the data is valid
        if (!type) {
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
            
            if (*type == '1')       output << SeperatedNetworks(data_file, key_file);
            else if (*type == '2')  output << CombinedNetworks(data_file, key_file);
            
             output.close();
            std::cout << "The network was successfully serialized and saved to " << output_file << "\n";
            return 0;
        }
        else {
            
            //Convert the serialized file by type, and run the test file
            
        }
        
        
    }
    
    return 0;
}
