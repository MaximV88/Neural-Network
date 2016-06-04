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

std::string SeperatedNetworks() {
    
    std::vector<Network*> networks;
    
    //Output layer will have only 1 neuron
    for (size_t index = 0 ; index < 10 ; index++) {
        
        Network* new_network = new Network(80);
        new_network->AddNetwork(19);
        new_network->AddNetwork(1);
        
        networks.push_back(new_network);
    }

    Trainer trainer("digits_train.csv", "digits_train_key.csv");
    
    //Train against 90% of data and compare the remaining 10%
    trainer.Train(90, [&](const Data& data, size_t key) {
        
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

std::string CombinedNetworks() {
    
    Network neural_network(310);
    neural_network.AddNetwork(200);
    neural_network.AddNetwork(200);
    neural_network.AddNetwork(180);
    neural_network.AddNetwork(80);
    neural_network.AddNetwork(10);
    
    Trainer trainer("digits_train.csv", "digits_train_key.csv");
    
    //Train against 90% of data and compare the remaining 10%
    trainer.Train(5, [&](const Data& data, size_t key) {
        
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

void LoadAndRun(const std::string& serialized_file_path, const std::string& test_file_path) {
    
    //Get the number of networks that compose the solution
    
    std::unique_ptr<Network> network(new Network(serialized_file_path));
    
    Trainer trainer("digits_train.csv", "digits_train_key.csv");
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

int main(int argc, const char * argv[]) {
    
    //Print the clusters to a file
//    std::ofstream output("output.txt");
//    output << CombinedNetworks();
//    output.close();
    
    Network neural_network(310);
    neural_network.AddNetwork(200);
    neural_network.AddNetwork(200);
    neural_network.AddNetwork(180);
    neural_network.AddNetwork(80);
    neural_network.AddNetwork(10);

    LoadAndRun(neural_network.Serialize(), "test.txt");
    
    return 0;
}
