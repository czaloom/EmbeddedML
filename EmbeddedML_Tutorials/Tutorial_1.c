/*
 * Tutorial_1.c
 *
 * EmbeddedML
 * v1.0
 *
 * Embedded Machine Learning
 * Created by Charles Zaloom on 5/27/18.
 *
 */

/*
 * Tutorial 1.1: The ANN Strucuture and core functions. 
 */

#include "embeddedML.h" // -> embeddedML.h is the main library.
#include "testcases.h"  // -> testcases.h is a supportive header for the example programs.

#include <stdio.h>
#include <math.h>
#include <time.h>

void get_training_data(float *x, float *y){
    x[0] = 0;
    x[1] = 1;
    y[0] = 1;
}

int main(){

    // -> The next section of code in between the dashed lines is the initialization of a ANN model.
    // -> The user is not required to ever write this, as it is generated code from the designer.c program.

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
    net.eta = 0.15;     //Learning Rate
    net.alpha = 0.25;   //Momentum Coefficient
    net.output_activation_function = &relu;
    net.hidden_activation_function = &relu;

    // -> User options are listed here.
    // -> Learning rate and momentum should be tweaked to find a fast convergence.
    // -> The activation functions offered are:
    // ->       - relu
    // ->       - relu2
    // ->       - sigmoid
    // ->       - tanhf

    init_ann(&net); 
    float x[2];
    float y[1];
    //---------
    
    // -> As can be seen by the last three lines of the generated code, the model sets up input and training varibles as well as slef-initialization.
    // -> x[2] is the input array
    // -> y[1] is the desired output corresponding to the input 

    // -> Using these arrays lets train on one set of data:

    x[0] = 0.0;
    x[1] = 1.0; 
    y[0] = 1.0;
    train_ann(&net, x, y);

    // -> Now we can run the model using:

    x[0] = 0.0;
    x[1] = 0.0;
    run_ann(&net, x);
 
    // -> The output to the model is stored into:

    float my_models_output = net.output[0];

/*
 * Tutorial 1.2: Training Implementation 
 */

// -> Obviously one training cycles is not enough to train a model.
// -> A better implementation is:

    unsigned int i;
    int number_of_training_cycles = 10;
    
    for(i = 0; i < number_of_training_cycles; i++){
        get_training_data(x, y);
        train_ann(&net, x, y);
    }

// -> Using a XOR input generator from testcases.h we can simulate an input and desired output.

    //TRAINING CYCLES
    for(i = 0; i < 1000; i++){
        generate_xor(x, y);
        train_ann(&net, x, y);
    }

// -> The model should now be trained to behave like an XOR gate.
// -> You can run this the same way as above:
    
    x[0] = 1.0;
    x[0] = 0.0;
    run_ann(&net, x);

    printf("%f\n", net.output[0]);

// -> To save a model to a file, use save_ann(&net, "myfile.net")

    save_ann(&net, "model.net");
}
