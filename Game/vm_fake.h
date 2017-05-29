#pragma once
/*
A more advanced version of the VM
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define STACK_SIZE 256
static int stack[STACK_SIZE];

typedef enum {
	PSH, 
	POP, 
	HLT, 
} Instructions;

typedef enum {
	A, B, C, D, E, F, I, J, 
	EX,                     
	EXA,                   
	IP,                  
	SP,               
	REGISTER_SIZE
} Registers;

static int registers[REGISTER_SIZE];


#define SP (registers[SP])
#define IP (registers[IP])

#define FETCH (instructions[IP])

void eval(int instr);
int run(const char* temp);

void start();