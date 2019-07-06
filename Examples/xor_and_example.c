/*
 * xor_and_example.c
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
#include <time.h>

void printOutput_ANN( ANN *net ){
    int i;
    printf("OUTPUT: ");
    for(i = 0; i < net->topology[net->n_layers-1]; i++){
      printf("%f ", net->output[i]);
    }
    printf("\n");
}

clock_t start, end;
double cpu_time_used;

int main(int argc, const char * argv[]) {
    //---NETWORK_BUILDER---
    float weights[15];
    float dedw[15];
    float bias[5];
    unsigned int ANN_topology[3] = {3,3,2};
    float error[2];
    ANN net;
    net.weights = weights;
    net.dedw = dedw;
    net.bias = bias;
    net.topology = ANN_topology;
    net.n_layers = 3;
    net.n_weights = 15;
    net.n_bias = 5;
    net.output = error;

    //OPTIONS
    net.eta = 0.13;     //Learning Rate
    net.alpha = 0.25;   //Momentum Coefficient
    net.output_activation_function = &relu;
    net.hidden_activation_function = &relu;

    init_ann(&net);
    float x[3];
    float y[2];
    //---------------------

    start = clock(); //CLOCK
    
    //TRAINING CYCLES
    unsigned int i;
    for(i = 0; i < 1000; i++){
        generate_xorand(x, y);
        train_ann(&net, x, y);
    }
 
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nTime used: %f\n", cpu_time_used);
    printf("Error: %f\n\n", net.output[0]);

    //TEST SAMPLES
    float x0[3] = {0,0,0};  // output 0 0 
    float x1[3] = {0,1,0};  // output 1 0
    float x2[3] = {0,1,1};  // output 1 1
    float x3[3] = {1,1,1};  // output 0 1
    float x4[3] = {0,0,1};  // output 0 0

    //RUNNING A TRAINED NETWORK
    run_ann(&net,x0);
    printOutput_ANN(&net);
    
    run_ann(&net,x1);
    printOutput_ANN(&net);
    
    run_ann(&net,x2);
    printOutput_ANN(&net);
    
    run_ann(&net,x3);
    printOutput_ANN(&net);
    
    run_ann(&net,x4);
    printOutput_ANN(&net);
}
