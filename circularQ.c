/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4

typedef char element;   /* char ������ Ÿ�Կ� ��Ī element�� ���δ�. */
typedef struct
{
    element queue[MAX_QUEUE_SIZE];
    int front, rear;
} QueueType;

QueueType *createQueue();                       /* ť ���� */
int freeQueue(QueueType *cQ);                   /* ť�� ���� �Ҵ�� �޸� ���� */
int isEmpty(QueueType *cQ);                     /* ť�� ������� Ȯ�� */
int isFull(QueueType *cQ);                      /* ť�� ���� á���� Ȯ�� */
void enQueue(QueueType *cQ, element item);      /* ť�� ���� ���� */
void deQueue(QueueType *cQ, element *item);     /* ť���� ���� ���� */
void printQ(QueueType *cQ);                     /* ť ��� */
void debugQ(QueueType *cQ);                     /* ���� Ȯ�� */
element getElement();                           /* ���� �Է¹ޱ� */

int main(void)
{
    QueueType *cQ = createQueue();
    element data;                               /* char data; �� ����. */
    char command;

    printf("[----- [�忹��] [2021041018] -----]\n");
    
    do
    {
        printf("\n-----------------------------------------------------\n");
        printf("                     Circular Q                   \n");
        printf("------------------------------------------------------\n");
        printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
        printf("------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command);

        switch (command)
        {
        case 'i':
        case 'I':
        /* ���Ҹ� �Է¹޾� ť�� ���� */
            data = getElement();
            enQueue(cQ, data);
            break;
        case 'd':
        case 'D':
        /* ť���� ���� ���� */
            deQueue(cQ, &data);
            break;
        case 'p':
        case 'P':
        /* ť ���� ��� */
            printQ(cQ);
            break;
        case 'b':
        case 'B':
        /* ť ���� Ȯ�� */
            debugQ(cQ);
            break;
        case 'q':
        case 'Q':
        /* ���� �Ҵ� �޸� ���� */
            freeQueue(cQ);
            break;
        default:
        /* ����� �� Ŀ�ǵ带 �Է����� ���� ��� ��� �ż����� ��� */
            printf("\n       >>>>>   Concentration!!   <<<<<     \n");
            break;
        }

    } while (command != 'q' && command != 'Q'); // ���� �Ҵ� �޸𸮸� �����ϰ� ���α׷��� ������.

    return 1;
}

/**
 * ����ü ������ ���� cQ�� ����ü ũ�⸸ŭ�� �޸𸮸� ���� �Ҵ��Ѵ�.
 * ����ü ��� front, rear�� 0���� �ʱ�ȭ�Ѵ�.
 * ����ü �޸� ���� �ּ��� cQ�� ��ȯ�Ѵ�.
 */
QueueType *createQueue()
{
    QueueType *cQ;
    cQ = (QueueType *)malloc(sizeof(QueueType));
    cQ->front = 0;
    cQ->rear = 0;
    return cQ;
}

/**
 * cQ�� NULL�̸� �̹� free�Ǿ��ٴ� �ǹ��̹Ƿ� 1�� ��ȯ�Ѵ�.
 * free(cQ)�� ���� �Ҵ�� �޸𸮸� ������ �� ���� 1�� ��ȯ�Ѵ�.
 */
int freeQueue(QueueType *cQ)
{
    if (cQ == NULL)
        return 1;
    free(cQ);
    return 1;
}

/**
 * ���� item�� ť�� ������ ���Ҹ� �Է¹޴´�.
 * item�� ��ȯ�Ѵ�.
 */
element getElement()
{
    element item;
    printf("Input element = ");
    scanf(" %c", &item);
    return item;
}

/**
 * ť�� front, rear�� ���� ������ ���� �ʾҴٴ� �ǹ��̹Ƿ� -1�� ��ȯ�Ѵ�.
 * �׷��� ������(���� ���ٸ�) ť�� ����ٴ� �޽����� ����ϰ� 0�� ��ȯ�Ѵ�.
 */
int isEmpty(QueueType *cQ)
{
    if (cQ->front != cQ->rear) {
        return -1;
    }
    else 
        printf("Queue is empty.\n");
    return 0;
}

/**
 * ť�� front, rear�� ���� ������ ���� ���� �ʾҴٴ� �ǹ��̹Ƿ� -1�� ��ȯ�Ѵ�.
 * �׷��� ������(���� ���ٸ�) ť�� ���� á�ٴ� �޽����� ����ϰ� 0�� ��ȯ�Ѵ�.
 */
int isFull(QueueType *cQ)
{
    if (cQ->front != cQ->rear) {
        return -1;
    }
    else 
        printf("Queue is full.\n");
    return 0;
}

/**
 * ������ ������ ���� rear�� ������ ���� ���� �����Ѵ�. (���� ť�̹Ƿ�)
 * isFull�� ��ȯ�� �� 0�̸� ť�� ���� á�ٴ� �ǹ��̹Ƿ� rear�� �ٽ� �����·� �ǵ����� �Լ��� ������.
 * isFull�� ��ȯ�� �� 0�� �ƴ϶�� ť�� rear ��ġ�� ���Ҹ� �����ϰ� �޽����� ����Ѵ�.
 */
void enQueue(QueueType *cQ, element item)
{
    cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;
    if (!isFull(cQ)) {
        cQ->rear = --cQ->rear + MAX_QUEUE_SIZE;
        return;
    }
    else {
        cQ->queue[cQ->rear] = item;
        printf("%c inserted to queue.\n", item);
    }
}

/**
 * isEmpty�� ��ȯ�� �� 0�̸� ť�� ����ٴ� �ǹ��̹Ƿ� �Լ��� ������.
 * isEmpty�� ��ȯ�� �� 0�� �ƴ϶�� ������ ������ ���� front�� ������ ���� ���� �����Ѵ�. (���� ť�̹Ƿ�)
 * ť�� front ��ġ�� ���� *item�� �����ϰ� �޽����� ����Ѵ�.
 */
void deQueue(QueueType *cQ, element *item)
{
    if (!isEmpty(cQ)) {
        return;
    }
    else {
        cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE;
        *item = cQ->queue[cQ->front];
        printf("%c deleted from queue.\n", *item);
    }
}

/**
 * first���� front�� ������ ���� ���� �����Ѵ�.
 * last���� rear�� ������ ���� ���� �����Ѵ�.
 * first�� i�� �����ϰ� ��� i�� ������ ���� ���� �����ϸ鼭 ť ������ ����Ѵ�.
 * i ���� last�� �������� �Լ��� ������.
 */
void printQ(QueueType *cQ)
{
    int i, first, last;

    first = (cQ->front + 1) % MAX_QUEUE_SIZE;
    last = (cQ->rear + 1) % MAX_QUEUE_SIZE;

    printf("Circular Queue : [");

    i = first;
    while (i != last)
    {
        printf("%3c", cQ->queue[i]);
        i = (i + 1) % MAX_QUEUE_SIZE;
    }
    printf(" ]\n");
}

/**
 * i�� ť ������ ������Ű�鼭 i==front�� ������ ã�� ť������ front ��ġ�� ��������� ����Ѵ�.
 * ť�� �� �ٸ� ���ҵ鵵 ��ġ�� �Բ� ����Ѵ�.
 * front, rear ���� ���� ����Ѵ�.
 */
void debugQ(QueueType *cQ)
{

    printf("\n---DEBUG\n");
    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        if (i == cQ->front)
        {
            printf("  [%d] = front\n", i);
            continue;
        }
        printf("  [%d] = %c\n", i, cQ->queue[i]);
    }
    printf("front = %d, rear = %d\n", cQ->front, cQ->rear);
}
