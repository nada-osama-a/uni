#include <stdio.h>
#include <stdlib.h>
#include"stack.h"
#include "converter.h"
#include <math.h>


//convert a infix expression to postfix expression
/*pesudo code:
---isoperand(); 
---isoperator();
---precedence();
empty stack
output string
check each token scan from left to write
if tooken is operand add it to output string
if ( push int stack 
if) pop from stack until ( if found
if token is operator pop from stack to the ouput if they have higher or equal precedence than the token
then push the token to stack
at the end pop all remaining operator from stack to output

*/
char*infixTopostfix(char*infix)
{
stack*operatorstack=createStack();//this function from helper.c elmafrood 
char*postfix=(char*)malloc(sizeof(char));  //el mafrood in helper.c bardo 
int i,j=0;
for(i=0,infix[i]!='/0';i++)//loop over infix to scan it
{
    if(isoperand(infix[i]))
    {
        //add it to the postfix string
        postfix[j++]=infix[i];

    }
    else if(infix[i]=='(')
    {
        push(operatorstack,infix[i]);

    }
    else if(infix[i]==')')
    {
        while(!isempty(operatorstack)&&peek(operatorstack)!='(')
        {
            postfix[j++]=pop(operatorstack);
        }
        //pop the '(' from stack
        pop(operatorstack);
    }
    else if(isoperator(infix[i]))
    { 
        while(!isempty(opratorstack)&&precedance(peek(operatorstack))>=presedance(infix[i]))
        {  //pop to the output then push the current operator to the stack
            postfix[j++]=pop(operatorstack);
        }
        push(operatorstack,infix[i]);
    }

    }

    while(!isempty(operatorstack))
    {
        postfix[j++]=pop(operatorstack);
    }
    postfix[j]='\0';//to make it a string

return postfix;

}
/*
how to eavaluate a postfix expression

*/
float evaluatePostfix(char* postfix)
{

}