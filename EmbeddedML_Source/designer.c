/* EMBEDDEDML V1.2.1 */
/*
    designer.c - Tools for quicker development in EmbeddedML 
    Copyright (C) 2018 Charles Zaloom

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "embeddedML.h"

unsigned int model_ID = 0;

unsigned int n_layers = 0;
unsigned int n_weights = 0, n_bias = 0;

unsigned int begin,end;

void clrscr(){
	system("clear||cls");
}

void help_screen(){
	float version = 1.0;
	clrscr();
	printf("Model Designer - V%f\n\n", version);
	printf("Usage: \n$ ./my_prgram 2 2 1 \n");
	printf("2 2 1 Represents the topology of the network.\n");
	printf("In this example the ANN has an input of 2 neurons, \na hidden layer of 2 neurons, and output layer made of 1 neuron.\n");
	printf("\n Note: Some models require certain specifications to be met for generation.\n");
	printf("\nPress ENTER to exit.");
	char input[5];
	fgets(input,5,stdin);
	clrscr();
	exit(1);
}

void print_path(int path){
	switch(path){
		case 0: 
			printf("Models\n");
			break;
		case 1: 
			printf("Models->ANN\n");
			break;
		default:
			printf("Unknown Path\n");
			break;
	}
}

int check_if_continue(int path){
	print_path(path);
	printf("Continue/Go back/Quit (Y/ENTER/Q):\nInput: ");
	char input[5];
    fgets(input,5,stdin);
    switch(input[0]){
    	case 'Y':
    	case 'y':
    		clrscr();
    		return 1;
    		break;
    	case 'Q':
    	case 'q':
    		clrscr();
    		exit(1);
    		break;
		case 'N':
		case 'n': 
		default:
			clrscr();
			return 0;
			break;
    }
    return 0;
}

int generate_weights(int argc, const char * argv[]){
	clrscr();
    unsigned i;

    float W[n_weights];
    fill_rand(W, n_weights, 0.2, 0.8);

    printf("-----GENERATED WEIGHTS-----\n");
    printf("Note: Generating weights is only necessary when _EMBEDDED is defined.\n\n");
    printf("float weights[%d] = {", n_weights);
    int k;
    for(k = 0; k < n_weights; k++){
    	printf("%f", W[k]);
    	if(k+1 < n_weights) printf(",");
    }
    printf("};\n\n");
    return 1;
}

int generate_softmax(int argc, const char * argv[]){
	clrscr();
    int size = atoi(argv[begin+1]);

    printf("NOTE: The softmax function generator uses the first number used in calling the designer program as the input size.\n\n");

    printf("float strong_softmax(float *x, float *y){\n");
    printf("  //For use with systems that have low-precision floating point number storage.\n");
    printf("  NOTE: If the input values are within -1.0 to 1.0 change the multiplier to a multiple of 10.\n\n");
    printf("  float multiplier = 1.0;\n");
    printf("  unsigned int size = %d;\n\n", size);
    
    int i,k;
    for(i = 0; i < size; i++){
    	printf("  y[%d] = (x[%d]*x[%d]*multiplier) / (",i,i,i);
    	for(k = 0; k < size; k++){
    		printf("(x[%d]*x[%d]*multiplier)",k,k);
    		if(k+1 < size) printf(" + ");
    	}
    	printf(");\n");
    }
    printf("\n");

    printf("  int i;\n");
    printf("  for(i = 0; i < size; i++){\n");
    printf("    if(x[i] < 0.0) y[i] = y[i] * -1.0;\n");
    printf("  }\n");


    printf("}\n\n");
    return 1;
}

int generate_model(int argc, const char * argv[]){
	clrscr();
    unsigned i;
    float W[n_weights];
    fill_rand(W, n_weights, 0.2, 0.8);

    int loop = 1;
    while(loop == 1){
    	clrscr();
		int i;
	    const char *a[3] = {"New","New Embedded","Load"};
	    printf("Options:\n");
	    for(i = 0; i < 3; i++){
	    	printf("(%d) %s\n",i,a[i]);
	    }
	    printf("Input: ");
	    char input[5];
	    fgets(input,5,stdin);
	    clrscr();
	    switch(input[0]){
	    	case '0':
	    		if(model_ID != 0){
		    		printf("//--- ANN ID:%d ---\n",model_ID);
					printf("float weights%d[%d];\n",model_ID,n_weights);
					printf("float dedw%d[%d];\n",model_ID,n_weights);
					printf("float bias%d[%d];\n",model_ID,n_bias);
					printf("unsigned int network_topology%d[%d] = {",model_ID, n_layers);
					for(i = 0; i < n_layers; i++){
					    printf("%s", argv[i+1+begin]);
					    if(i+1 != n_layers) printf(",");
					}
					printf("};\n");
					printf("float output%d[%s];\n",model_ID,argv[n_layers+begin]);

					printf("\nANN net%d;\n", model_ID);
					printf("net%d.weights = weights%d;\n", model_ID, model_ID);
					printf("net%d.dedw = dedw%d;\n", model_ID, model_ID);
					printf("net%d.bias = bias%d;\n", model_ID, model_ID);
					printf("net%d.topology = network_topology%d;\n", model_ID, model_ID);
					printf("net%d.n_layers = %d;\n", model_ID, n_layers);
					printf("net%d.n_weights = %d;\n", model_ID, n_weights);
					printf("net%d.n_bias = %d;\n", model_ID, n_bias);
					printf("net%d.output = output%d;\n\n", model_ID, model_ID);

					printf("//OPTIONS\n");
					printf("net%d.eta = 0.25;     //Learning Rate\n", model_ID);
					printf("net%d.alpha = 0.15;   //Momentum Coefficient\n", model_ID);
					printf("net%d.output_activation_function = &relu;\n", model_ID);
					printf("net%d.hidden_activation_function = &relu;\n", model_ID);

					printf("\ninit_ann(&net%d);\n", model_ID);
					printf("float x%d[%s];\n", model_ID, argv[begin+1]);
					printf("float y%d[%s];\n", model_ID, argv[n_layers+begin]);
					printf("//---------------------\n");
				}
				else{
					printf("//---ANN---\n");
					printf("float weights[%d];\n", n_weights);
					printf("float dedw[%d];\n", n_weights);
					printf("float bias[%d];\n", n_bias);
					printf("unsigned int network_topology[%d] = {", n_layers);
					for(i = 0; i < n_layers; i++){
					    printf("%s", argv[i+1+begin]);
					    if(i+1 != n_layers) printf(",");
					}
					printf("};\n");
					printf("float output[%s];\n", argv[n_layers+begin]);

					printf("\nANN net;\n");
					printf("net.weights = weights;\n");
					printf("net.dedw = dedw;\n");
					printf("net.bias = bias;\n");
					printf("net.topology = network_topology;\n");
					printf("net.n_layers = %d;\n", n_layers);
					printf("net.n_weights = %d;\n", n_weights);
					printf("net.n_bias = %d;\n", n_bias);
					printf("net.output = output;\n\n");

					printf("//OPTIONS\n");
					printf("net.eta = 0.25;     //Learning Rate\n");
					printf("net.alpha = 0.15;   //Momentum Coefficient\n");
					printf("net.output_activation_function = &relu;\n");
					printf("net.hidden_activation_function = &relu;\n");

					printf("\ninit_ann(&net);\n");
					printf("float x[%s];\n", argv[begin+1]);
					printf("float y[%s];\n", argv[n_layers+begin]);
					printf("//---------------------\n");
				}
	    		break;
    		case '1':
    			if(model_ID != 0){
    				printf("//--- EMBEDDED ANN ID:%d ---\n", model_ID);
					printf("float weights%d[%d] = {", model_ID, n_weights);
					int k;
				    for(k = 0; k < n_weights; k++){
				    	printf("%f", W[k]);
				    	if(k+1 < n_weights) printf(",");
				    }
				    printf("};\n");
					printf("float dedw%d[%d];\n",model_ID, n_weights);
					printf("float bias%d[%d];\n",model_ID, n_bias);
					printf("unsigned int network_topology%d[%d] = {",model_ID, n_layers);
					for(i = 0; i < n_layers; i++){
					    printf("%s", argv[i+1+begin]);
					    if(i+1 != n_layers) printf(",");
					}
					printf("};\n");
					printf("float output%d[%s];\n",model_ID, argv[n_layers+begin]);

					printf("\nANN net%d;\n",model_ID);
					printf("net%d.weights = weights%d;\n",model_ID,model_ID);
					printf("net%d.dedw = dedw%d;\n",model_ID,model_ID);
					printf("net%d.bias = bias%d;\n",model_ID,model_ID);
					printf("net%d.topology = network_topology%d;\n",model_ID,model_ID);
					printf("net%d.n_layers = %d;\n",model_ID, n_layers);
					printf("net%d.n_weights = %d;\n",model_ID, n_weights);
					printf("net%d.n_bias = %d;\n",model_ID, n_bias);
					printf("net%d.output = output%d;\n\n",model_ID,model_ID);

					printf("//OPTIONS\n");
					printf("net%d.eta = 0.25;     //Learning Rate\n",model_ID);
					printf("net%d.alpha = 0.15;   //Momentum Coefficient\n",model_ID);
					printf("net%d.output_activation_function = &relu;\n",model_ID);
					printf("net%d.hidden_activation_function = &relu;\n",model_ID);

					printf("\ninit_embedded_ann(&net%d);\n",model_ID);
					printf("float x%d[%s];\n",model_ID, argv[begin+1]);
					printf("float y%d[%s];\n",model_ID, argv[n_layers+begin]);
					printf("//---------------------\n");
		    		break;
    			}
    			else{
	   	    		printf("//---EMBEDDED ANN---\n");
					printf("float weights[%d] = {", n_weights);
					int k;
				    for(k = 0; k < n_weights; k++){
				    	printf("%f", W[k]);
				    	if(k+1 < n_weights) printf(",");
				    }
				    printf("};\n");
					printf("float dedw[%d];\n", n_weights);
					printf("float bias[%d];\n", n_bias);
					printf("unsigned int network_topology[%d] = {", n_layers);
					for(i = 0; i < n_layers; i++){
					    printf("%s", argv[i+1+begin]);
					    if(i+1 != n_layers) printf(",");
					}
					printf("};\n");
					printf("float output[%s];\n", argv[n_layers+begin]);

					printf("\nANN net;\n");
					printf("net.weights = weights;\n");
					printf("net.dedw = dedw;\n");
					printf("net.bias = bias;\n");
					printf("net.topology = network_topology;\n");
					printf("net.n_layers = %d;\n", n_layers);
					printf("net.n_weights = %d;\n", n_weights);
					printf("net.n_bias = %d;\n", n_bias);
					printf("net.output = output;\n\n");

					printf("//OPTIONS\n");
					printf("net.eta = 0.25;     //Learning Rate\n");
					printf("net.alpha = 0.15;   //Momentum Coefficient\n");
					printf("net.output_activation_function = &relu;\n");
					printf("net.hidden_activation_function = &relu;\n");

					printf("\ninit_embedded_ann(&net);\n");
					printf("float x[%s];\n", argv[begin+1]);
					printf("float y[%s];\n", argv[n_layers+begin]);
					printf("//---------------------\n");
		    		break;
	    		}
			case '2':
				printf("//---LOAD ANN FROM FILE---\n");
			    printf("float weights[%d];\n", n_weights);
			    printf("float dedw[%d];\n", n_weights);
			    printf("float bias[%d];\n", n_bias);
			    printf("unsigned int topology[%d];\n", n_layers);
			    printf("float output[%s];\n", argv[n_layers+begin]);

			    printf("\nANN net;\n");
			    printf("net.weights = weights;\n");
			    printf("net.dedw = dedw;\n");
			    printf("net.bias = bias;\n");
			    printf("net.topology = topology;\n");
			    printf("net.output = output;\n");
			    printf("load_ann(&net, <Filename>);\n");
			    printf("float x[%d];\n", atoi(argv[begin+1]));
			    printf("float y[%s];\n", argv[n_layers+begin]);
			    printf("//-------------\n");
	    		break;
			default:
				printf("UNKOWN INPUT\n");
				break;
	    }
	    printf("\n");
	 	loop = check_if_continue(1);
  	}
    return 1;
}

int main(int argc, const char * argv[]) {
	printf("%d\n", argc);
	if(argc == 1) help_screen();

	else{
		unsigned i;
	    n_layers = argc - 1;
	    n_weights = 0; 
	    n_bias = 0;
	    int id_state = 0;
	    for(i = 0; i < n_layers; i++){
	    	if(strcmp(argv[i+1], "-ID") == 0){
	    		//if(i + 2 == n_layers - 1) 
	   
	    		model_ID = atoi(argv[i+2]); 
	    		if(i == 0){
	    			begin = i+2;
	    			end = n_layers;
	    		}
	    		else{
	    			begin = 0;
	    			end = n_layers-2;
	    		}
	    		i+=2;

	    		id_state = 1;
	    	}
	    	else if(i > 0){
	        	n_weights += atoi(argv[i+1])*atoi(argv[i]);
	        	n_bias += atoi(argv[i+1]);
	    	}
	    }
	    if(id_state) n_layers -= 2;

		clrscr();
		int loop = 1;
		while(loop == 1){
			int i;
		    const char *a[4] = {"Create/Load ANN", "Weight Generator", "Strong_Softmax Generator"};
		    printf("Tools:\n");

		    int tools = 3;
		    for(i = 0; i < tools; i++){
		    	printf("(%d) %s\n",i,a[i]);
		    }

		    printf("Input: ");
		    char input[5];
		    fgets(input,5,stdin);
		    switch(input[0]){
		    	case '0':
		    		generate_model(argc,argv);
		    		break;
	    		case '1':
	    			generate_weights(argc, argv);
	    			break;
    			case '2':
    				generate_softmax(argc, argv);
    				break;
				default:
					printf("UNKOWN INPUT\n");
					break;
		    }
		 	loop = check_if_continue(0);
	  	}
  	}
    return 0;
}
