#include "intercode.h"

typedef struct instruction_ instruction;
typedef struct reg_ Reg;
typedef struct var_ Var;
typedef struct Operandlist_ Operandlist;
struct instruction_{
    enum{
        ITYPE,RTYPE,JTYPE,
    }kind;
};

struct reg_{
    //char* name;
    int free; //0 free 1 alloc
    int offset; //相对于$fp的偏移量 
    int var_num; //所分配的变量名
};
struct var_{
    int reg;
    int offset;
    int nextuse;
    int lastuse;
};
struct Operandlist_{
    Operand op;
    Operandlist* next;
};
void Varnum();
void assemble(char* filename);
int findnum(Operand op);
int GetReg(int num);
void Ir_to_s(char* filename);