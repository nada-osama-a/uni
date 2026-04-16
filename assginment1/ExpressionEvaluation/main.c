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



int main()
{
    /*Stack *s=initialize();
    push(s,4);
    push(s,6);
    push(s,3);
    printf("%.2f\n\n",pop(s));
    display(s);*/
    return 0;
}
