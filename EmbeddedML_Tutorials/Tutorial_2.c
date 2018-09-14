/*
 * Tutorial_2.c
 *
 * EmbeddedML
 * v1.0
 *
 * Embedded Machine Learning
 * Created by Charles Zaloom on 5/27/18.
 *
 */

/*
 * Tutorial 2.1: Loading a model from a file. 
 */

#include "embeddedML.h" 
#include "testcases.h"  

#include <stdio.h>
#include <math.h>
#include <time.h>

int main(){

// -> Using designer.c create a template for the 2 2 1 topology network we used in the previous example.
// -> The option will appear as 'load'

    //---LOAD ANN FROM FILE---
    float weights[6];
    float dedw[6];
    float bias[3];
    unsigned int topology[3];
    float y[1];

    ANN net;
    net.weights = weights;
    net.dedw = dedw;
    net.bias = bias;
    net.topology = topology;
    net.output = y;
    load_ann(&net, "model.net");
    float x[2];
    //-------------

// -> The model we trained in Tutorial 1 is now loaded and can be trained further.

    //TRAINING CYCLES
    unsigned int i;
    for(i = 0; i < 10; i++){
        generate_xor(x, y);
        train_ann(&net, x, y);
    }

    // -> Now we can run the network

    x[0] = 0.0;
    x[1] = 1.0;
    run_ann(&net, x);

    float output = net.output[0];

    printf("%f\n", output);

}