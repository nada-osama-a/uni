#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct Node {
    float data;
    struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
} Stack;

Stack* initialize()
{
    Stack*s=malloc(sizeof(Stack));
    s->top=NULL;
    return s;
}
Node* NewNode(float value)
{
    Node*n=malloc(sizeof(Node));
    n->next=NULL;
    n->data=value;
    return n;
}

int isEmpty(Stack* s)
{
    return (s->top==NULL||s==NULL);
}

void push(Stack* s, float value)
{
    if(s==NULL)
    {
        printf("stack is not initialized\n");
        return;
    }
    Node *N=NewNode(value);
    N->next=s->top;
    s->top=N;
}

float pop(Stack* s)
{
    Node *temp;
    if (isEmpty(s))
    {
        printf("Stack underflow!\n");
        return 0.0;
    }
    float x=s->top->data;
    temp=s->top;
    s->top=s->top->next;
    free(temp);
    return x;
}

float peek(Stack* s)
{
    if (isEmpty(s))
    {
    printf("Stack underflow!\n");
    return 0.0;
    }
    else
        return s->top->data;

}

void freeStack(Stack* s)
{
    Node *temp;
    while(!isEmpty(s))
    {
        temp = s->top;
        s->top = s->top->next;
        free(temp);
    }
    free(s);
}
void display(Stack* s)
{
    if (isEmpty(s))
    {
    printf("Stack underflow!\n");
    return ;
    }
    Node*temp=s->top;
    while(temp)
    {
        printf("%.3f\n",temp->data);
        temp=temp->next;
    }
}
int priorty(char c)
{
    switch(c)
    {
        case '^':
            return 4;
        case '*':
        case '/':
        case '%':
            return 3;
        case '+':
        case '-':
            return 2;
        default:
            return 0;
    }
}

//  Check right associativity (deepseek ely aly 3aleha)   :)
int isRightAssociative(char op) {
    return (op == '^');
}
float evaluate(char x, float op1, float op2) {
    switch(x) {
        case '+':
            return op1 + op2;
        case '-':
            return op1 - op2;
        case '*':
            return op1 * op2;
        case '/':
            if (op2 == 0) {
                printf("Error: Division by zero!\n");
                return 0;
            }
            return op1 / op2;
        case '%':
            if (op2 == 0) {
                printf("Error: Modulus by zero!\n");
                return 0;
            }
            return fmod(op1, op2);
        case '^':
            return pow(op1, op2);
        default:
            printf("Error: Unknown operator %c\n", x);
            return 0;
    }
}

int isoperand(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
int isoperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}


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
Stack*operatorstack=initialize();//this function from helper.c elmafrood 
char*postfix=(char*)malloc(sizeof(char));  //el mafrood in helper.c bardo 
int i,j=0;
for(i=0;infix[i]!='\0';i++) //loop over infix to scan it
{
    if(isoperand(infix[i]))
    {
        //add it to the postfix string
       while(isoperand(infix[i])||infix[i]=='.')
       {
        postfix[j++]=infix[i++];
       }
       postfix[j++]=' '; //add space after each operand to separate them in the output string
       i--; //decrement i to recheck the last character which is not an operand 

    }
    else if(infix[i]=='(')
    {
        push(operatorstack,infix[i]);

    }
    else if(infix[i]==')')
    {
        while(!isEmpty(operatorstack)&&peek(operatorstack)!='(')
        {
            postfix[j++]=pop(operatorstack);
        }
        //pop the '(' from stack
        pop(operatorstack);
    }
    else if(isoperator(infix[i]))
    { 
        while(!isEmpty(operatorstack)&&
        peek(operatorstack)!='('&&
        (priorty(peek(operatorstack))>=priorty(infix[i]))||(priorty(peek(operatorstack))==priorty(infix[i]))&&!isRightAssociative(infix[i]))
        {  //pop to the output then push the current operator to the stack
            postfix[j++]=pop(operatorstack);
            postfix[j++]=' ';
        }
        push(operatorstack,infix[i]);
    }

    }

    while(!isEmpty(operatorstack))
    {
        postfix[j++]=pop(operatorstack);
        postfix[j++] = ' ';
    }
    postfix[j]='\0';//to make it a string

return postfix;

}


int main()
{
    /*Stack *s=initialize();
    push(s,4);
    push(s,6);
    push(s,3);
    printf("%.2f\n\n",pop(s));
    display(s);*/
    char*exp_converted=infixTopostfix("3+4*2/(1-5)");
    printf("Postfix expression: %s\n", exp_converted);
    
    return 0;
}
