/*
 * Tutorial_3.c
 *
 * Embedded ML
 * v1.0
 *
 * Embedded Machine Learning
 * Created by Charles Zaloom on 5/27/18.
 *
 */

/*
 * Tutorial 3.1: Using the library on SensorTile (and other resource limited platforms)
 */

#define _EMBEDDED_  // -> By defining _EMBEDDED_ all supportive functions are disabled
                    // -> This means no saving, loading, as well as some maath functions beign disabled.
                    // -> In return there are no dependencies outside of the EmbeddedML library.

#include "embeddedML.h" 
#include "testcases.h"  

#include <stdio.h>
#include <math.h>

int main() {

    // -> When _EMBEDDED_ is defined use the Create New Embedded ANN option in Designer.c
    
    //---EMBEDDED ANN---
    float weights[6] = {0.680700,0.324900,0.607300,0.365800,0.693000,0.527200};
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
    net.eta = 0.25;     //Learning Rate
    net.alpha = 0.15;   //Momentum Coefficient
    net.output_activation_function = &relu;
    net.hidden_activation_function = &relu;

    init_ann(&net);
    float x[2];
    float y[1];
    //---------------------
    
    //TRAINING CYCLES
    unsigned int i;
    for(i = 0; i < 500; i++){
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
    printf("Output: %f\n", net.output[0]);
    
    run_ann(&net,x1);
    printf("Output: %f\n", net.output[0]);
    
    run_ann(&net,x2);
    printf("Output: %f\n", net.output[0]);
    
    run_ann(&net,x3);
    printf("Output: %f\n", net.output[0]);
}
