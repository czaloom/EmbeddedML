/*
 * xor_and_example.c
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
    
    // ------- ANN -------
    float weights[15] = {0.680700,0.324900,0.607300,0.365800,0.693000,0.527200,0.754400,0.287800,0.592300,0.570900,0.644000,0.416500,0.249200,0.704200,0.598700};
    float dedw[15];
    float bias[5];
    unsigned int ANN_topology[3] = {3,3,2};
    float output[2];

    ANN net;
    set_model_memory(&net, weights, dedw, bias, output);
    set_model_parameters(&net, ANN_topology, 3, 'r');
    set_model_hyperparameters(&net, 0.13, 0.01, 0.25);
    init_ann(&net);

    float x[3];
    float y[2];
    //---------------------

    start = clock(); //CLOCK
    
    //TRAINING CYCLES
    unsigned int i;
    for(i = 0; i < 2000; i++){
        generate_xorand(x, y);
        train_ann(&net, x, y);
    }
 
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nTime used: %f\n", cpu_time_used);

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
