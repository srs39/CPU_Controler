/*
Names: Jasvir Dosanjh,  Samantha Shoecraft
Version: 2/7/2017
*/
#define CPU_H
#define NO_OF_REGISTERS 8
#define NO_OF_BITS 16
#define END_OPP 3
#define END_RD 6
#define END_RS 9
#define FETCH 0
#define DECODE 1
#define EVAL_ADDR 2
#define FETCH_OP 3
#define EXECUTE 4
#define STORE 5


#include <stdlib.h>
#include <stdio.h>

typedef unsigned short Register;
typedef unsigned char Bit;

Register reg_file[NO_OF_REGISTERS];

typedef struct alu_s {
    // registers a, b, and r
    Register a;
    Register b;
    Register r;
} ALU_s;

typedef struct cpu_s {
    Register reg_file[NO_OF_REGISTERS]; // register file
    Register ir;// ir
    Register sext;// sext
    short int PC;
} CPU_s;    // the _s designates the type as a structure

typedef struct cpu_s* CPU_p;

int controller (CPU_p cpu);