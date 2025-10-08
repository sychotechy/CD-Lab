Lex file only \
flex lexfile.l \
gcc lex.yy.c -o output_name -ll \
./output_name \
\
\
Lex and yacc file \
bison yaccfile.y \
flex lexfile.l \
gcc lex.yy.c filename.tab.c -o output_name -ll -ly \
./output_name \
