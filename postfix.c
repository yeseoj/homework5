/* postfix.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 8,   /* * 곱셈 */					// 곱셈, 나눗셈 우선순위를 각각 1씩 임의로 증가하였음
	divide = 7,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1	/* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];		/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];			/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixPush(char x);			/* postfixStack으로 push */
char postfixPop();					/* postfixStack에서 pop */
void evalPush(int x);				/* evalStack으로 push */
int evalPop();						/* evalStack에서 pop */
void getInfix();					/* infix expression 받기 */
precedence getToken(char symbol);	/* Token 종류 리턴 */
precedence getPriority(char x);		/* Token 우선순위 리턴 */
void charCat(char* c);				/* 문자 하나를 전달받아 postfixExp에 추가 */
void toPostfix();					/* infix expression를 postfix로 변환 */ 
void debug();						/* 상태 확인 */
void reset();						/* 상태 초기화 */
void evaluation();					/* postfix 연산(계산) */

int main()
{
	char command;

	printf("[----- [장예서] [2021041018] -----]\n");

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
		/* infix 수식 입력 */
			getInfix();
			break;
		case 'p': case 'P':
		/* infix 수식 postfix로 변환 */
			toPostfix();
			break;
		case 'e': case 'E':
		/* postfix 수식 계산 */
			evaluation();
			break;
		case 'd': case 'D':
		/* 현재 상태 확인 */
			debug();
			break;
		case 'r': case 'R':
		/* 입력된 값 초기화 */
			reset();
			break;
		case 'q': case 'Q':
		/* 프로그램 종료 */
			break;
		default:
			/* 제대로 된 커맨드를 입력하지 않으면 경고 메시지 출력 */
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

/**
 * postfixStackTop(초기값 -1)을 1만큼 증가
 * 인자로 받은 문자를 postfixStack에서 postfixStackTop 위치에 대입
 */
void postfixPush(char x)
{
	postfixStack[++postfixStackTop] = x;
}

/**
 * postfixStack이 비었다면 \0 반환
 * postfixStack에서 postfixStackTop 위치의 값을 x에 대입
 * postfixStackTop을 1만큼 감소
 * x를 반환
 */
char postfixPop()
{
    char x;
    if(postfixStackTop == -1)
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];
    }
    return x;
}

/**
 * evalStackTop(초기값 -1)을 1만큼 증가
 * 인자로 받은 숫자를 evalStack에서 evalStackTop 위치에 대입
 */
void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

/**
 * evalStack이 비었다면 -1 반환
 * evalStack에서 evalStackTop 위치의 값을 반환
 * evalStackTop을 1만큼 감소
 */
int evalPop()
{
    if(evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--];
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

/**
 * 인자로 들어온 문자 종류에 따라 precedence에 지정된 값을 반환
 * 연산자가 아니라면 전부 피연산자이므로 default는 operand 반환이다.
 */
precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

/**
 * enum을 통해 지정되었으므로 사실상 getToken과 같은 역할을 한다.
 */
precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')
	{
		/* 피연산자이면 즉시 출력 */
		if (getToken(*exp) == operand) {
			x = *exp;
			charCat(&x);
		}
		/* 연산자일 경우 스택이 비어 있으면 바로 push */
		else if (postfixStackTop == -1)
			postfixPush(*exp);
		/* 'C' 연산자일 경우 우선순위가 낮게 설정되어 있으므로 입력 시 push되게끔 설정 */
		else if (*exp == '(') {
			postfixPush(*exp);
		} // 이 부분은 full-postfix.c 코드를 보고 추가하였음
		/* ')' 연산자일 경우 스택에서 '('이 나올 때까지 pop(출력) */
		else if (*exp == ')') {
			while (postfixStack[postfixStackTop] != '(') {
				x = postfixPop();
				charCat(&x);
			} postfixStackTop--;	/* '('을 스택에서 삭제 */
		}
		/* 입력 연산자의 우선순위가 스택 Top에 있는 연산자보다 2 이상 높을 경우 스택에 push */
		else if (getPriority(*exp) > getPriority(postfixStack[postfixStackTop]) + 1)
			postfixPush(*exp);
		/* 입력 연산자의 우선순위가 스택 Top에 있는 연산자보다 낮거나 같다면 스택 연산자 pop하고 입력 연산자 push */
		else {
			x = postfixPop();
			charCat(&x);
			postfixPush(*exp);
		}
		exp++;
	}
	while (postfixStackTop != -1) {		/* 스택에 남은 연산자들을 pop */
		x = postfixPop();
		charCat(&x);
	}
	printf("Transformation completed.\n");
}

/**
 * infix, postfix 수식과 계산 결과 및 postfix 스택 상태를 확인한다.
 */
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

/**
 * infix, postfix 배열의 0번에 널 문자를 대입해 입력된 수식을 초기화한다.
 * postfixStack 배열도 널 문자로 초기화한다.
 * 각 스택의 Top을 초기값인 -1로, 계산 결과는 0으로 초기화한다.
 */
void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';

	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

/* postfixExp, evalStack을 이용한 계산 */
void evaluation()
{
	int i, op1, op2;
	/* postfix 수식이 끝날 때까지 아래 사항을 반복한다. */
	for (i = 0; postfixExp[i] != '\0'; i++) {
		/* 피연산자면 문자인 숫자이므로 문자 0을 뺀 값을 계산용 스택에 push */
		if (getToken(postfixExp[i]) == operand)
			evalPush(postfixExp[i] - '0');
		/* 연산자면 연산에 필요한 만큼 피연산자를 pop하고 연산자에 따라 연산 */
		else {
			op2 = evalPop();
			op1 = evalPop();
			switch (getToken(postfixExp[i])) {
			case plus: evalPush(op1 + op2); break;
			case minus: evalPush(op1 - op2); break;
			case divide: evalPush(op1 / op2); break;
			case times: evalPush(op1 * op2); break;
			}
		}
	}
	/* 계산 결과는 스택의 Top 위치에 저장되어 있다. */
	evalResult = evalStack[evalStackTop];
	printf("Result = %d\n", evalResult);
}

