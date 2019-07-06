/* 
 * EMBEDDEDML v1.3b
 * Lightweight Version
 * - Optimized for resource constrained platforms.
 */

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
void BP_ANN(ANN *net, float *input, float *output, float *weights, float *velocity, float *bias, float *delta, int depth){
    unsigned int i,j;
    unsigned int DIM[2] = {net->topology[net->n_layers - depth], net->topology[net->n_layers - depth - 1]};

    if(depth == 1){
        for(i = 0; i < DIM[0]; i++){
            net->output[i] = 0.0;
            for(j = 0; j < DIM[1]; j++){
                net->output[i] += weights[(DIM[1]*i)+j]*input[j];
            }
            net->output[i] = net->output[i] + bias[i];
            delta[i] = (output[i]-net->output_activation_function(net->output[i])) * net->output_activation_derivative(net->output[i]);
            net->output[i] = net->output_activation_function(net->output[i]);
            bias[i] = bias[i] + delta[i]*net->beta;
        }

        float dEdW[DIM[0]*DIM[1]];
        for(i = 0; i < DIM[0]; i++){
            for(j = 0; j < DIM[1]; j++){
                dEdW[(DIM[1]*i)+j] = delta[i]*input[j];
            }
        }
        for(i = 0; i < DIM[0]*DIM[1]; i++){
            velocity[i] = dEdW[i]*net->eta - velocity[i]*net->alpha;
            weights[i] = weights[i] + velocity[i];
        }
        return;
    }
    else{
        float a[DIM[0]];
        float d[DIM[0]];

        for(i = 0; i < DIM[0]; i++){
            a[i] = 0.0;
            for(j = 0; j < DIM[1]; j++){
                a[i] += weights[(DIM[1]*i)+j]*input[j];
            }
            a[i] += bias[i];
            d[i] = net->hidden_activation_derivative(a[i]);
            a[i] = net->hidden_activation_function(a[i]);
        }

        unsigned int DIM1 = net->topology[net->n_layers - depth + 1];

        float prev_delta[DIM1];
        unsigned int weight_iter = DIM[0] * DIM[1];

        float next_weights_T[DIM[0]*DIM1];
        unsigned int iter = 0;
        for(i = 0; i < DIM[0]; i++){
            for(j = 0; j < DIM1; j++){
                next_weights_T[iter] = weights[(DIM[0]*j)+i+weight_iter];
                iter++;
            }
        }

        BP_ANN(net, a, output, &weights[weight_iter], &velocity[weight_iter], &bias[DIM[0]], prev_delta, depth-1);

        for(i = 0; i < DIM[0]; i++){
            delta[i] = 0;
            for(j = 0; j < DIM1; j++){
                delta[i] += next_weights_T[(DIM1*i)+j]*prev_delta[j];
            }
            delta[i] = delta[i]*d[i];
            bias[i] = bias[i] + delta[i]*net->beta;
        }
        float dEdW[DIM[0]*DIM[1]];
        for(i = 0; i < DIM[0]; i++){
            for(j = 0; j < DIM[1]; j++){
                dEdW[(DIM[1]*i)+j] = delta[i]*input[j];
            }
        }
        for(i = 0; i < DIM[0]*DIM[1]; i++){
            velocity[i] = dEdW[i]*net->eta - velocity[i]*net->alpha;
            weights[i] = weights[i] + velocity[i];
        }
        return;
    }
}

void train_ann(ANN *net, float *input, float *output){
    float delta[net->topology[1]];
    BP_ANN(net, input, output, net->weights, net->dedw, net->bias, delta, net->n_layers-1);
}

void FP_ANN(ANN *net, float *input, unsigned int depth, float *weights){
    unsigned int DIM[2] = {net->topology[net->n_layers - depth], net->topology[net->n_layers - depth - 1]};
    unsigned int i,j,k;

    if(depth == 1){
        for(i = 0; i < DIM[0]; i++){
            net->output[i] = 0.0;
            for(k = 0; k < DIM[1]; k++){
                net->output[i] += weights[(DIM[1]*i)+k]*input[k];
            }
            net->output[i] = net->output_activation_function(net->output[i] + net->bias[i]);
        }
        return;
    }
    else{
        float a[DIM[0]];
        for(i = 0; i < DIM[0]; i++){
            a[i] = 0.0;
            for(k = 0; k < DIM[1]; k++){
                a[i] += weights[(DIM[1]*i)+k]*input[k];
            }
            a[i] = net->hidden_activation_function(a[i] + net->bias[i]);
            //if(depth == 2) printf("%f,", a[i]);
        }
        //if(depth == 2) printf("0\n");
        FP_ANN(net, a, depth-1, &weights[DIM[0]*DIM[1]]);
    }
    return;
}

void run_ann(ANN *net, float *input){
    FP_ANN(net, input, net->n_layers-1, net->weights);
}

void init_ann(ANN *net){
    fill_number(net->bias, net->n_bias, 0.1);
    fill_zeros(net->dedw, net->n_weights);

    if(net->output_activation_function == &relu) net->output_activation_derivative = &relu_derivative;
    else if(net->output_activation_function == &relu2) net->output_activation_derivative = &relu2_derivative;

    if(net->hidden_activation_function == &relu) net->hidden_activation_derivative = &relu_derivative;
    else if(net->hidden_activation_function == &relu2) net->hidden_activation_derivative = &relu2_derivative;
}

void init_pretrained_ann(ANN *net){
    fill_zeros(net->dedw, net->n_weights);

    if(net->output_activation_function == &relu) net->output_activation_derivative = &relu_derivative;
    else if(net->output_activation_function == &relu2) net->output_activation_derivative = &relu2_derivative;

    if(net->hidden_activation_function == &relu) net->hidden_activation_derivative = &relu_derivative;
    else if(net->hidden_activation_function == &relu2) net->hidden_activation_derivative = &relu2_derivative;
}

//-----Utility-----
void fill_zeros(float *v, unsigned int size){
    int i;
    for(i = 0; i < size; i++){ v[i] = 0.0; }
}
void fill_number(float *v, unsigned int size, float number){
    int i;
    for(i = 0; i < size; i++){ v[i] = number; }
}

//-----Activation Functions-----
float relu(float x){
    if(x < 0.0) return 0.0;
    else if(x > 1.0) return 0.1*x+0.93;
    return x;
}

//Similar to Tanh
float relu2(float x){
    if(x < -1.0)     return 0.1*x-0.93;
    else if(x > 1.0) return 0.1*x+0.93;
    return x;
}

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

//----Wrapper Functions-----

void set_model_memory(ANN *model, float *weights, float *dedw, float *bias, float *output){
    model->weights = weights;
    model->dedw = dedw;
    model->bias = bias;
    model->output = output;
}

void set_model_parameters(ANN *model, unsigned int *topology, unsigned int nlayers, char activation_function){
    model->topology = topology;
    model->n_layers = nlayers;

    int i;
    int nweights = 0, nbias = 0;
    for(i = 1; i < nlayers; i++){
        nweights += topology[i]*topology[i-1];
        nbias += topology[i-1];
    }

    model->n_weights = nweights;
    model->n_bias = nbias;

    switch(activation_function){
        case 'r':
            model->output_activation_function = &relu;
            model->hidden_activation_function = &relu;
            break;
        case 'R':
            model->output_activation_function = &relu2;
            model->hidden_activation_function = &relu2;
            break;
        case 's':
            model->output_activation_function = &sigmoid;
            model->hidden_activation_function = &sigmoid;
            break;
        case 't':
            model->output_activation_function = &tanhf;
            model->hidden_activation_function = &tanhf;
            break;
        default:
            model->output_activation_function = &relu;
            model->hidden_activation_function = &relu;
            break;
    }
}

void set_model_hyperparameters(ANN *model, float learning_rate, float bias_learning_rate, float momentum_factor){
    model->eta = learning_rate;
    model->beta = bias_learning_rate;
    model->alpha = momentum_factor;
}

void set_learning_rate(ANN *model, float eta){
    model->eta = eta;
}

void set_bias_learning_rate(ANN *model, float beta){
    model->beta = beta;
}

void set_momentum_factor(ANN *model, float alpha){
    model->alpha = alpha;
}

void set_output_actfunc(ANN *model, char func){
    switch(func){
        case 'r':
            model->output_activation_function = &relu;
            model->output_activation_derivative = &relu_derivative;
            break;
        case 'R':
            model->output_activation_function = &relu2;
            model->output_activation_derivative = &relu2_derivative;
            break;
        case 's':
            model->output_activation_function = &sigmoid;
            model->output_activation_derivative = &sigmoid_derivative;
            break;
        case 't':
            model->output_activation_function = &tanhf;
            model->output_activation_derivative = &tanhf_derivative;
            break;
        default:
            model->output_activation_function = &relu;
            model->output_activation_derivative = &relu_derivative;
            break;
    }
}

void set_hidden_actfunc(ANN *model, char func){
    switch(func){
        case 'r':
            model->hidden_activation_function = &relu;
            model->hidden_activation_derivative = &relu_derivative;
            break;
        case 'R':
            model->hidden_activation_function = &relu2;
            model->hidden_activation_derivative = &relu2_derivative;
            break;
        case 's':
            model->hidden_activation_function = &sigmoid;
            model->hidden_activation_derivative = &sigmoid_derivative;
            break;
        case 't':
            model->hidden_activation_function = &tanhf;
            model->hidden_activation_derivative = &tanhf_derivative;
            break;
        default:
            model->hidden_activation_function = &relu;
            model->hidden_activation_derivative = &relu_derivative;
            break;
    }
}
