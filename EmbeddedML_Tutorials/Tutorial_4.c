/*
 * Tutorial_4.c
 *
 * EmbeddedML
 * v1.0
 *
 * Embedded Machine Learning
 * Created by Charles Zaloom on 5/27/18.
 *
 */

/*
 *  Tutorial 4: User Customization
 */

#include "embeddedML.h"
#include "testcases.h"

#include <stdio.h>
#include <math.h>
#include <time.h>

// -> In EmbeddedML, any function that takes a float and returns a float can be an activation function 
// -> For example lets use 'myFunction' and its derivative 'myFunctionDerivative' as defined below.

float myFunction(float x){
    if(x < 0.0) return 0.0;
    else if(x > 1.0) return 1.0+x*0.1;
    return x;
}

float myFunctionDerivative(float x){
    if(x < 0.0) return 0.0;
    else if(x > 1.0) return 0.1;
    return 1.0;
}

void printOutput_ANN( ANN *net ){
    int i;
    printf("OUTPUT: ");
    for(i = 0; i < net->topology[net->n_layers-1]; i++){
      printf("%f ", net->output[i]);
    }
    printf("\n");
}

int main() {

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

    // -> Learning Rate and Momentum are two key hyperparameters that will affect the rate of model convergence.

    net.eta = 0.15;     //Learning Rate
    net.alpha = 0.25;   //Momentum Coefficient

    // -> Now in the model options we can change 'relu' to 'myFunction'

    net.output_activation_function = &myFunction;
    net.hidden_activation_function = &myFunction;

    // -> Note: When defining your own activation function. It is also necessay to declare the derivative as well.
    // ->       This is because the init_ann() function normally fills this requirement.

    net.output_activation_derivative = &myFunctionDerivative;
    net.hidden_activation_derivative = &myFunctionDerivative;

    // -> EmbeddedML comes with 4 functions already
    // ->   - relu and relu2 will always work. 
    // ->   - tanhf and sigmoid will work when _EMBEDDED_ is not defined.

    init_ann(&net);
    float x[2];
    float y[1];
    //---------------------
    
    // -> The number of training cycles is another customizable parameter
    int number_of_training_cycles = 2500;

    unsigned int i;
    for(i = 0; i < number_of_training_cycles; i++){
        generate_xor(x, y);
        train_ann(&net, x, y);
    }

    float x0[2] = {0,0};   
    float x1[2] = {0,1}; 
    float x2[2] = {1,0};  
    float x3[2] = {1,1};  

    run_ann(&net,x0);
    printOutput_ANN(&net);
    
    run_ann(&net,x1);
    printOutput_ANN(&net);
    
    run_ann(&net,x2);
    printOutput_ANN(&net);
    
    run_ann(&net,x3);
    printOutput_ANN(&net);
}
