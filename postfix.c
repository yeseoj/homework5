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

/* stack ������ �켱����, lparen = 0 ���� ���� */
typedef enum{
	lparen = 0,  /* ( ���� ��ȣ */
	rparen = 9,  /* ) ������ ��ȣ*/
	times = 8,   /* * ���� */					// ����, ������ �켱������ ���� 1�� ���Ƿ� �����Ͽ���
	divide = 7,  /* / ������ */
	plus = 5,    /* + ���� */
	minus = 4,   /* - ���� */
	operand = 1	/* �ǿ����� */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression�� �����ϴ� �迭 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix�� ����� ���ڿ��� �����ϴ� �迭 */
char postfixStack[MAX_STACK_SIZE];		/* postfix�� ��ȯ�� ���� �ʿ��� ���� */
int evalStack[MAX_STACK_SIZE];			/* ����� ���� �ʿ��� ���� */

int postfixStackTop = -1;  /* postfixStack�� top */
int evalStackTop = -1;	   /* evalStack�� top */

int evalResult = 0;	   /* ��� ����� ���� */

void postfixPush(char x);			/* postfixStack���� push */
char postfixPop();					/* postfixStack���� pop */
void evalPush(int x);				/* evalStack���� push */
int evalPop();						/* evalStack���� pop */
void getInfix();					/* infix expression �ޱ� */
precedence getToken(char symbol);	/* Token ���� ���� */
precedence getPriority(char x);		/* Token �켱���� ���� */
void charCat(char* c);				/* ���� �ϳ��� ���޹޾� postfixExp�� �߰� */
void toPostfix();					/* infix expression�� postfix�� ��ȯ */ 
void debug();						/* ���� Ȯ�� */
void reset();						/* ���� �ʱ�ȭ */
void evaluation();					/* postfix ����(���) */

int main()
{
	char command;

	printf("[----- [�忹��] [2021041018] -----]\n");

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
		/* infix ���� �Է� */
			getInfix();
			break;
		case 'p': case 'P':
		/* infix ���� postfix�� ��ȯ */
			toPostfix();
			break;
		case 'e': case 'E':
		/* postfix ���� ��� */
			evaluation();
			break;
		case 'd': case 'D':
		/* ���� ���� Ȯ�� */
			debug();
			break;
		case 'r': case 'R':
		/* �Էµ� �� �ʱ�ȭ */
			reset();
			break;
		case 'q': case 'Q':
		/* ���α׷� ���� */
			break;
		default:
			/* ����� �� Ŀ�ǵ带 �Է����� ������ ��� �޽��� ��� */
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

/**
 * postfixStackTop(�ʱⰪ -1)�� 1��ŭ ����
 * ���ڷ� ���� ���ڸ� postfixStack���� postfixStackTop ��ġ�� ����
 */
void postfixPush(char x)
{
	postfixStack[++postfixStackTop] = x;
}

/**
 * postfixStack�� ����ٸ� \0 ��ȯ
 * postfixStack���� postfixStackTop ��ġ�� ���� x�� ����
 * postfixStackTop�� 1��ŭ ����
 * x�� ��ȯ
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
 * evalStackTop(�ʱⰪ -1)�� 1��ŭ ����
 * ���ڷ� ���� ���ڸ� evalStack���� evalStackTop ��ġ�� ����
 */
void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

/**
 * evalStack�� ����ٸ� -1 ��ȯ
 * evalStack���� evalStackTop ��ġ�� ���� ��ȯ
 * evalStackTop�� 1��ŭ ����
 */
int evalPop()
{
    if(evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--];
}

/**
 * infix expression�� �Է¹޴´�.
 * infixExp���� �Էµ� ���� �����Ѵ�.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

/**
 * ���ڷ� ���� ���� ������ ���� precedence�� ������ ���� ��ȯ
 * �����ڰ� �ƴ϶�� ���� �ǿ������̹Ƿ� default�� operand ��ȯ�̴�.
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
 * enum�� ���� �����Ǿ����Ƿ� ��ǻ� getToken�� ���� ������ �Ѵ�.
 */
precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * �����ϳ��� ���޹޾�, postfixExp�� �߰�
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/**
 * infixExp�� ���ڸ� �ϳ��� �о�鼭 stack�� �̿��Ͽ� postfix�� �����Ѵ�.
 * ����� postfix�� postFixExp�� ����ȴ�.
 */
void toPostfix()
{
	/* infixExp�� ���� �ϳ����� �б����� ������ */
	char *exp = infixExp;
	char x; /* �����ϳ��� �ӽ÷� �����ϱ� ���� ���� */

	/* exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� ���� */
	while(*exp != '\0')
	{
		/* �ǿ������̸� ��� ��� */
		if (getToken(*exp) == operand) {
			x = *exp;
			charCat(&x);
		}
		/* �������� ��� ������ ��� ������ �ٷ� push */
		else if (postfixStackTop == -1)
			postfixPush(*exp);
		/* 'C' �������� ��� �켱������ ���� �����Ǿ� �����Ƿ� �Է� �� push�ǰԲ� ���� */
		else if (*exp == '(') {
			postfixPush(*exp);
		} // �� �κ��� full-postfix.c �ڵ带 ���� �߰��Ͽ���
		/* ')' �������� ��� ���ÿ��� '('�� ���� ������ pop(���) */
		else if (*exp == ')') {
			while (postfixStack[postfixStackTop] != '(') {
				x = postfixPop();
				charCat(&x);
			} postfixStackTop--;	/* '('�� ���ÿ��� ���� */
		}
		/* �Է� �������� �켱������ ���� Top�� �ִ� �����ں��� 2 �̻� ���� ��� ���ÿ� push */
		else if (getPriority(*exp) > getPriority(postfixStack[postfixStackTop]) + 1)
			postfixPush(*exp);
		/* �Է� �������� �켱������ ���� Top�� �ִ� �����ں��� ���ų� ���ٸ� ���� ������ pop�ϰ� �Է� ������ push */
		else {
			x = postfixPop();
			charCat(&x);
			postfixPush(*exp);
		}
		exp++;
	}
	while (postfixStackTop != -1) {		/* ���ÿ� ���� �����ڵ��� pop */
		x = postfixPop();
		charCat(&x);
	}
	printf("Transformation completed.\n");
}

/**
 * infix, postfix ���İ� ��� ��� �� postfix ���� ���¸� Ȯ���Ѵ�.
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
 * infix, postfix �迭�� 0���� �� ���ڸ� ������ �Էµ� ������ �ʱ�ȭ�Ѵ�.
 * postfixStack �迭�� �� ���ڷ� �ʱ�ȭ�Ѵ�.
 * �� ������ Top�� �ʱⰪ�� -1��, ��� ����� 0���� �ʱ�ȭ�Ѵ�.
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

/* postfixExp, evalStack�� �̿��� ��� */
void evaluation()
{
	int i, op1, op2;
	/* postfix ������ ���� ������ �Ʒ� ������ �ݺ��Ѵ�. */
	for (i = 0; postfixExp[i] != '\0'; i++) {
		/* �ǿ����ڸ� ������ �����̹Ƿ� ���� 0�� �� ���� ���� ���ÿ� push */
		if (getToken(postfixExp[i]) == operand)
			evalPush(postfixExp[i] - '0');
		/* �����ڸ� ���꿡 �ʿ��� ��ŭ �ǿ����ڸ� pop�ϰ� �����ڿ� ���� ���� */
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
	/* ��� ����� ������ Top ��ġ�� ����Ǿ� �ִ�. */
	evalResult = evalStack[evalStackTop];
	printf("Result = %d\n", evalResult);
}

