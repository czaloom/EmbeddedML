/* 
 * EMBEDDEDML v1.3b
 */

/*
    embeddedML.h - Embedded Machine Learning Library
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
    along with EmbeddedML. If not, see <https://www.gnu.org/licenses/>
*/

#ifndef EMBEDDED_ML_METAL
#define EMBEDDED_ML_METAL

//-----ANN Structure-----
typedef struct {
    float *weights;
    float *dedw;
    float *bias;
    unsigned int *topology;
    unsigned int n_layers;
    unsigned int n_weights;
    unsigned int n_bias;
    float *output;

    float (*output_activation_function)(float);
    float (*output_activation_derivative)(float);
    float (*hidden_activation_function)(float);
    float (*hidden_activation_derivative)(float);
    
    float eta;      //Learning Rate
    float beta;     //Bias Learning Rate
    float alpha;    //Momentum Coefficient
} ANN;

void train_ann(ANN *net, float *input, float *output);
void run_ann(ANN *net, float *input);

void init_ann(ANN *net);
void init_pretrained_ann(ANN *net);

//-----Utility-----
void fill_zeros(float *v, unsigned int size);
void fill_number(float *v, unsigned int size, float number);

//------Activation Functions-----
float relu(float x);
float relu_derivative(float x);

float relu2(float x);
float relu2_derivative(float x);
    
#endif
