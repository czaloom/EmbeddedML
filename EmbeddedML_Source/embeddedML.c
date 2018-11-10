/* EMBEDDEDML V1.1 */
/*
    embeddedML.c - Embedded Machine Learning Library
    Copyright (C) 2018  Charles Zaloom

    EmbeddedML is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    EmbeddedML is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with EmbeddedML.  If not, see <https://www.gnu.org/licenses/>
*/

#include "embeddedML.h"

//-----ANN-----
void BP_ANN(ANN *net, float *input, float *output, float *weights, float *velocity, float *bias, float *delta, unsigned int *_delta, int depth){
    unsigned int i,j,k;
    unsigned int _currW[2] = {net->topology[net->n_layers - depth], net->topology[net->n_layers - depth - 1]};
    unsigned int _X[2] = {net->topology[net->n_layers - depth - 1],1};

    float a[net->topology[net->n_layers - depth]];
    float d[net->topology[net->n_layers - depth]];
    
    for(i = 0; i < _currW[0]; i++){
        for(j = 0; j < _X[1]; j++){
            a[(_X[1]*i)+j] = 0.0;
            for(k = 0; k < _currW[1]; k++){
                a[(_X[1]*i)+j] += weights[(_currW[1]*i)+k]*input[(_X[1]*k)+j];
            }
        }
    }

    unsigned int _X_T[2] = {1,net->topology[net->n_layers - depth - 1]};
    if(depth == 1){
        for(i = 0; i < net->topology[net->n_layers - depth]; i++){
            a[i] += bias[i];
            d[i] = net->output_activation_derivative(a[i]);
            a[i] = net->output_activation_function(a[i]);
        }

        for(i = 0; i < net->topology[net->n_layers-1]; i++){
            net->output[i] = (output[i] - a[i]);
            delta[i] = net->output[i] * d[i];
        }

        float dEdW[_currW[0]*_currW[1]];
        for(i = 0; i < _delta[0]; i++){
            for(j = 0; j < _X_T[1]; j++){
                dEdW[(_X_T[1]*i)+j] = 0.0;
                for(k = 0; k < _delta[1]; k++){
                    dEdW[(_X_T[1]*i)+j] += delta[(_delta[1]*i)+k]*input[(_X_T[1]*k)+j];
                }
            }
        }
        for(i = 0; i < _currW[0]; i++){
            bias[i] = bias[i] + delta[i]*0.1; 
        }
        for(i = 0; i < _currW[0]*_currW[1]; i++){
            velocity[i] = dEdW[i]*net->eta - velocity[i]*net->alpha;
            weights[i] = weights[i] + velocity[i];
        }
        return;
    }
    else{
        for(i = 0; i < net->topology[net->n_layers - depth]; i++){
            a[i] += bias[i];
            d[i] = net->hidden_activation_derivative(a[i]);
            a[i] = net->hidden_activation_function(a[i]);
        }

        unsigned int _nextW_T[2] = {net->topology[net->n_layers - depth], net->topology[net->n_layers - depth + 1]};
        unsigned int _nextW[2] = {net->topology[net->n_layers - depth + 1], net->topology[net->n_layers - depth]};
        unsigned int _prevDelta[2] = {net->topology[net->n_layers - depth + 1], 1};
        
        float prev_delta[net->topology[net->n_layers - depth + 1]];
        unsigned int weight_iter = net->topology[net->n_layers - depth] * net->topology[net->n_layers - depth - 1];
        unsigned int bias_iter = net->topology[net->n_layers - depth];
       
        float next_weights_T[_nextW_T[0]*_nextW_T[1]];
        unsigned int iter = 0;
        for(i = 0; i < _nextW[1]; i++){
            for(j = 0; j < _nextW[0]; j++){
                next_weights_T[iter] = weights[(_nextW[1]*j)+i+weight_iter];
                iter++;
            }
        }
    
        BP_ANN(net, a, output, &weights[weight_iter], &velocity[weight_iter], &bias[bias_iter], prev_delta, _prevDelta, depth-1);
        
        for(i = 0; i < _nextW_T[0]; i++){
            for(j = 0; j < _prevDelta[1]; j++){
                delta[(_prevDelta[1]*i)+j] = 0;
                for(k = 0; k < _nextW_T[1]; k++){
                    delta[(_prevDelta[1]*i)+j] += next_weights_T[(_nextW_T[1]*i)+k]*prev_delta[(_prevDelta[1]*k)+j];
                }
                delta[(_prevDelta[1]*i)+j] = delta[(_prevDelta[1]*i)+j]*d[(_prevDelta[1]*i)+j];
            }
        }
        float dEdW[_currW[0]*_currW[1]];
        for(i = 0; i < _delta[0]; i++){
            for(j = 0; j < _X_T[1]; j++){
                dEdW[(_X_T[1]*i)+j] = 0.0;
                for(k = 0; k < _delta[1]; k++){
                    dEdW[(_X_T[1]*i)+j] += delta[(_delta[1]*i)+k]*input[(_X_T[1]*k)+j];
                }
            }
        }
        for(i = 0; i < _currW[0]; i++){
            bias[i] = bias[i] + delta[i]*0.1; 
        }
        for(i = 0; i < _currW[0]*_currW[1]; i++){
            velocity[i] = dEdW[i]*net->eta - velocity[i]*net->alpha;
            weights[i] = weights[i] + velocity[i];
        }
        return;
    }
}

void train_ann(ANN *net, float *input, float *output){
    float delta[net->topology[1]];
    unsigned int delta_dimensions[2] = {net->topology[1],1};
    BP_ANN(net, input, output, net->weights, net->dedw, net->bias, delta, delta_dimensions, net->n_layers-1);
}

void FP_ANN(ANN *net, float *input, unsigned int depth, float *weights){
    unsigned int _currW[2] = {net->topology[net->n_layers - depth], net->topology[net->n_layers - depth - 1]};
    unsigned int _X[2] = {net->topology[net->n_layers - depth - 1],1};
    
    unsigned int i,j,k;
    float a[net->topology[net->n_layers - depth]];
    for(i = 0; i < _currW[0]; i++){
        for(j = 0; j < _X[1]; j++){
            a[(_X[1]*i)+j] = 0.0;
            for(k = 0; k < _currW[1]; k++){
                a[(_X[1]*i)+j] += weights[(_currW[1]*i)+k]*input[(_X[1]*k)+j];
            }
        }
    }
    
    if(depth == 1){
        for(i = 0; i < net->topology[net->n_layers - depth]; i++){
            a[i] += net->bias[i];
            a[i] = net->output_activation_function(a[i]);
        }

        for(i = 0; i < net->topology[net->n_layers-1]; i++){
            net->output[i] = a[i];
        }
        return;
    }
    else{
        for(i = 0; i < net->topology[net->n_layers - depth]; i++){
            a[i] += net->bias[i];
            a[i] = net->hidden_activation_function(a[i]);
        }
        unsigned int weights_iter = (_currW[0]*_currW[1]);
        FP_ANN(net, a, depth-1, &weights[weights_iter]);
    }
    return;
}

void run_ann(ANN *net, float *input){
    FP_ANN(net, input, net->n_layers-1, net->weights);
}

void init_ann(ANN *net){ 
    #ifndef _EMBEDDED_
        fill_rand(net->weights, net->n_weights, 0.2, 0.8);
    #endif
    fill_zeros(net->dedw, net->n_weights);
    fill_number(net->bias, net->n_bias,0.01);

    if(net->output_activation_function == &relu) net->output_activation_derivative = &relu_derivative;
    else if(net->output_activation_function == &relu2) net->output_activation_derivative = &relu2_derivative;
    #ifndef _EMBEDDED_
        else if(net->output_activation_function == &tanhf) net->output_activation_derivative = &tanhf_derivative;
        else if(net->output_activation_function == &sigmoid) net->output_activation_derivative = &sigmoid_derivative;
    #endif
    
    if(net->hidden_activation_function == &relu) net->hidden_activation_derivative = &relu_derivative;
    else if(net->hidden_activation_function == &relu2) net->hidden_activation_derivative = &relu2_derivative;
    #ifndef _EMBEDDED_
        else if(net->hidden_activation_function == &tanhf) net->hidden_activation_derivative = &tanhf_derivative;
        else if(net->hidden_activation_function == &sigmoid) net->hidden_activation_derivative = &sigmoid_derivative;
    #endif
}

#ifndef _EMBEDDED_
    void save_ann(ANN *net, const char *filename){ //REPLACE - SAVE
        FILE *outfile;
        outfile = fopen(filename, "w");
        if (outfile == NULL){
            fprintf(stderr, "\nError opening file\n");
            exit (1);
        }
        fprintf(outfile, "%d\n", net->n_layers);
        unsigned int i, n_bias = 0, n_weights = 0;
        for(i = 1; i < net->n_layers; i++){
            n_bias += net->topology[i];
            n_weights += net->topology[i]*net->topology[i-1];
        }
        for(i = 0; i < net->n_layers; i++){
            fprintf(outfile, "%d ", net->topology[i]);
        }
        fprintf(outfile, "\n");
        for(i = 0; i < n_weights; i++){
            fprintf(outfile, "%f ", net->weights[i]);
        }
        fprintf(outfile, "\n");
        for(i = 0; i < n_bias; i++){
            fprintf(outfile, "%f ", net->bias[i]);
        }
        fprintf(outfile, "\n");

        if(net->hidden_activation_function == &relu) fprintf(outfile, "r ");
        else if(net->hidden_activation_function == &relu2) fprintf(outfile, "R ");
        else if(net->hidden_activation_function == &tanhf) fprintf(outfile, "t ");
        else if(net->hidden_activation_function == &sigmoid) fprintf(outfile, "s ");
        else fprintf(outfile, "u ");

        if(net->output_activation_function == &relu) fprintf(outfile, "r ");
        else if(net->output_activation_function == &relu2) fprintf(outfile, "R ");
        else if(net->output_activation_function == &tanhf) fprintf(outfile, "t ");
        else if(net->output_activation_function == &sigmoid) fprintf(outfile, "s ");
        else fprintf(outfile, "u ");
        fprintf(outfile, "\n");

        fprintf(outfile, "%f ", net->eta);
        fprintf(outfile, "%f ", net->alpha);
        fprintf(outfile, "\n");

        fclose(outfile);
    }

    void load_ann(ANN *net, const char *filename){
        FILE *infile;
         
        infile = fopen (filename, "r");
        if (infile == NULL)
        {
            printf("\nError opening file\n");
            exit (1);
        }
        fscanf(infile, "%d\n", &net->n_layers);
        unsigned int i;
        for(i = 0; i < net->n_layers; i++){
            fscanf(infile, "%d", &net->topology[i]);
        }
        unsigned int n_bias = 0, n_weights = 0;
        for(i = 1; i < net->n_layers; i++){
            n_bias += net->topology[i];
            n_weights += net->topology[i]*net->topology[i-1];
        }

        net->n_weights = n_weights;
        net->n_bias = n_bias;

        for(i = 0; i < n_weights; i++){
            fscanf(infile, "%f", &net->weights[i]);
        }
        for(i = 0; i < n_bias; i++){
            fscanf(infile, "%f", &net->bias[i]);
        }

        char actfunc;
        i = 0;
        while(i != 2){ //This loop exists to make sure any newlines are omitted
            fscanf(infile, "%c", &actfunc);
            if(i == 0){
                if(actfunc == 'r'){
                    net->hidden_activation_derivative = &relu_derivative;
                    net->hidden_activation_function = &relu;
                    i++;
                }
                else if(actfunc == 'R'){
                    net->hidden_activation_derivative = &relu2_derivative;
                    net->hidden_activation_function = &relu2;
                    i++;
                }
                else if(actfunc == 't'){ 
                    net->hidden_activation_derivative = &tanhf_derivative;
                    net->hidden_activation_function = &tanhf;
                    i++;
                }
                else if(actfunc == 's'){ 
                    net->hidden_activation_derivative = &sigmoid_derivative;
                    net->hidden_activation_function = &sigmoid;
                    i++;
                }
                else if(actfunc == 'u'){ 
                    printf("User Defined Activation Function\n");
                    i++;
                }
            }
            else{
                if(actfunc == 'r'){ 
                    net->output_activation_derivative = &relu_derivative;
                    net->output_activation_function = &relu;
                    i++;
                }
                else if(actfunc == 't'){
                    net->output_activation_derivative = &tanhf_derivative;
                    net->output_activation_function = &tanhf;
                    i++;
                }
                else if(actfunc == 's'){
                    net->output_activation_derivative = &sigmoid_derivative;
                    net->output_activation_function = &sigmoid;
                    i++;
                }
                else if(actfunc == 'u'){ 
                    printf("User Defined Activation Function\n");
                    i++;
                }
            }
        }
        fscanf(infile, "%f", &net->eta);
        fscanf(infile, "%f", &net->alpha);

        fill_zeros(net->dedw, net->n_weights);

        fclose(infile);
    }
#endif

//-----Utility-----

#ifndef _EMBEDDED_
    void fill_rand(float *v, unsigned int size, float lower, float upper){
        int i;
        int up = ((upper - lower)*10000.0);
        for(i = 0; i < size; i++){
            v[i] = ((rand()%up)/10000.0)+lower;
        }
    }
#endif

void fill_zeros(float *v, unsigned int size){
    int i;
    for(i = 0; i < size; i++){ v[i] = 0.0; }
}
void fill_number(float *v, unsigned int size, float number){
    int i;
    for(i = 0; i < size; i++){ v[i] = number; }
}

#ifndef _EMBEDDED_
    void softmax(unsigned int size, float multiplier, float *x, float *y){
        int i; 
        float sum;
        for(i = 0; i < size; i++){
            sum += exp(x[i] * multiplier);
        }
        for(i = 0; i < size; i++){
            y[i] = exp(x[i] * multiplier) / (sum);
        }
    }

    void softmax2(unsigned int size, float multiplier, float *x, float *y) {
        int i;
        float sum;
        for(i = 0; i < size; i++){
            if(x[i] < 0.0) sum += (-1.0 * x[i] * multiplier);
            else sum += (x[i] * multiplier);
        }
        for(i = 0; i < size; i++){
            y[i] = (x[i] * multiplier) / sum; 
        }
    }
#endif

void weak_softmax(float *x, float *y) {
    //For use with systems that have low-precision floating point number storage.
    
    //NOTE: Adjust size as required, the low-precision requires the computation to be defined in a single line.

    //NOTE: The Designer program has a tool to format this function to other input sizes.

    float multiplier = 1.0;
    unsigned int size = 3;

    y[0] = (x[0]*x[0]*multiplier) / ((x[0]*x[0]*multiplier) + (x[1]*x[1]*multiplier) + (x[2]*x[2]*multiplier));
    y[1] = (x[1]*x[1]*multiplier) / ((x[0]*x[0]*multiplier) + (x[1]*x[1]*multiplier) + (x[2]*x[2]*multiplier));
    y[2] = (x[2]*x[2]*multiplier) / ((x[0]*x[0]*multiplier) + (x[1]*x[1]*multiplier) + (x[2]*x[2]*multiplier));

    int i;
    for(i = 0; i < size; i++){
        if(x[i] < 0.0) y[i] = y[i] * -1.0;
    }
}

//-----Activation Functions-----
float relu(float x){
    if(x < 0.0) return 0.0;
    else if(x > 1.0) return 1.0+x*0.1;
    return x;
}

//Similar to Tanh
float relu2(float x){
    if(x < -1.0) return 0.1*x-0.9;
    else if(x > 1.0) return 0.1*x+0.9;
    return x;
}

#ifndef _EMBEDDED_
    float sigmoid(float x){
        return 1 / (1 + exp(-x));
    }
#endif

//-----Derivative Functions-----
float relu_derivative(float x){
    if(x < 0.0) return 0.0;
    else if(x > 1.0) return 0.1;
    return 1.0;
}
float relu2_derivative(float x){
    if(x < -1.0) return 0.1;
    else if(x > 1.0) return 0.1;
    return 1.0;
}

#ifndef _EMBEDDED_
    float tanhf_derivative(float x){
        x = tanhf(x);
        return 1 - (x*x);
    }
    float sigmoid_derivative(float x){
        return sigmoid(x) * (1 - sigmoid(x));
    }
#endif
