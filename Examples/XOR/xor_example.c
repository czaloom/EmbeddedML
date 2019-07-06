/*
 * xor_example.c
 *
 * EmbeddedML
 * v1.3
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
    float weights[6] = {0.680700,0.324900,0.607300,0.365800,0.693000,0.527200};
    float dedw[6];
    float bias[3];
    unsigned int network_topology[3] = {2,2,1};
    float output[1];

    ANN net;
    set_model_memory(&net, weights, dedw, bias, output);
    set_model_parameters(&net, network_topology, 3, 'r');
    set_model_hyperparameters(&net, 0.13, 0.01, 0.25);
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
