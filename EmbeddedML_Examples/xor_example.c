/*
 * xor_example.c
 *
 * EmbeddedML
 * v1.0
 *
 * Embedded Machine Learning
 * Created by Charles Zaloom on 5/27/18.
 *
 */

#include "embeddedML.h"
#include "testcases.h"

#include <stdio.h>
#include <math.h>

void printOutput_ANN( ANN *net ){
    int i;
    printf("OUTPUT: ");
    for(i = 0; i < net->topology[net->n_layers-1]; i++){
      printf("%f ", net->output[i]);
    }
    printf("\n");
}

int main(int argc, const char * argv[]) {
    //---ANN---
    float weights[6];
    float dedw[6];
    float bias[3];
    unsigned int network_topology[3] = {2,2,1};
    float run_error[1];

    ANN net;
    net.weights = weights;
    net.dedw = dedw;
    net.bias = bias;
    net.topology = network_topology;
    net.n_layers = 3;
    net.n_weights = 6;
    net.n_bias = 3;
    net.output = run_error;

    //OPTIONS
    net.eta = 0.13;     //Learning Rate
    net.alpha = 0.25;   //Momentum Coefficient
    net.output_activation_function = &relu;
    net.hidden_activation_function = &relu;

    init_ann(&net);
    float x[2];
    float y[1];
    //---------------------
    
    //TRAINING CYCLES
    unsigned int i;
    for(i = 0; i < 200000; i++){
        generate_xor(x, y);
        train_ann(&net, x, y);
    }

    //TEST SAMPLES
    float x0[2] = {0,0};  // output 0 
    float x1[2] = {0,1};  // output 1 
    float x2[2] = {1,0};  // output 1 
    float x3[2] = {1,1};  // output 0 

    //RUNNING A TRAINED NETWORK
    run_ann(&net,x0);
    printOutput_ANN(&net);
    
    run_ann(&net,x1);
    printOutput_ANN(&net);
    
    run_ann(&net,x2);
    printOutput_ANN(&net);
    
    run_ann(&net,x3);
    printOutput_ANN(&net);
}
