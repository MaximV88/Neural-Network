//
//  main.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 03/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <iostream>
#include "Network.hpp"
#include "DataIterator.hpp"

void SeperatedNetworks() {
    
    using namespace neural;
    std::vector<Network*> networks;
    
    //Each network will contain
    for (size_t index = 0 ; index < 10 ; index++) {
        
        Network* new_network = new Network(2);
        
        new_network->AddNetwork(68);
        new_network->AddNetwork(29);
        
        //Output layer will have only 1 neuron
        new_network->AddNetwork(1);
        
        networks.push_back(new_network);
    }
    
    DataIterator data("digits_train.csv");
    DataIterator results("digits_train_key.csv");
    
    //Train all networks
    size_t index = 0;
    size_t correct = 0;
    for (DataIterator data("digits_train.csv"), results("digits_train_key.csv") ;
         data.Valid() && results.Valid() ;
         data.Next(), results.Next()) {
        
        size_t real_value = static_cast<size_t>(lround(results.Value().content.front()));
        
        Data current_data = data.Value();
        Data modified_data;
        modified_data.content.reserve(784);
        for (size_t i = 0 ; i < 784 ; i++)
            modified_data.content.push_back(current_data.content.at(i) > 50 ? 1.0 : 0.0);
        
        if (index < 40000) {
            
            if (++index % 400 == 0) {
                
                std::cout << "overall progress:" << index/40000.0 * 100 << '%' << '\n';
                
            }
            
            for (size_t network_index = 0 ; network_index < 10 ; network_index++) {
                
                //Get the network to identify the number based on it's index
                Data modified_result;
                modified_result.content = std::vector<double>(1, (real_value == network_index) ? 1.0 : 0.0);
                
                for (int i = 0 ; i < 10 ; i ++ )
                    networks[network_index]->Train(modified_data, modified_result);
                
            }
            
        }
        else {
            
            //Validate
            
            double max = 0.0;
            size_t max_index = 0;
            for (size_t network_index = 0 ; network_index < 10 ; network_index++) {
                if (networks[network_index]->Feed(modified_data).front() > max) {
                    max = networks[network_index]->Feed(modified_data).front();
                    max_index = network_index;
                }
            }
            
            if (max_index == real_value) ++correct;
            
            if (++index % 200 == 0) {
                
                std::cout << "correctness: " << correct/((double)index-40000) * 100 << "%\t\toverall progress:" << (index-40000)/20000.0 * 100 << '%' << '\n';
                
            }
            
        }
    }

}

void CombinedNetworks() {
    
    using namespace neural;
    
    Network neural_network(10);
    neural_network.AddNetwork(300);
    neural_network.AddNetwork(200);
    neural_network.AddNetwork(10);
    
    DataIterator data("digits_train.csv");
    DataIterator results("digits_train_key.csv");
    
    //Train all networks
    size_t index = 0;
    size_t total_correct = 0;
    for (DataIterator data("digits_train.csv"), results("digits_train_key.csv") ;
         data.Valid() && results.Valid() ;
         data.Next(), results.Next()) {
        
        size_t real_value = static_cast<size_t>(lround(results.Value().content.front()));
        Data current_data = data.Value();
        Data modified_data;
        modified_data.content.reserve(784);
        for (size_t i = 0 ; i < 784 ; i++)
            modified_data.content.push_back(current_data.content.at(i) > 50 ? 1.0 : 0.0);
        
        ++index;
        
        //Calculate a test
        std::vector<double> network_results = neural_network.Feed(modified_data);
        
        size_t max_pos = 0;
        double max = 0.0;
        for (size_t results_index = 0 ; results_index < network_results.size() ; results_index++)
            if (network_results.at(results_index) > max) {
                max_pos = results_index;
                max = network_results.at(results_index);
            }
        
        if (max_pos == real_value) ++ total_correct;
        
        if (index % 1000 == 0) {
            
            std::cout.precision(2);
            std::cout << "correctness: " << (total_correct /static_cast<double>(index)) * 100
            << "%\t\t\t" << "overall progress:" << index/60000.0 * 100 << '%' << '\n';
        
        }
        
        /*
         * The results need to be translated to a form that the last network can understand.
         * Since there are 10 neurons in the last network their position will indicate the
         * value of the number. So, a vector that represents something similar will need to
         * interpreted from the result and sent to the network for training.
         */
    
        Data modified_result;
        modified_result.content = std::vector<double>(10, 0.0);
        modified_result.content[real_value] = 1.0;
        
        for (int i = 0 ; i < 10 ; i ++ )
            neural_network.Train(modified_data, modified_result);
    }

}

int main(int argc, const char * argv[]) {

    using namespace neural;

    CombinedNetworks();
    return 0;
}
