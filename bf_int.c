#include <stdio.h>
#include <stdlib.h>
#include "dynarray.h"

/*  Brainf*ck interpreter by cdm */

typedef unsigned char MemByte;
#define MEM_SIZE 3*1024 // 3KB

typedef enum{
    t_INCR_DP,
    t_DECR_DP,
    t_INCR,
    t_DECR,
    t_OUT,
    t_IN,
    t_JMP_OP,
    t_JMP_CLS,
    t_END
} OP;

const char* const OP_NAMES[] = {
    [t_INCR_DP] = "INCR_DP",
    [t_DECR_DP] = "DECR_DP",
    [t_INCR] = "INCR",
    [t_DECR] = "DECR",
    [t_OUT] = "OUT",
    [t_IN] = "IN",
    [t_JMP_OP] = "JMP_OP",
    [t_JMP_CLS] = "JMP_CLS",
    [t_END] = ""
};

DEFINE_DYNAMIC_ARRAY(OP, Ops);

void print_op(OP op){
    printf("%s", OP_NAMES[op]);
}
int lex_file(Ops_t* ops, char* filename){
    if (!ops || !filename) return 1;
    FILE* file = fopen(filename, "rb");
    if (!file){
        fprintf(stderr, "[ERROR] Could not open the provided file!\n");
        return 1;
    }

    char c;
    while ((c = fgetc(file)) != EOF){
        switch (c){
            case '>':{
                Ops_append(ops, t_INCR_DP);
            }break;
            case '<':{
                Ops_append(ops, t_DECR_DP);
            } break;
            case '+':{
                Ops_append(ops, t_INCR);
            }break;
            case '-':{
                Ops_append(ops, t_DECR);
            } break;
            case '.':{
                Ops_append(ops, t_OUT);
            } break;
            case ',':{
                Ops_append(ops, t_IN);
            } break;
            case '[':{
                Ops_append(ops, t_JMP_OP);
            } break;
            case ']':{
                Ops_append(ops, t_JMP_CLS);
            } break;
        }
    }
    Ops_append(ops, t_END);
    fclose(file);
    return 0;
}

int emulate_program(Ops_t* ops, MemByte* mem){
    if (!ops || ops->size == 0 || !mem){
        fprintf(stderr, "[ERROR] Emulate: Invalid input!\n");
        return 1;
    }
    MemByte* data_ptr = mem;
    MemByte* data_max = mem;
    OP* op_ptr = ops->items;
    OP* op_base = op_ptr;
    OP curr_op;
    while ((curr_op = *op_ptr) != t_END){
        switch (curr_op){
            case t_INCR_DP:{
                data_ptr += 1;
                if (data_ptr > data_max) data_max = data_ptr;
                if (data_ptr-mem >= MEM_SIZE){
                    fprintf(stderr, "[ERROR] Memory Pointer out of bounds!\n");
                    return 1;
                }
            }break;
            case t_DECR_DP:{
                data_ptr -= 1;
                if (data_ptr-mem < 0){
                    fprintf(stderr, "[ERROR] Memory Pointer out of bounds!\n");
                    return 1;
                }
            } break;
            case t_INCR:{
                *data_ptr = (*data_ptr) +1;
            }break;
            case t_DECR:{
                *data_ptr -= 1;
            } break;
            case t_OUT:{
                putchar(*data_ptr);
            } break;
            case t_IN:{
                *data_ptr = fgetc(stdin);
            } break;
            case t_JMP_OP:{
                if (*data_ptr == 0){
                    int nested = 0;
                    OP op;
                    op_ptr++;
                    while (true){
                        op = *op_ptr;
                        if (op == t_END){
                            fprintf(stderr, "[ERROR] Out of bounds! Unclosed bracket found!\n");
                            return 1;
                        }
                        if (op == t_JMP_CLS){
                            if (nested == 0){
                                break;
                            }
                            nested--;
                        }
                        else if (op == t_JMP_OP){
                            nested ++;
                        }
                        else{
                        }
                        op_ptr++;
                    }
                }
            } break;
            case t_JMP_CLS:{
                if (*data_ptr != 0){
                    int nested = 0;
                    OP op;
                    op_ptr--;
                    while (true){
                        if ((int) (op_ptr-op_base) < 0){
                            fprintf(stderr, "[ERROR] Out of bounds!\n");
                            return 1;
                        }
                        op = *op_ptr;
                        if (op == t_JMP_OP){
                            if (nested == 0){
                                break;
                            }
                            nested --;
                        }
                        else if (op == t_JMP_CLS){
                            nested ++;
                        }
                        op_ptr--;
                    }
                }
            } break;
        }
        op_ptr++;
    }
}

int main(int argc, char** argv){

    if (argc != 2){
        fprintf(stderr, "[ERROR] Wrong argument count!\n");
        printf("[INFO] How to use:\n  > bf <brainf*ck-file>\n");
        return 1;
    }
    MemByte* memory = (MemByte*) calloc(MEM_SIZE, sizeof(*memory));
    if (!memory){
        fprintf(stderr, "[ERROR] Failed to allocate memory for the memory array!\n");
        return 1;
    }
    Ops_t* ops = (Ops_t*) Ops_create(32, NULL);
    if (!ops){
        fprintf(stderr, "[ERROR] Failed to allocate memory for the instruction array!\n");
        free(memory);
        return 1;
    }
    if (lex_file(ops, argv[1]) == 1) return 1;
    emulate_program(ops, memory);
    
    free(memory);
    free(ops);

    return 0;
}