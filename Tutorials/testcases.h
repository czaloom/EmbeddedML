#ifndef TESTCASES_h
#define TESTCASES_h

#include <stdlib.h>
#include <math.h>

int rand_val(int min_rand, int max_rand)
{
    int rand_value;
    int max_value;
    max_rand++;
    max_value = RAND_MAX - (RAND_MAX % max_rand);
    while((rand_value = rand()) >= max_value);
    return (rand_value%max_rand) + min_rand;
}

void generate_xor(float *x, float *y){
    switch(rand_val(0,3)){
        case 0:
            x[0] = 0.0;
            x[1] = 0.0;
            y[0] = 0.0;
            break;
        case 1:
            x[0] = 1.0;
            x[1] = 0.0;
            y[0] = 1.0;
            break;
        case 2:
            x[0] = 0.0;
            x[1] = 1.0;
            y[0] = 1.0;
            break;
        case 3:
            x[0] = 1.0;
            x[1] = 1.0;
            y[0] = 0.0;            
            break;
        default:
            break;
    }
}

void generate_xorand(float *x, float *y){
    int i = rand_val(0,7);
    switch(i%8){
	case 0:
            x[0] = 0.0;
            x[1] = 0.0;
            x[2] = 1.0;
            y[0] = 0.0;
	        y[1] = 0.0;
            break;
        case 1:
            x[0] = 0.0;
            x[1] = 1.0;
            x[2] = 1.0;
            y[0] = 1.0;
            y[1] = 1.0;
            break;
        case 2:
            x[0] = 1.0;
            x[1] = 0.0;
            x[2] = 1.0;
            y[0] = 1.0;
            y[1] = 0.0;
            break;
        case 3:
            x[0] = 1.0;
            x[1] = 1.0;
            x[2] = 1.0;
            y[0] = 0.0;
            y[1] = 1.0;
            break;
        case 4:
            x[0] = 0.0;
            x[1] = 0.0;
            x[2] = 0.0;
            y[0] = 0.0;
            y[1] = 0.0;
            break;
        case 5:
            x[0] = 0.0;
            x[1] = 1.0;
            x[2] = 0.0;
            y[0] = 1.0;
            y[1] = 0.0;
            break;
        case 6:
            x[0] = 1.0;
            x[1] = 0.0;
	        x[2] = 0.0;
            y[0] = 1.0;
            y[1] = 0.0;
            break;
        case 7:
            x[0] = 1.0;
            x[1] = 1.0;
	        x[2] = 0.0;
            y[0] = 0.0;
            y[1] = 0.0;		   
	    break;
        default:
            break;
    }
}

void generate_sine(float *x, float *y){
    x[0] = (float)(rand_val(0,100))/100.0;
    y[0] = sin(x[0]);
}

void generate_accel_orientation(float *x, float *y){
    int i = rand_val(0,5);
    switch(i){
        case 0: 
            y[0] = 1.0;     //F
            y[1] = 0.0;     //R
            y[2] = -1.0;    //B
            y[3] = 0.0;     //L
            y[4] = 0.0;     //U
            y[5] = 0.0;     //D
            x[0] = 1.0;
            x[1] = 0.0;
            x[2] = 0.0;
            return;
        case 1: 
            y[0] = -1.0;    //F
            y[1] = 0.0;     //R
            y[2] = 1.0;     //B
            y[3] = 0.0;     //L
            y[4] = 0.0;     //U
            y[5] = 0.0;     //D
            x[0] = -1.0;
            x[1] = 0.0;
            x[2] = 0.0;
            return;
        case 2: 
            y[0] = 0.0;     //F
            y[1] = 1.0;     //R
            y[2] = 0.0;     //B
            y[3] = -1.0;    //L
            y[4] = 0.0;     //U
            y[5] = 0.0;     //D
            x[0] = 0.0;
            x[1] = -1.0;
            x[2] = 0.0;
            return;
        case 3: 
            y[0] = 0.0;     //F
            y[1] = -1.0;    //R
            y[2] = 0.0;     //B
            y[3] = 1.0;     //L
            y[4] = 0.0;     //U
            y[5] = 0.0;     //D
            x[0] = 0.0;
            x[1] = 1.0;
            x[2] = 0.0;
            return;
        case 4: 
            y[0] = 0.0;     //F
            y[1] = 0.0;     //R
            y[2] = 0.0;     //B
            y[3] = 0.0;     //L
            y[4] = 1.0;     //U
            y[5] = -1.0;    //D
            x[0] = 0.0;
            x[1] = 0.0;
            x[2] = 1.0;
            return;
        case 5: 
            y[0] = 0.0;     //F
            y[1] = 0.0;     //R
            y[2] = 0.0;     //B
            y[3] = 0.0;     //L
            y[4] = -1.0;    //U
            y[5] = 1.0;     //D
            x[0] = 0.0;
            x[1] = 0.0;
            x[2] = -1.0;
            return;
        default:
            return;
    }
}

#endif










