#include "intercode.h"
#define TODO assert(0);
#define INTERCODE
#ifdef INTERCODE
#define idebug(...) printf(__VA_ARGS__)
#else
#define idebug(...) assert(1)
#endif
InterCodes intercodeshead;// 中间代码链表头
InterCodes intercodestail;// 链表尾部
int tmpcnt = 0;//临时变量计数器
int labelcnt = 0;
//NewTmp依次生成新的临时变量名
Operand NewTmp(){
    tmpcnt++;
    int len = 1,lentmp=tmpcnt;
    while(lentmp/=10)len++;
    char *name = (char*)malloc(sizeof(char)*(len+1));
    Operand res = (Operand)malloc(sizeof(Operand_d));
    name[0]='t';
    lentmp=tmpcnt;
    int pos = 0;
    while(lentmp){
        name[len-pos]='0'+lentmp%10;
        lentmp/=10;
        pos++;
    }
    res->kind = TMPVAR_OP;
    res->u.name = name;
    return res;
}
//NewLabel依次生成新的临时变量名
Operand NewLabel(){
    labelcnt++;
    int len = 1,lentmp=labelcnt;
    while(lentmp/=10)len++;
    char *name = (char*)malloc(sizeof(char)*(len+1));
    Operand res = (Operand)malloc(sizeof(Operand_d));
    strcpy(name,"label");
    lentmp=labelcnt;
    int pos = 0;
    while(lentmp){
        name[len-pos+4]='0'+lentmp%10;
        lentmp/=10;
        pos++;
    }
    res->kind = TMPVAR_OP;
    res->u.name = name;
    return res;
}
void InterCodes_init(){
    intercodeshead = NULL;
    intercodestail = NULL;
}

//InterCodes链表尾部插入新的一条Intercode
void NewInterCode(int codekind,Operand op0,Operand op1,Operand op2){//生成一行中间代码
    InterCode res = (InterCode) malloc(sizeof(struct InterCode_));
    res->kind = codekind;
    if(op0!=NULL)res->op[0] = op0;
    if(op1!=NULL)res->op[1] = op1;
    if(op2!=NULL)res->op[2] = op2;
    InterCodes code = (InterCodes) malloc(sizeof(struct InterCodes_));
    code->code = res;
    code->next = NULL;
    if(intercodeshead == NULL){
        code->prev = NULL;
        intercodeshead = code;
        intercodestail = intercodeshead;
    }
    else{
        intercodestail->next = code;
        code->prev = intercodestail;
        intercodestail = code; 
    }
}

//模块主函数：调用中间代码生成器并打印到文件
FILE* f;
void PrintInterCode(char *filename){
    idebug("----In PrintInterCode----\n");
    f = fopen(filename, "w");
    if (!f){
        printf("cannot open file : %s\n",filename);
        return;
    }
    Translate_Program(root);
    idebug("----Finish Translate program----\n");
    idebug("----Start Print Intercode----\n");
    InterCodes p = NULL;
    if(intercodeshead != NULL){p = intercodeshead;}//遍历链表的结构体指针
    while(p!=NULL){
        InterCode i = p->code; //一条中间代码的指针
        switch(i->kind){
            case LABEL_IR:
                idebug("Print LABEL_IR\n");
                fprintf(f,"LABEL ");
                PrintOperand(i->op[0]);
                fprintf(f," :\n");
                break;
            case FUNC_IR://函数入口
                fprintf(f,"\n");
                idebug("Print FUNC _IR\n");
                fprintf(f,"FUNCTION ");
                PrintOperand(i->op[0]);
                fprintf(f," :\n");
                break;
            case ASSIGN_IR:
                idebug("Print ASSIGN_IR\n");
                PrintOperand(i->op[0]);
                fprintf(f," := ");
                PrintOperand(i->op[1]);
                fprintf(f," \n");
                break;
            case ADD_IR:    
                idebug("Print ADD_IR\n");
                PrintOperand(i->op[0]);
                fprintf(f," := ");
                PrintOperand(i->op[1]);
                fprintf(f," + ");
                PrintOperand(i->op[2]);
                fprintf(f," \n");
                break;
            case SUB_IR:
                idebug("Print SUB_IR\n");
                PrintOperand(i->op[0]);
                fprintf(f," := ");
                PrintOperand(i->op[1]);
                fprintf(f," - ");
                PrintOperand(i->op[2]);
                fprintf(f," \n");
                break;
            case MUL_IR:
                idebug("Print MUL_IR\n");
                PrintOperand(i->op[0]);
                fprintf(f," := ");
                PrintOperand(i->op[1]);
                fprintf(f," * ");
                PrintOperand(i->op[2]);
                fprintf(f," \n");
                break;
            case DIV_IR:
                idebug("Print DIV_IR\n");
                PrintOperand(i->op[0]);
                fprintf(f," := ");
                PrintOperand(i->op[1]);
                fprintf(f," / ");
                PrintOperand(i->op[2]);
                fprintf(f," \n");
                break;
            case GOTO_IR:
                idebug("Print GOTO_IR\n");
                fprintf(f,"GOTO ");
                PrintOperand(i->op[0]);
                fprintf(f," \n");
                break;
            case IFGOTO_IR://IF t1 op t2 GOTO,trick是拆分成两条第二条当成GOTO ，但这里不打换行
                idebug("Print IFGOTO_IR\n");
                fprintf(f,"IF ");
                PrintOperand(i->op[0]);
                fprintf(f," ");
                PrintOperand(i->op[1]);
                fprintf(f," ");
                PrintOperand(i->op[2]);
                fprintf(f," ");
                break;
            case RETURN_IR:
                idebug("Print RETURN_IR\n");
                fprintf(f,"RETURN ");
                PrintOperand(i->op[0]);
                fprintf(f," \n");
                break;
            case DEC_IR:
                idebug("Print DEC_IR\n");
                fprintf(f,"DEC ");
                PrintOperand(i->op[0]);
                fprintf(f," ");
                PrintOperand(i->op[1]);
                fprintf(f," \n");
                break;
            case ARG_IR:
                idebug("Print ARG_IR\n");
                fprintf(f,"ARG ");
                PrintOperand(i->op[0]);
                fprintf(f," \n");
                break;
            case CALL_IR://op0 := CALL op1 
                idebug("Print CALL_IR\n");
                PrintOperand(i->op[0]);
                fprintf(f," := CALL ");
                PrintOperand(i->op[1]);
                fprintf(f," \n");
                break;
            case PARAM_IR:
                idebug("Print PARAM_IR\n");
                fprintf(f,"PARAM ");
                PrintOperand(i->op[0]);
                fprintf(f," \n");
                break;
            case READ_IR:
                idebug("Print READ_IR\n");
                fprintf(f,"READ ");
                PrintOperand(i->op[0]);
                fprintf(f," \n");
                break;
            case WRITE_IR:
                idebug("Print WRITE_IR\n");
                fprintf(f,"WRITE ");
                PrintOperand(i->op[0]);
                fprintf(f," \n");
                break;
            default:
                printf("Error in print intercode:unknown kind\n");
        }
        p = p->next;
    }
    fclose(f);
    printf("----Finish Print Intercodes!---\n");
}
//打印操作数
void PrintOperand(Operand op){
    char* s;int var;
    int opdebug =0;
    switch(op->kind){
        case VARIABLE_OP://用户定义变量名
            if(opdebug)idebug("Print OP VARIABLE\n");
            if(op->u.name==NULL){
                idebug("Error in Print OP:VARIABLE_OP = null\n");
                assert(0);
            }
            s = op->u.name;
            if(op->u.name[0]=='t')fprintf(f,"t");//一个trick：如果用户定义变量名为t1，前面多打个t来避免与临时变量重名
            fprintf(f,"%s",s);
            break;
        case TMPVAR_OP://临时变量名，标签名
            if(opdebug)idebug("Print OP TMPVAR\n");
            if(op->u.name==NULL){
                idebug("Error in Print OP:TMPVAR_OP = null\n");
                assert(0);
            }
            s = op->u.name;
            fprintf(f,"%s",s);
            break;
        case CONSTANT_OP://常量
            if(opdebug)idebug("Print OP CONSTANT\n");
            var = op->u.value;
            fprintf(f,"#%d",var);//常量用#表示
            break;
        case NUMBER_OP://数字
            if(opdebug)idebug("Print OP NUMBER\n");
            var = op->u.value;
            fprintf(f,"%d",var);//常量用#表示
            break;
        case GETADDR_OP://取地址
            if(opdebug)idebug("Print OP GETADDR_OP\n");
            if(op->u.name==NULL){
                idebug("Error in Print OP:GETADDR_OP = null\n");
                assert(0);
            }
            s = op->u.name;
            fprintf(f,"&");
            if(op->u.name[0]=='t')fprintf(f,"t");//一样需要多加一个t
            fprintf(f,"%s",s);
            break;
        case GETVAL_OP://解引用
            if(opdebug)idebug("Print OP GETVAL_OP\n");
            if(op->u.name==NULL){
                idebug("Error in Print OP:GETVAL_OP = null\n");
                assert(0);
            }
            s = op->u.name;
            fprintf(f,"*");
            if(op->u.name[0]=='t')fprintf(f,"t");
            fprintf(f,"%s",s);
            break;
        case GETADDRTMP_OP://取地址
            if(opdebug)idebug("Print OP GETADDRTMP_OP\n");
            if(op->u.name==NULL){
                idebug("Error in Print OP:GETADDR_OP = null\n");
                assert(0);
            }
            s = op->u.name;
            fprintf(f,"&");
            fprintf(f,"%s",s);
            break;
        case GETVALTMP_OP://解引用
            if(opdebug)idebug("Print OP GETVALTMP_OP\n");
            if(op->u.name==NULL){
                idebug("Error in Print OP:GETVAL_OP = null\n");
                assert(0);
            }
            s = op->u.name;
            fprintf(f,"*");
            fprintf(f,"%s",s);
            break;
        case FUNC_OP://函数名
            if(opdebug)idebug("Print OP FUNCOP\n");
            if(op->u.name==NULL){
                idebug("Error in Print OP:FUNC_OP = null\n");
            }
            s = op->u.name;
            fprintf(f,"%s",s);
            break;
        default:
            printf("Error in PrintOperand:wrong type\n");
            assert(0);
    }
}

//中间代码翻译：迭代遍历语法生成树
void Translate_Program(Tnode* s){//Program → 
    idebug("Translate_Program---\n");
    InterCodes_init();
    assert(!strcmp(s->name,"Program"));
    if(!strcmp(s->firstchild->name,"ExtDefList")) Translate_Extdeflist(s->firstchild);
}

void Translate_Extdeflist(Tnode *s){//ExtDefList → ExtDef ExtDefList|空
    idebug("Translate_Extdeflist---\n");
    assert(!strcmp(s->name,"ExtDefList"));
    Tnode* cur = s->firstchild;
    if(!strcmp(cur->name,"ExtDef")) Translate_Extdef(cur);
    cur = cur->nextbro;
    if(cur!=NULL){
        assert(!strcmp(cur->name,"ExtDefList"));
        if(cur->firstchild!=NULL){Translate_Extdeflist(cur);}//ExtDefList不为空
    }
    return;
}
void Translate_Extdef(Tnode *s){
//ExtDef → Specifier ExtDecList SEMI
//      | Specifier SEMI
//      | Specifier FunDec CompSt
    idebug("Translate_Extdef---\n");
    assert(!strcmp(s->name,"ExtDef"));
    Tnode* cur = s->firstchild;
    Type* def=NULL;
    if(!strcmp(cur->name,"Specifier")) {
        //def = Specifier(cur);返回类型只有int，所以不用这个了
        if(!strcmp(cur->firstchild->name,"StructSpecifier")){//结构体定义
            //char* structname = cur->firstchild->firstchild->nextbro->firstchild->s_val;
            //assert(structname!=NULL);//ID
            //Element* e = Search(structname); 可以发现全局结构体定义信息还是存在表中的，所以不用在这里处理了，后面直接Search结构体变量
        }
        else{//TYPE：一般为INT不用处理

        }
    }
    cur = cur->nextbro;
    if(!strcmp(cur->name,"SEMI")) {//全局结构体定义，上面处理了
        
    }
    else if(!strcmp(cur->name,"ExtDecList")) {//全局变量：假设没有
        TODO
    }
    else if(!strcmp(cur->name,"FunDec")) {
        if(!strcmp(cur->nextbro->name,"CompSt")){
            //field=Fundec(cur,def,1);  //调用Fundec只是用来插入元素属性
            Translate_FunDec(cur);
            cur = cur->nextbro;
            Translate_CompSt(cur);
        }
        else{//函数声明：假设没有
            TODO
        }
    }
}
void Translate_FunDec(Tnode *s){
//FunDec → ID LP VarList RP | ID LP RP
//函数定义设置入口，并读取形参
    idebug("Translate_FunDec---\n");
    assert(!strcmp(s->name,"FunDec"));
    Tnode* cur = s->firstchild;
    assert(!strcmp(cur->name,"ID"));
    char *funcname = cur->s_val;
    idebug("Funcname : %s\n",funcname);
    Operand op = (Operand)malloc(sizeof(Operand_d));
    op->kind=FUNC_OP;
    op->u.name=funcname;
    NewInterCode(FUNC_IR,op,NULL,NULL);
    cur = cur->nextbro->nextbro;
    if(!strcmp(cur->name,"VarList")){//函数有形参列表
        FieldList *fieldlistvarlist = Varlist(cur);//调用semantic的函数在哈希表中插入形参的信息
        Translate_VarList(cur);
    }
}
void Translate_VarList(Tnode *s){
//函数的参数列表VarList → ParamDec COMMA VarList| ParamDec
//ParamDec → Specifier VarDec
//VarDec → ID| VarDec LB INT RB
//类型信息已经在上一层函数FunDec中调用semantic.c里面的Varlist插入过元素了
//这里首先需要PARAM所有参数名，然后查看形参是否是结构体/数组类型，如果是的话就已经是地址形式了，后面都改成解引用
    idebug("Translate_VarList---\n");
    assert(!strcmp(s->name,"VarList"));
    Tnode* cur = s->firstchild;//ParamDec
    assert(!strcmp(cur->name,"ParamDec"));
    if(!strcmp(cur->firstchild->name,"Specifier")){
        cur = s->firstchild;//ParamDec
        cur = cur->firstchild->nextbro;//Vardec
        char* paraname;
        //VarDec → ID| VarDec LB INT RB迭代出ID
        while(!strcmp(cur->firstchild->name,"VarDec"))cur = cur->firstchild;
        cur = cur->firstchild;//ID
        assert(!strcmp(cur->name,"ID"));
        paraname=cur->s_val;
        Operand op = (Operand)malloc(sizeof(Operand_d));
        op->kind = VARIABLE_OP;
        op->u.name = paraname;
        NewInterCode(PARAM_IR,op,NULL,NULL);
        //修改形参变量的element中的varflag值表明为形参
        Element* evar = Search(paraname);
        evar->varflag = 1;
    }
    if(cur->nextbro!=NULL){
        cur = cur->nextbro;//COMMA
        cur = cur->nextbro;
        assert(!strcmp(cur->name,"VarList"));
        Translate_VarList(cur);
    }
    
}

void Translate_CompSt(Tnode *s){
//CompSt → LC DefList StmtList RC
    idebug("Translate_CompSt---\n");
    assert(!strcmp(s->name,"CompSt"));
    Tnode* cur = childth_node(s,2);//DefList
    if(!strcmp(cur->name,"DefList")) {
        FieldList* h=(FieldList*)malloc(sizeof(FieldList));
        h=Deflist(cur,NULL,0);
        Translate_DefList(cur);//加入局部定义的所有变量类型等后面使用
        if(cur->nextbro !=NULL){
            cur = cur->nextbro; 
            if(!strcmp(cur->name,"StmtList")) {
                Translate_StmtList(cur);
            }
        }
        else{//StmtList为空：直接不干
            return;
        }
    }
    else if(!strcmp(cur->name,"StmtList")) {//DefList为空
        Translate_StmtList(cur);
        return;
    }
    else{//DefList和StmtList都是空，直接不干
        return;
    }
    
}
void Translate_DefList(Tnode *s){
//DefList → Def DefList | 空
    idebug("Translate_DefList---\n");
    assert(!strcmp(s->name,"DefList"));
    Tnode* cur = s->firstchild;
    if(!strcmp(cur->name,"Def")) {
        Translate_Def(cur);
    }
    cur = cur->nextbro;
    if(cur!=NULL){
        if(!strcmp(cur->name,"DefList")) {
            if(cur->firstchild!=NULL){Translate_DefList(cur);}//DefList不为空
        }
    }
}
void Translate_Def(Tnode *s){
//Def → Specifier DecList SEMI
    idebug("Translate_Def---\n");
    assert(!strcmp(s->name,"Def"));
    Tnode* cur = s->firstchild;//Specifier
    if(!strcmp(cur->firstchild->name,"StructSpecifier")){//结构体定义
        
    }
    cur = cur->nextbro;
    if(cur!=NULL){
        if(!strcmp(cur->name,"DecList")) {
        Translate_DecList(cur);
        }
    }
}
void Translate_DecList(Tnode *s){
//DecList → Dec | Dec COMMA DecList
    idebug("Translate_DecList---\n");
    assert(!strcmp(s->name,"DecList"));
    Tnode* cur = s->firstchild;
    if(!strcmp(cur->name,"Dec")) {
        Translate_Dec(cur);
    }
    cur = cur->nextbro;
    if(cur!=NULL){
        cur = cur->nextbro;
        if(!strcmp(cur->name,"DecList")) {
            Translate_DecList(cur);
        }
    }
}
void Translate_Dec(Tnode *s){
//Dec是单个局部变量定义
//Dec → VarDec| VarDec ASSIGNOP Exp 变量定义时赋值
//VarDec → ID| VarDec LB INT RB
    idebug("Translate_Dec---\n");
    assert(!strcmp(s->name,"Dec"));
    Tnode* cur = s->firstchild;//VarDec
    Operand op = (Operand)malloc(sizeof(Operand_d));
    char *varname;//变量名
    if(!strcmp(cur->name,"VarDec")) {//VarDec → ID | VarDec LB INT RB
        if(!strcmp(cur->firstchild->name,"ID")){
            varname = cur->firstchild->s_val;//ID,包括结构体
        }
        else if(!strcmp(cur->firstchild->name,"VarDec")){//数组
            while(!strcmp(cur->firstchild->name,"VarDec")){
                cur = cur->firstchild;
            }
            assert(!strcmp(cur->firstchild->name,"ID"));
            cur = cur->firstchild;
            assert(cur->s_val!=NULL);
            varname = cur->s_val;
        }
        else{
            printf("Error in translate:wrong VarDec\n");
            assert(0);
        }
    }
    assert(varname!=NULL);
    //看是不是数组/结构体决定要不要DEC
    Element* evar = Search(varname);
    assert(evar!=NULL);
    int size = SizeofType(evar->type);
    //idebug("-----SIZE:%d\n",size);
    if(evar->type->kind!=BASIC){//数组和结构体变量：需要DEC
        Operand op0 = (Operand)malloc(sizeof(Operand_d));
        Operand op1 = (Operand)malloc(sizeof(Operand_d));
        op0->kind = VARIABLE_OP;op0->u.name = varname;
        op1->kind = NUMBER_OP;op1->u.value = size;
        NewInterCode(DEC_IR,op0,op1,NULL);
    }

    cur = s->firstchild->nextbro;//ASSIGNOP
    if(cur!=NULL){ 
        cur = cur->nextbro;
        if(!strcmp(cur->name,"Exp")) {
            op->kind = VARIABLE_OP;
            op->u.name = varname;
            Translate_Exp(cur,op);
        }
    }
}
void Translate_StmtList(Tnode *s){
//StmtList → Stmt StmtList|空
    idebug("Translate_StmtList---\n");
    assert(!strcmp(s->name,"StmtList"));
    Tnode* cur = s->firstchild;
    if(!strcmp(cur->name,"Stmt")) {
        Translate_Stmt(cur);
    }
    cur = cur->nextbro;
    if(cur!=NULL){
        if(!strcmp(cur->name,"StmtList")) {
            if(cur->firstchild!=NULL){Translate_StmtList(cur);}//StmtList不为空
        }
    }
}
void Translate_Stmt(Tnode *s){
    idebug("Translate_Stmt---\n");
    assert(!strcmp(s->name,"Stmt"));
    Tnode* cur = s->firstchild;
    Operand op = (Operand)malloc(sizeof(Operand_d));
    if(!strcmp(cur->name,"Exp")) {
        Translate_Exp(cur,NULL);
    }
    else if(!strcmp(cur->name,"CompSt")) {
        Translate_CompSt(cur);
    }
    else if(!strcmp(cur->name,"RETURN")) {
    //return EXP ：t=Exp，return t
        cur = cur->nextbro;
        if(!strcmp(cur->name,"Exp")) {
            if(!strcmp(cur->firstchild->name,"INT")){//直接返回立即数
                op->kind =CONSTANT_OP;
                op->u.value = cur->firstchild->i_val;
                NewInterCode(RETURN_IR,op,NULL,NULL);
            }
            else{
                op = NewTmp();
                Translate_Exp(cur,op);
                NewInterCode(RETURN_IR,op,NULL,NULL);
            }
        }
    }
    else if(!strcmp(cur->name,"IF")) {
    //IF LP Exp RP Stmt | IF LP Exp RP Stmt ELSE Stmt
        Operand L1 = NewLabel();
        Operand L2 = NewLabel();
        Operand L3 = NewLabel();
        cur = childth_node(s,3);//Exp
        Translate_Cond(cur,L1,L2);//code1
        NewInterCode(LABEL_IR,L1,NULL,NULL);//label1
        cur = childth_node(s,5);//Stmt1
        Translate_Stmt(cur);//code2
        cur=cur->nextbro;
        if(cur!=NULL){//
            NewInterCode(GOTO_IR,L3,NULL,NULL);
            NewInterCode(LABEL_IR,L2,NULL,NULL);
            cur = childth_node(s,7);
            Translate_Stmt(cur);//code3
            NewInterCode(LABEL_IR,L3,NULL,NULL);
        }
        else{
            NewInterCode(LABEL_IR,L2,NULL,NULL);
        }
    }
    else if(!strcmp(cur->name,"WHILE")) {
        // /WHILE LP Exp RP Stmt
        Operand L1 = NewLabel();
        Operand L2 = NewLabel();
        Operand L3 = NewLabel();
        NewInterCode(LABEL_IR,L1,NULL,NULL);
        cur = childth_node(s,3);//Exp
        Translate_Cond(cur,L2,L3);//code1
        NewInterCode(LABEL_IR,L2,NULL,NULL);
        cur = childth_node(s,5);//Stmt
        Translate_Stmt(cur);//code2
        NewInterCode(GOTO_IR,L1,NULL,NULL);
        NewInterCode(LABEL_IR,L3,NULL,NULL);
    }
    else{
        printf("Error in translate:wrong stmt\n");
        assert(0);
    }
}

void Translate_Exp(Tnode *s,Operand place){//place是Exp前可能的变量，也可能是NULL
    idebug("Translate_Exp---\n");
    assert(!strcmp(s->name,"Exp"));
    Tnode* cur = s->firstchild;
    Operand op0 = (Operand)malloc(sizeof(Operand_d));
    Operand op1 = (Operand)malloc(sizeof(Operand_d));
    Operand op2 = (Operand)malloc(sizeof(Operand_d));
    if(!strcmp(cur->name,"INT")) { //常数
        idebug("In Exp:INT\n");
        op1->kind = CONSTANT_OP;
        op1->u.value = cur->i_val;//得到INT节点的数值
        //printf("%s = %d\n",op0->u.name,op1->u.value);
        if(place!=NULL)NewInterCode(ASSIGN_IR,place,op1,NULL);
    }
    else if(!strcmp(cur->name,"FLOAT")) {//1) 假设1：不会出现浮点型
        TODO
    }
    else if(!strcmp(cur->name,"ID")) {
        idebug("In Exp:ID\n");
        if(cur->nextbro == NULL){
            op1->kind=VARIABLE_OP;
            op1->u.name = cur->s_val;//得到节点的用户变量名
            if(place!=NULL)NewInterCode(ASSIGN_IR,place,op1,NULL);
        }
        else if(!strcmp(cur->nextbro->name,"LP")){//函数调用
        idebug("In Exp:ID->FUNC\n");
            cur = cur->nextbro->nextbro;//Args 或者 ）
            char *funcname=s->firstchild->s_val;//ID
            if(!strcmp(cur->name,"Args")){
                //看参数
                if(!strcmp(funcname,"write")){//是写函数，只有一个int类型参数
                    cur = cur->firstchild;//Exp
                    //这里可以考虑加速，如果Exp = id且不为函数直接write ID
                    if(!strcmp(cur->firstchild->name,"ID")&&(cur->firstchild->nextbro == NULL)){
                        char *varname = cur->firstchild->s_val;
                        Operand op = (Operand)malloc(sizeof(Operand_d));
                        op->kind = VARIABLE_OP;op->u.name = varname;
                        NewInterCode(WRITE_IR,op,NULL,NULL);//WRITE t1
                    }
                    else{
                        Operand op = NewTmp();//t1
                        Translate_Exp(cur,op);
                        NewInterCode(WRITE_IR,op,NULL,NULL);//WRITE t1
                    }
                }
                else{
                    Translate_Args(cur);//传入参数
                    op1->kind = FUNC_OP;op1->u.name = funcname;
                    if(place!=NULL)NewInterCode(CALL_IR,place,op1,NULL);//place = CALL funcname
                    else{
                        Operand t1 = NewTmp();//t1
                        NewInterCode(CALL_IR,t1,op1,NULL);
                    }
                }
            }
            else{//无参数的函数调用
                if(!strcmp(funcname,"read")){//是读函数
                    if(place!=NULL)NewInterCode(READ_IR,place,NULL,NULL);//place = read()
                    else{
                        //printf("Error in translate:READ has no place");
                        //assert(0);
                        Operand t1 = NewTmp();//t1
                        NewInterCode(READ_IR,t1,NULL,NULL);
                    }
                }
                else{
                    op1->kind = FUNC_OP;op1->u.name = funcname;
                    if(place!=NULL)NewInterCode(CALL_IR,place,op1,NULL);//place = CALL funcname
                    else{
                        Operand t1 = NewTmp();//t1
                        NewInterCode(CALL_IR,t1,op1,NULL);
                    }
                }
            }
        }
        else{
            assert(0);
        }
    }
    else if(!strcmp(cur->name,"MINUS")) {//取负数》0-t1
        idebug("In Exp:MINUS\n");
        cur = cur->nextbro;//Exp
        assert(!strcmp(cur->name,"Exp"));
        Operand op = NewTmp();//t1
        if(!strcmp(cur->name,"Exp")) {
            Translate_Exp(cur,op);
        }
        op1->kind=CONSTANT_OP;op1->u.value=0;
        if(place!=NULL)NewInterCode(SUB_IR,place,op1,op);//place = 0 -t1
    }
    else if(!strcmp(cur->name,"LP")) {//Exp -> LP Exp RP 去掉括号就行
        idebug("In Exp:LP\n");
        cur = cur->nextbro;//Exp
        assert(!strcmp(cur->name,"Exp"));
        //Operand op = NewTmp();
        if(!strcmp(cur->name,"Exp")) {
            Operand op = NewTmp();
            Translate_Exp(cur,op);//t1 = (Exp)
            if(place!=NULL)NewInterCode(ASSIGN_IR,place,op,NULL);
        }
    }
    else if(!strcmp(cur->name,"NOT")) {
        idebug("In Exp:NOT\n");
        TranslateExpCond(cur,place);
    }
    else if(!strcmp(cur->name,"Exp")) {
        idebug("In Exp:EXP___\n");
        assert(cur->nextbro!=NULL);
        if(!strcmp(cur->nextbro->name,"ASSIGNOP")) {//place = Exp1 = Exp2
            idebug("In Exp:EXP-ASSIGNOP\n");
            if(place==NULL&&(!strcmp(cur->firstchild->name,"ID")&&cur->firstchild->nextbro==NULL)&&!strcmp(cur->nextbro->nextbro->firstchild->name,"INT")){
                //直接ID = 立即数
                op1->kind = VARIABLE_OP;op1->u.name = cur->firstchild->s_val;
                op2->kind =CONSTANT_OP;op2->u.value = cur->nextbro->nextbro->firstchild->i_val;
                NewInterCode(ASSIGN_IR,op1,op2,NULL);
                return;
            }
            if(!strcmp(cur->firstchild->name,"ID")){//Exp1直接为ID,左值不可能函数调用
                Operand op = NewTmp();
                if(!strcmp(cur->nextbro->nextbro->name,"Exp")){//Exp2
                    Translate_Exp(cur->nextbro->nextbro,op);//t1 = Exp2
                }
                op0->kind=VARIABLE_OP;op0->u.name=cur->firstchild->s_val;
                NewInterCode(ASSIGN_IR,op0,op,NULL);//exp1 = t1(exp2)
                if(place!=NULL)NewInterCode(ASSIGN_IR,place,op0,NULL);//place = exp1
            }
            else{
                //exp1不是ID，t1 = exp1
                Operand t1 = NewTmp();
                Translate_Exp(cur,t1);//t1 = exp1
                Operand t2 = NewTmp();
                assert(!strcmp(cur->nextbro->nextbro->name,"Exp"));//Exp2
                Translate_Exp(cur->nextbro->nextbro,t2);//t2 = Exp2
                NewInterCode(ASSIGN_IR,t1,t2,NULL);//
                if(place!=NULL)NewInterCode(ASSIGN_IR,place,t1,NULL);//place
            }
        }
        else if(!strcmp(cur->nextbro->name,"PLUS")||!strcmp(cur->nextbro->name,"MINUS")||!strcmp(cur->nextbro->name,"STAR")||!strcmp(cur->nextbro->name,"DIV")) {
            //加减乘除一起
            idebug("In Exp:EXP-PLUS/MINUS/STAR/DIV\n");
            int x = 0;
            if(!strcmp(cur->nextbro->name,"PLUS"))x=ADD_IR;
            if(!strcmp(cur->nextbro->name,"MINUS"))x=SUB_IR;
            if(!strcmp(cur->nextbro->name,"STAR"))x=MUL_IR;
            if(!strcmp(cur->nextbro->name,"DIV"))x=DIV_IR;
            assert(x);
            op1 = NewTmp();//t1
            op2 = NewTmp();//t2
            Translate_Exp(cur,op1);//Exp1
            Translate_Exp(cur->nextbro->nextbro,op2);//Exp2
            assert(place!=NULL);
            //下面的if主要是用来处理表达式中带有地址的问题
            if((op1->kind==GETADDRTMP_OP||op1->kind==GETVALTMP_OP)&&(op2->kind==GETADDRTMP_OP||op2->kind==GETVALTMP_OP)){
                Operand t3 = NewTmp();//t3
                Operand t4 = NewTmp();//t4
                NewInterCode(ASSIGN_IR,t3,op1,NULL);
                NewInterCode(ASSIGN_IR,t4,op2,NULL);
                NewInterCode(x,place,t3,t4);
            }
            else if(op1->kind==GETADDRTMP_OP||op1->kind==GETVALTMP_OP){
                Operand t3 = NewTmp();//t3
                NewInterCode(ASSIGN_IR,t3,op1,NULL);
                NewInterCode(x,place,t3,op2);
            }
            else if(op2->kind==GETADDRTMP_OP||op2->kind==GETVALTMP_OP){
                Operand t3 = NewTmp();//t3
                NewInterCode(ASSIGN_IR,t3,op2,NULL);
                NewInterCode(x,place,op1,t3);
            }
            else { 
                NewInterCode(x,place,op1,op2);//place = exp1 + exp2
            }
        }
        else if(!strcmp(cur->nextbro->name,"AND")) {
            idebug("In Exp:EXP-AND\n");
            TranslateExpCond(cur,place);
        }
        else if(!strcmp(cur->nextbro->name,"OR")) {
            idebug("In Exp:EXP-OR\n");
            TranslateExpCond(cur,place);
        }
        else if(!strcmp(cur->nextbro->name,"RELOP")) {
            idebug("In Exp:EXP-RELOP\n");
            TranslateExpCond(cur,place);
        }
        else if(!strcmp(cur->nextbro->name,"LB")) {//数组
        //EXP1 LB EXP2 RB,EXP1是地址，EXP2是整数    
            idebug("In Exp:EXP-ARRAY\n");
            Operand t1 = NewTmp();
            Operand t2 = NewTmp();
            Operand t3 = NewTmp();
            //先找到数组名
            int arrlen = 0;//记录数组的当前维数
            while(strcmp(cur->firstchild->name,"ID")){
                arrlen++;
                cur = cur->firstchild;
                assert(cur!=NULL);
            }
            char* arrname = cur->firstchild->s_val;
            Element* earr = Search(arrname);
            assert(earr->type->kind==ARRAY);
            cur = s->firstchild;
            if(!strcmp(cur->firstchild->name,"ID")){
                Operand addr = (Operand)malloc(sizeof(Operand_d));//这个数组开头地址
                if(earr->varflag == 1){
                    addr->kind = VARIABLE_OP;addr->u.name = arrname;    
                }
                else{
                    addr->kind = GETADDR_OP;addr->u.name = arrname; 
                }
                NewInterCode(ASSIGN_IR,t1,addr,NULL);//t1 = 数组开头地址
            }
            else {
                Translate_Exp(cur,t1);//Exp1
                t1->kind = TMPVAR_OP;
            }
            Translate_Exp(cur->nextbro->nextbro,t2);//t2是后面的偏移格数
            int arrsize = ArraySize(earr->type->u.array.elem,arrlen);
            t3->kind=CONSTANT_OP;t3->u.value=arrsize;//t3是单位元素的大小
            t2->kind=TMPVAR_OP;
            Operand t4 =NewTmp();
            NewInterCode(MUL_IR,t4,t2,t3); 
            Operand fuck = (Operand)malloc(sizeof(Operand_d));
            fuck->u.name = place->u.name;
            fuck->kind = place->kind;
            NewInterCode(ADD_IR,fuck,t1,t4);//这里的fuck是place的地址，后面place都解引用
            if(place->kind = TMPVAR_OP){
                place->kind=GETVALTMP_OP;
            }
            else {
                place->kind=GETVAL_OP;
            }

        /*
            //先找到ID，Search，得到地址
            int arrlen = 1;//记录数组的维数
            while(strcmp(cur->firstchild->name,"ID")){
                arrlen++;
                cur = cur->firstchild;
                assert(cur!=NULL);
            }
            char* arrname = cur->firstchild->s_val;
            Element* earr = Search(arrname);
            assert(earr->type->kind==ARRAY);
            int arrsize = SizeofType(earr->type->u.array.elem);//单位元素的大小
            Operand t3=NewTmp();
            Operand addr = (Operand)malloc(sizeof(Operand_d));
            if(earr->varflag = 1){
                addr->kind = VARIABLE_OP;addr->u.name = arrname;    
            }
            else{
                addr->kind = GETADDR_OP;addr->u.name = arrname; 
            }
            NewInterCode(ASSIGN_IR,t3,addr,NULL);//t3 = 数组开头地址
            cur = s->firstchild;//回到Exp1开始看
            if(!strcmp(cur->firstchild->name,"ID")){//一维数组
                cur = cur->nextbro->nextbro;//Exp2
                Operand t2;
                if(!strcmp(cur->firstchild->name,"INT")){
                    // ID LB EXP RB 的exp就是int时可以加速
                    t2=(Operand)malloc(sizeof(Operand_d));
                    int nnum = cur->firstchild->i_val;//元素数量
                    int ppos = nnum*arrsize;//t2 = size*num
                    t2->kind=CONSTANT_OP;t2->u.value = ppos;
                }
                else {
                    //exp为表达式求值存到t1
                    Operand t1=NewTmp();
                    Operand t4=NewTmp();
                    t2 = NewTmp();
                    Translate_Exp(cur,t1);//t1=exp2
                    Operand op1 = (Operand)malloc(sizeof(Operand_d));
                    op1->kind = CONSTANT_OP;op1->u.value=arrsize;
                    NewInterCode(ASSIGN_IR,t4,op1,NULL);//t4 = size
                    NewInterCode(MUL_IR,t2,t1,t4);//t2 = size*num
                }
                assert(place!=NULL);
                Operand fuck = (Operand)malloc(sizeof(Operand_d));
                fuck->u.name = place->u.name;
                fuck->kind = place->kind;
                NewInterCode(ADD_IR,fuck,t3,t2);//这里的fuck是place的地址，后面place都解引用
                if(place->kind = TMPVAR_OP){
                    place->kind=GETVALTMP_OP;
                }
                else {
                    place->kind=GETVAL_OP;
                }
            }
            else{//多维数组:要看前面经历了多少个元素
                int pos = 0;int dep = 0;int ArrErrorFlag = 0;//errorlag = 1说明中间有[Exp]不是INT
                int num[999];int arrnum[999];//arrnum是宽度，num是实际位置
                while(strcmp(cur->firstchild->name,"ID")){
                    dep++;//多一维度，最后一维是1
                    Tnode* exp2 = cur->nextbro->nextbro;
                    assert(!strcmp(exp2->name,"Exp"));
                    if(strcmp(exp2->firstchild->name,"INT")){//不是INT
                        ArrErrorFlag = 1;
                        break;
                    }
                    int tmpnum = exp2->firstchild->i_val;//这一维上的数值
                    num[arrlen - dep] = tmpnum;//从0到arrlen-1
                    cur = cur->firstchild;
                    assert(cur!=NULL);
                }
                if(!ArrErrorFlag){
                    num[0]=cur->nextbro->nextbro->firstchild->i_val;
                    assert(num[0]);
                    Type* arrt = earr->type;
                    for(int i=0;i<arrlen;i++){
                        assert(arrt->kind == ARRAY);
                        arrnum[i] = arrt->u.array.size;
                        arrt = arrt->u.array.elem;
                    }
                    for(int i=0;i<arrlen;i++){
                        int tmppos = 0;
                        tmppos = num[i];
                        for(int j=i+1;j<arrlen;j++){
                            tmppos *= arrnum[j];
                        }
                        pos+=tmppos;
                    }
                    Operand t2=(Operand)malloc(sizeof(Operand_d));
                    t2->kind=CONSTANT_OP;t2->u.value=pos*4;//pos*arrsize;
                    assert(place!=NULL);
                    Operand fuck = (Operand)malloc(sizeof(Operand_d));
                    fuck->u.name = place->u.name;
                    fuck->kind = place->kind;
                    NewInterCode(ADD_IR,fuck,t3,t2);//这里的fuck是place的地址，后面place都解引用
                    if(place->kind = TMPVAR_OP){
                        place->kind=GETVALTMP_OP;
                    }
                    else {
                        place->kind=GETVAL_OP;
                    }
                    return;
                }
                //[Exp]中间不是INT的另外做
                TODO
                
            }
        */
        }
        else if(!strcmp(cur->nextbro->name,"DOT")) {//结构体的属性
            idebug("In Exp:EXP-DOT\n");
            if(!strcmp(cur->firstchild->name,"ID")&&(cur->firstchild->nextbro == NULL)){
                //直接提取struct名字查表得到结构信息
                char *structname = cur->firstchild->s_val;
                Operand op = (Operand)malloc(sizeof(Operand_d));
                Element* estruct = Search(structname);
                assert(estruct!=NULL);//不可能找不到这个元素
                if(estruct->varflag == 1){//说明是形参，直接就是地址
                    op->kind = VARIABLE_OP;op->u.name = structname;
                }
                else{
                    op->kind = GETADDR_OP;op->u.name = structname;
                }
                Operand addr = NewTmp();//得到结构体开头地址
                NewInterCode(ASSIGN_IR,addr,op,NULL);
                //下面查看结构体信息找到对应属性
                cur =cur->nextbro->nextbro;//ID
                assert(cur!=NULL);
                assert(!strcmp(cur->name,"ID"));
                char *dotname = cur->s_val;
                idebug("structname: %s ,dotname: %s\n",structname,dotname);
                FieldList* fpos = estruct->type->u.structure;
                int size = 0;
                while(strcmp(fpos->name,dotname)){//遍历属性链表找匹配
                    size+= SizeofType(fpos->type);
                    fpos = fpos->next;
                   assert(fpos!=NULL);//找完了还没有这个属性
                }
                Operand sizeop = (Operand)malloc(sizeof(Operand_d));
                sizeop->kind = CONSTANT_OP;sizeop->u.value=size;
                Operand t5 = NewTmp();
                NewInterCode(ADD_IR,t5,addr,sizeop);//得到了属性对应地址
                if(place==NULL){
                    Operand tmp = NewTmp();
                    place->kind = tmp->kind;
                    place->u.name = tmp->u.name;
                }
                Operand fuck = (Operand)malloc(sizeof(Operand_d));                        
                fuck->u.name = place->u.name;
                fuck->kind = place->kind;
                NewInterCode(ASSIGN_IR,fuck,t5,NULL);
                if(place->kind = TMPVAR_OP){
                    place->kind = GETVALTMP_OP;//修改这个kind使得传出去后在用的时候都解引用
                }
                else{
                    place->kind = GETVAL_OP;
                }
            }
            else{
                TODO
                //Exp1不是直接是结构体的ID
            }
        }
        else{
            printf("Error in translate:wrong Exp in Exp\n");
            assert(0);
        }
    }
    else {
        printf("Error in translate:No Exp type\n");
    }
}
int ArraySize(Type* t,int n){//名字是t，维度在n（需要跳过几次），例如对3维数组第一维的size就是2*3
    idebug("In Arraysize\n");
    idebug("weidu:%d\n",n);
    int base = 0;
    Type *tmp =t;
    while(tmp->kind==ARRAY){
        tmp=tmp->u.array.elem;
    }
    if(tmp->kind==BASIC)base = 4;
    else base = SizeofType(tmp);//基本类型的大小
    int res = base;
    tmp = t;
    for(int i=0;i<n;i++){
        if(tmp->kind==ARRAY)tmp=tmp->u.array.elem;
    }
    while(tmp->kind==ARRAY){
        res*=tmp->u.array.size;
        tmp=tmp->u.array.elem;
    }
    idebug("ArraySize:%d\n",res);
    return res;
}
int SizeofType(Type* t){
    idebug("In SizeofType\n");
    int size=0;
    switch(t->kind){
        case BASIC:
            size+=4;//INT
            break;
        case ARRAY:
            size+=t->u.array.size*SizeofType(t->u.array.elem);
            break;
        case STRUCTURE:
        case STRUCTVAR:
            FieldList* fpos = t->u.structure;
            while(fpos!=NULL){//遍历属性链表
                size+= SizeofType(fpos->type);
                fpos = fpos->next;
            }
            break;
        case FUNC:
            TODO
        default:
            assert(0);
    }
    idebug("SizeofType:%d\n",size);
    return size;
}
void Translate_Args(Tnode *s){//函数调用的参数列表
//Args → Exp COMMA Args| Exp
//注意：当函数参数是结构体或一维数组时（通过对ID查表可知）ARG语句的参数为结构体或数组的地址=》Exp中看到这俩都取地址
    idebug("Translate_Args---\n");
    assert(!strcmp(s->name,"Args"));
    Tnode* cur = s->firstchild;//Exp
    if(!strcmp(cur->name,"Exp")) {
        if(cur->nextbro!=NULL){//trick:ARG传入参数是反着来的，所以先看后面
            Tnode* curtmp = cur->nextbro->nextbro;//Args
            assert(!strcmp(curtmp->name,"Args"));
            Translate_Args(curtmp);
        }
        if(!strcmp(cur->firstchild->name,"ID")&&(cur->firstchild->nextbro == NULL)){
            //传入的是变量名ID只要直接传就好，加速
            //但需要注意查表判断是否是结构体或数组类型
            char* varname = cur->firstchild->s_val;
            Operand op = (Operand)malloc(sizeof(Operand_d));
            assert(varname!=NULL);
            op->u.name = varname;
            Element* e = Search(varname);
            if(e!=NULL){//是之前记录过的
                if(e->type->kind!=BASIC){
                    op->kind = GETADDR_OP;
                    NewInterCode(ARG_IR,op,NULL,NULL);
                }
                else{
                    op->kind = VARIABLE_OP;
                    NewInterCode(ARG_IR,op,NULL,NULL);
                }
            } 
            else{//没记录过的是基本类型
                op->kind = VARIABLE_OP;
                NewInterCode(ARG_IR,op,NULL,NULL);
            }
        }
        else{
            Operand op = NewTmp();//t1获得实参的数值
            Translate_Exp(cur,op);
            NewInterCode(ARG_IR,op,NULL,NULL);
        }
    }
    else{
        printf("Error in translate : wrong Exp in Args\n");
        assert(0);
    }
}

//处理Exp中的条件表达式的中间共用的入口，改成宏也行
void TranslateExpCond(Tnode *s,Operand place){
    idebug("TranslateExpCond---\n");
    Operand L1 = NewLabel(); 
    Operand L2 = NewLabel();
    Operand op0 = (Operand)malloc(sizeof(Operand_d)); // op0是#0
    op0->kind = CONSTANT_OP;op0->u.value = 0;
    Operand op1 = (Operand)malloc(sizeof(Operand_d)); // op1是#1
    op1->kind = CONSTANT_OP;op1->u.value = 1;
    NewInterCode(ASSIGN_IR,place,op0,NULL);//place = #0
    Translate_Cond(s,L1,L2);
    NewInterCode(LABEL_IR,L1,NULL,NULL);//label 1
    NewInterCode(ASSIGN_IR,place,op1,NULL);//place = #1
    NewInterCode(LABEL_IR,L2,NULL,NULL);//label 2
}

//翻译条件表达式
void Translate_Cond(Tnode *s,Operand L1,Operand L2){
    idebug("Translate_Cond---\n");
    Tnode *cur = s;//传入的s是第一个开始的Exp位置
    if(!strcmp(cur->name,"NOT")){
        cur = cur->nextbro;//Exp
        Translate_Cond(cur,L2,L1);
        return;
    }
    else if(!strcmp(cur->name,"Exp")){
        if(!strcmp(cur->nextbro->name,"RELOP")){
            Operand t1 = NewTmp();
            Operand t2 = NewTmp();
            Translate_Exp(cur,t1);//code1
            cur = cur ->nextbro;//RELOP
            char* relop = cur->s_val;// RELOP的内容
            assert(relop!=NULL);
            Operand op1 = (Operand)malloc(sizeof(Operand_d)); // op1是比较符号
            op1->kind = TMPVAR_OP;op1->u.name = relop;
            cur = cur ->nextbro;//EXP2
            Translate_Exp(cur,t2);//code2
            NewInterCode(IFGOTO_IR,t1,op1,t2);
            NewInterCode(GOTO_IR,L1,NULL,NULL);
            NewInterCode(GOTO_IR,L2,NULL,NULL);//label 2
            return;
        }else if(!strcmp(cur->nextbro->name,"AND")){
            Operand L0 = NewLabel();
            Translate_Cond(cur,L0,L2);//code1
            NewInterCode(LABEL_IR,L0,NULL,NULL);//label 0
            cur = cur->nextbro->nextbro;//EXP2
            Translate_Cond(cur,L1,L2);//code2
            return;
        }else if(!strcmp(cur->nextbro->name,"OR")){
            Operand L0 = NewLabel();
            Translate_Cond(cur,L1,L0);//code1
            NewInterCode(LABEL_IR,L0,NULL,NULL);//label 0
            cur = cur->nextbro->nextbro;//EXP2
            Translate_Cond(cur,L1,L2);//code2
            return;
        }
    }
    //处理其他else情况??
        Operand t1=NewTmp();
        Translate_Exp(cur,t1);//code1
        //code2 = [IF t1 != #0 GOTO label_true]，trick是分成两行打印
        Operand op0 = (Operand)malloc(sizeof(Operand_d)); // op0是#0
        op0->kind = CONSTANT_OP;op0->u.value = 0;
        Operand op1 = (Operand)malloc(sizeof(Operand_d)); // op1是比较符号
        op1->kind = TMPVAR_OP;op1->u.name = "!=";
        NewInterCode(IFGOTO_IR,t1,op1,op0);
        NewInterCode(GOTO_IR,L1,NULL,NULL);
        NewInterCode(GOTO_IR,L2,NULL,NULL);//label 2
}
