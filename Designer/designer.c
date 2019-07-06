/* EMBEDDEDML V1.3 */
/*
    designer.c - Code generation for EmbeddedML  
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

#include "util.h"

int main(int argc, const char * argv[]) {
	unsigned int model_ID = 0;

	unsigned int n_layers = 0;
	unsigned int n_weights = 0, n_bias = 0;

	unsigned int begin,end;

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
		    const char *a[4] = {"Model Generator", "Weight Generator", "Strong_Softmax Generator"};
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
		    		generate_model(argc,argv,n_layers,begin,end,model_ID,n_bias,n_weights);
		    		break;
	    		case '1':
	    			generate_weights(argc, argv, n_weights);
	    			break;
    			case '2':
    				generate_softmax(argc, argv, begin);
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