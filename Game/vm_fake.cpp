#include "vm_fake.h"
#include "md5.h"
#include<string>
using namespace std;

FILE *file;
int *instructions;
int instruction_count = 0;
int instruction_space = 4;
bool running = true;
bool is_jmp = false;
char locals[9] = {0x46,0x43,0x53,0x52,0x4b,0x4d,0x4f,0x46,0x59};

void eval(int instr)
{
	is_jmp = false;
	switch (instr) 
	{
	case HLT: 
	{
		running = false;
		printf("Finished Execution\n");
		break;
	}
	case PSH:
	{
		SP = SP + 1;
		IP = IP + 1;
		stack[SP] = locals[instructions[IP]-1];
		break;
	}
	case POP: 
	{
		IP = IP + 1;
		locals[instructions[IP]-1]=stack[SP];
		SP = SP - 1;
		break;
	}
	default: 
	{
		printf("Unknown Instruction %d\n", instr);
		break;
	}
	}
}

void start()
{
	instructions = (int*)malloc(sizeof(int) * instruction_space);
	int num;
	int i = 0;
	while (fscanf(file, "%d", &num) > 0) 
	{
		instructions[i] = num;
		printf("%d\n", instructions[i]);
		i++;
		if (i >= instruction_space) {
			instruction_space *= 2;
			instructions = (int*)realloc(instructions, sizeof(int) * instruction_space); 
		}
	}
	instruction_count = i;

	fclose(file);

	SP = -1;

	while (running && IP < instruction_count) 
	{
		eval(FETCH);
		if (!is_jmp) {
			IP = IP + 1;
		}
	}
	

	free(instructions);
}




int run(const char* temp)
{

	instructions = (int *)malloc(sizeof(*instructions) * instruction_space);

	int num;
	int i = 0;
	while (sscanf(&temp[i], "%1d", &num) > 0) {
		instructions[i] = num;
		if (temp[i + 1] == 0)
			break;
		i++;
		if (i >= instruction_space) {
			instruction_space *= 2;
			instructions = (int*)realloc(instructions, sizeof(*instructions) * instruction_space); 
		}
	}

	instruction_count = i;

	SP = -1;

	while (running && IP < instruction_count) {
		eval(FETCH);
		if (!is_jmp) {
			IP = IP + 1;
		}
	}
	string result;
	MD5 md5(locals);
	result = md5.md5();
	if (result == "77be496aa517273f850f78f7425c76bb")
	{
		if(temp[47]-temp[45]==1 && temp[43]-temp[41]==1 &&temp[48]==0)
			printf("OK\nflag:SYC{%s}\n",temp);
	}
	free(instructions);

	return 0;
}
//020304050607151413120807060513141516171808091819
//FCSRKMOFY
//FROMOFSYC
