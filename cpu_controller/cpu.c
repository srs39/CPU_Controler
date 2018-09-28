/*
Names: Jasvir Dosanjh,  Samantha Shoecraft
Version: 2/7/2017
*/
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"


unsigned short memory[32]; 



int main(int argc, char *argv[]){
    struct cpu_s cpu1;//creating cpu
    CPU_p cpu = &cpu1;//creating pointer to cpu
    cpu->PC =0;//setting PC to 0
    if (argc < 2){//checking to make sure enough arguments passed through
       printf("Not enough arguements.");
    } else if (argc >=2 ){
       sscanf(argv[1], "%hx", &memory[cpu->PC]);//adding command line input to first memory line

    }
    //putting data in both cpu reg and memory
    cpu -> reg_file[0] = 14;
    for (int i =0; i <8; i ++){
        cpu->reg_file[i] = 32 + (i* 10);
 
    }

    //call to controller
    controller(cpu);

}

/* 
 * This method simulates the state machine controller. In this method, we can execute simple programs using the FSM as a loop using switch statements.
 * Parameters: CPU_p cpu is the actual cpu that is passed in to this method in main.
 * Return: This method returns an interger. An integer (-1) is only returned in the case that the pointer passed to the method is null.
 */
int controller (CPU_p cpu) {
    // check to make sure both pointers are not NULL
     Bit a, b;
     struct alu_s alu;
     if (cpu == NULL){
        printf("oops");
        return -1;  //if the pointer is NULL, return -1 and exit the function before going into all the switch statements.
     }
    // do any initializations here

    unsigned int opcode, Rd, Rs, state, immed7;

    int lsb, msb;
    state = FETCH;
    for (;;) {   // efficient endless loop
        switch (state) {
            case FETCH: //gets IR from memory slot pc is pointing to
                printf("Here in FETCH\n");
                // get memory[PC] into IR - memory is a global array
                cpu->ir = memory[cpu->PC];
                printf("Getting ir from memory");
                 //increment PC unless cpu is > memory, then will go to zero
                if (cpu->PC < 31){
                   cpu->PC++;
                } else {
                   cpu->PC = 0;
                }
                printf("Contents of IR = %04X\n", cpu->ir);
                // put printf statements in each state to see that it is working
                state = DECODE;
                break;
            case DECODE:
                printf("%hx" , cpu->ir);
                // get the fields out of the IR
                    //operation bits
                printf("I'm in decode\n");
                lsb = 13;
                msb = 15;
	        opcode = (cpu->ir >> lsb) & ~(~0 << (msb-lsb+1));
                printf("opcode = %d\n", opcode);
	        //rd location
	        lsb =10;
	        msb = 12;
	        Rd= (cpu->ir >> lsb) & ~(~0 << (msb-lsb+1));
                printf("Rd is R%d\n", Rd);
	        //rs location
	        lsb=7;
	        msb=9;
	        Rs= (cpu->ir>> lsb) & ~(~0 << (msb-lsb+1));
                printf("Rs is R%d\n", Rs);
	        //sext
	        lsb =0;
	        msb =6;
	        immed7 = (cpu->ir >> lsb) & ~(~0 << (msb-lsb+1));
                printf("sext = %hX\n", immed7);
                // make sure opcode is in integer form
                state = EVAL_ADDR;
                break;
            case EVAL_ADDR:
                printf("I'm evaluating the address\n");
                switch (opcode) {
                // different opcodes require different handling
		// compute effective address, e.g. add sext(immed7) to register
                   case 3://LD
		      //Rd = cpu->PC + immed7;
                      break;
		   case 4://LDI
                      //Rd = Rs + immed7;
                      break;
                   case 5://ST
                      //memory[Rd +immed7] = Rs;
                      break;
                   case 6://BRZ
                       cpu->PC += immed7;
                       break;
                   default:
                       break;

                }
                state = FETCH_OP;
                break;
            case FETCH_OP:
                printf("Fetching opperands\n");
                    // get operands out of registers into A, B of ALU
                    // or get memory for load instr.
                switch (opcode) {
                    case 0://Add - RD RS
                         printf("ADD- alu a is %hd and b is %hd\n", reg_file[Rd], reg_file[Rs]); 
                         alu.a = cpu->reg_file[Rd];
                         alu.b = cpu->reg_file[Rs];
                         break;
                    case 1://Adi - Rs immed7
                         printf("ADI- alu a is %hd and b is %hd\n", reg_file[Rs], immed7);
                         alu.a = cpu->reg_file[Rs];
                         alu.b = immed7;
                         break;
                    case 2://nand - Rd Rs
                        printf("NAND- alu a is %hd and b is %hd\n", reg_file[Rd], reg_file[Rs]);
                        alu.a = cpu->reg_file[Rd];
                        alu.b = cpu->reg_file[Rs];
                        break;
                     default:
                        break;

                }
                state = EXECUTE;
                break;
            case EXECUTE:
                printf("Executing the function\n");
                switch (opcode) {
                    case 0://adds alu a and b
                        alu.r = alu.a + alu.b;
                        printf("ADD = %hd\n", alu.r);
                        break;
                    case 1://adds alu a and b
                        alu.r = alu.a + alu.b;
                        printf("ADI = %hd\n", alu.r);
                        break;
                    case 2:
                        //ands alu a and be then nots
                        for (int i = 0; i <NO_OF_BITS; i++){
                            a = alu.a & (1<<i);
                            b = alu.b & (1<<i);
                            alu.r |= ~(a & b) << i;
                        }
                        printf("NAND = %hd\n", alu.r);
                        break;
                     default:
                        break;


                }
                state = STORE;
                break;
            case STORE:
                printf("Storing\n");
                switch (opcode) {
                    // write back to register or store MDR into memory
                    case 3://ldi
                        reg_file[Rd] = immed7;
                        break;
                    case 4://ld
                        reg_file[Rd] = memory[Rs + immed7];
                    case 5://st
                      memory[Rd+ immed7] = Rs;
                      break;
                    case 6://brz
                       break;
                    case 7://hault
                       return 1;
					   break;
                    default://used for add, adi and nand
                      reg_file[Rd] = alu.r;
                      break;
                   
 
                }
                // do any clean up here in prep for the next complete cycle
                state = FETCH;
                break;
        }
		if (state == STORE)
           return 1;
             
    }
}
