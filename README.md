yylval是一个全局变量，bison中的语法模块通过它直接获得词法模块的属性值
在yyparse（）中通过将语法符与一个局部变量数组绑定来控制语法符的属性值
产生式左边的值由右边得来，需要自定义给出语义动作（默认情况下$$=$1）

