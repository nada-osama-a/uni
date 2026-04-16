#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    float data;
    struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
} Stack;

Stack* initialize() {
    Stack* s = malloc(sizeof(Stack));
    s->top = NULL;
    return s;
}

Node* NewNode(float value) {
    Node* n = malloc(sizeof(Node));
    n->next = NULL;
    n->data = value;
    return n;
}

int isEmpty(Stack* s) {
    return (s == NULL || s->top == NULL);
}

void push(Stack* s, float value) {
    if(s == NULL) {
        printf("stack is not initialized\n");
        return;
    }
    Node *N = NewNode(value);
    N->next = s->top;
    s->top = N;
}

float pop(Stack* s) {
    Node *temp;
    if (isEmpty(s))
        return 0.0;

    float x = s->top->data;
    temp = s->top;
    s->top = s->top->next;
    free(temp);
    return x;
}

float peek(Stack* s) {
    if (isEmpty(s)) {
        printf("Stack underflow!\n");
        return 0.0;
    }
    return s->top->data;
}

void freeStack(Stack* s) {
    Node *temp;
    while(!isEmpty(s)) {
        temp = s->top;
        s->top = s->top->next;
        free(temp);
    }
    free(s);
}

int priority(char c) {
    switch(c) {
        case '^': return 4;
        case '*': case '/': case '%': return 3;
        case '+': case '-': return 2;
        default: return 0;
    }
}

int isRightAssociative(char op) {
    return (op == '^');
}

float evaluate(char x, float op1, float op2) {
    switch(x) {
        case '+': return op1 + op2;
        case '-': return op1 - op2;
        case '*': return op1 * op2;
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

int isoperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

// Part 2: Convert infix to postfix
char* infixTopostfix(char* infix) {
    Stack* operatorstack = initialize();
    char* postfix = (char*)malloc(strlen(infix) * 2 + 100);
    int i, j = 0;

    for(i = 0; infix[i] != '\0'; i++) {
        if(isspace(infix[i]))
            continue;

        if(infix[i] == '-') {
            int isUnary = 0;

            if(i == 0) {
                isUnary = 1;
            } else {
                int k = i - 1;
                while(k >= 0 && isspace(infix[k])) k--;
                if(k < 0 || infix[k] == '(' || isoperator(infix[k])) {
                    isUnary = 1;
                }
            }

            if(isUnary) {
                postfix[j++] = '-';
                i++;
                while(isdigit(infix[i]) || infix[i] == '.') {
                    postfix[j++] = infix[i++];
                }
                postfix[j++] = ' ';
                i--;
                continue;
            }
        }

        if(isdigit(infix[i])) {
            while(i < strlen(infix) && (isdigit(infix[i]) || infix[i] == '.')) {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';
            i--;
        }
        else if(infix[i] == '(') {
            push(operatorstack, infix[i]);
        }
        else if(infix[i] == ')') {
            while(!isEmpty(operatorstack) && peek(operatorstack) != '(') {
                postfix[j++] = pop(operatorstack);
                postfix[j++] = ' ';
            }
            if(!isEmpty(operatorstack) && peek(operatorstack) == '(')
                pop(operatorstack);
        }
        else if(isoperator(infix[i])) {
            while(!isEmpty(operatorstack) && peek(operatorstack) != '(' &&
                  (priority(peek(operatorstack)) > priority(infix[i]) ||
                   (priority(peek(operatorstack)) == priority(infix[i]) && !isRightAssociative(infix[i])))) {
                postfix[j++] = pop(operatorstack);
                postfix[j++] = ' ';
            }
            push(operatorstack, infix[i]);
        }
    }

    while(!isEmpty(operatorstack)) {
        postfix[j++] = pop(operatorstack);
        postfix[j++] = ' ';
    }
    postfix[j] = '\0';

    freeStack(operatorstack);
    return postfix;
}
float evaluatePostfix(char* postfix) {
    Stack *s = initialize();

    char* copy = (char*)malloc(strlen(postfix) + 1);
    strcpy(copy, postfix);

    char* token = strtok(copy, " ");

    while(token != NULL) {

            if(isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            float num = atof(token);
            push(s, num);
        }
        else {

            float b = pop(s);
            float a = pop(s);
            float res = evaluate(token[0], a, b);
            push(s, res);
        }

        token = strtok(NULL, " ");
    }

    float result = pop(s);
    freeStack(s);
    free(copy);
    return result;
}


int main() {
    char infix[500];

    printf("Enter an infix expression: ");
    fgets(infix, sizeof(infix), stdin);

    infix[strcspn(infix, "\n")] = '\0';

    char* postfix = infixTopostfix(infix);
    float value = evaluatePostfix(postfix);
    printf("Postfix expression: %s\n", postfix);
    printf("Value :%.3f",value);

    free(postfix);
    return 0;
}
