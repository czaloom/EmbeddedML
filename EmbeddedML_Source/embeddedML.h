/* EMBEDDEDML V1.2 */
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
    along with EmbeddedML.  If not, see <https://www.gnu.org/licenses/>
*/

#ifndef EMBEDDED_ML
#define EMBEDDED_ML

//#ifndef _EMBEDDED_
//	#define _EMBEDDED_
//#endif

/*
 * NOTE: Defining _EMBEDDED_ removes all dependencies on standard libraries. 
 *       This means that some functions may not be available, but it will allow 
 *       usage on any system that runs C/C++.
 */


#ifndef _EMBEDDED_
    #include <stdlib.h>
    #include <math.h>
    #include <stdio.h>
#endif

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
    float alpha;    //Momentum Coefficient
} ANN;

void train_ann(ANN *net, float *input, float *output);
void run_ann(ANN *net, float *input);
void init_ann(ANN *net);
void init_embedded_ann(ANN *net);
#ifndef _EMBEDDED_
    void load_ann(ANN *net, const char *filename);   
    void save_ann(ANN *net, const char *filename);
#endif

//-----Utility-----
void fill_zeros(float *v, unsigned int size);
void fill_number(float *v, unsigned int size, float number);
void strong_softmax(float *x, float *y);
void weak_softmax(float *x, float *y);
#ifndef _EMBEDDED_
    void fill_rand(float *v, unsigned int size, float lower, float upper);
    void softmax(unsigned int size, float multiplier, float *x, float *y);
    void softmax2(unsigned int size, float multiplier, float *x, float *y);
#endif

//------Activation Functions-----
float relu(float x);
float relu_derivative(float x);

float relu2(float x);
float relu2_derivative(float x);

#ifndef _EMBEDDED_
    //float tanhf(float x); //USE 'math.h' tanhf(x) function
    float tanhf_derivative(float x);

    float sigmoid(float x);
    float sigmoid_derivative(float x);
#endif
    
#endif
