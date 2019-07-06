/* EMBEDDEDML V1.3 */
/*
    util.h - Code generation for EmbeddedML 
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

#ifndef _UTIL_
#define _UTIL_

void clrscr();

void help_screen();

void print_path(int path);

int check_if_continue(int path);

int generate_weights(int argc, const char * argv[], int n_weights);

int generate_softmax(int argc, const char * argv[], int begin);

int generate_model(int argc, const char * argv[], int n_layers, int begin, int end, int model_ID, int n_bias, int n_weights);

#endif