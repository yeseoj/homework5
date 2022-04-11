/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4

typedef char element;   /* char 데이터 타입에 별칭 element을 붙인다. */
typedef struct
{
    element queue[MAX_QUEUE_SIZE];
    int front, rear;
} QueueType;

QueueType *createQueue();                       /* 큐 생성 */
int freeQueue(QueueType *cQ);                   /* 큐에 동적 할당된 메모리 해제 */
int isEmpty(QueueType *cQ);                     /* 큐가 비었는지 확인 */
int isFull(QueueType *cQ);                      /* 큐가 가득 찼는지 확인 */
void enQueue(QueueType *cQ, element item);      /* 큐에 원소 삽입 */
void deQueue(QueueType *cQ, element *item);     /* 큐에서 원소 삭제 */
void printQ(QueueType *cQ);                     /* 큐 출력 */
void debugQ(QueueType *cQ);                     /* 상태 확인 */
element getElement();                           /* 원소 입력받기 */

int main(void)
{
    QueueType *cQ = createQueue();
    element data;                               /* char data; 와 같다. */
    char command;

    printf("[----- [장예서] [2021041018] -----]\n");
    
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
        /* 원소를 입력받아 큐에 삽입 */
            data = getElement();
            enQueue(cQ, data);
            break;
        case 'd':
        case 'D':
        /* 큐에서 원소 삭제 */
            deQueue(cQ, &data);
            break;
        case 'p':
        case 'P':
        /* 큐 내용 출력 */
            printQ(cQ);
            break;
        case 'b':
        case 'B':
        /* 큐 상태 확인 */
            debugQ(cQ);
            break;
        case 'q':
        case 'Q':
        /* 동적 할당 메모리 해제 */
            freeQueue(cQ);
            break;
        default:
        /* 제대로 된 커맨드를 입력하지 않은 경우 경고 매세지를 출력 */
            printf("\n       >>>>>   Concentration!!   <<<<<     \n");
            break;
        }

    } while (command != 'q' && command != 'Q'); // 동적 할당 메모리를 해제하고 프로그램을 끝낸다.

    return 1;
}

/**
 * 구조체 포인터 변수 cQ에 구조체 크기만큼의 메모리를 동적 할당한다.
 * 구조체 멤버 front, rear을 0으로 초기화한다.
 * 구조체 메모리 시작 주소인 cQ를 반환한다.
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
 * cQ가 NULL이면 이미 free되었다는 의미이므로 1을 반환한다.
 * free(cQ)로 동적 할당된 메모리를 해제해 준 다음 1을 반환한다.
 */
int freeQueue(QueueType *cQ)
{
    if (cQ == NULL)
        return 1;
    free(cQ);
    return 1;
}

/**
 * 변수 item에 큐에 삽입할 원소를 입력받는다.
 * item을 반환한다.
 */
element getElement()
{
    element item;
    printf("Input element = ");
    scanf(" %c", &item);
    return item;
}

/**
 * 큐의 front, rear가 같지 않으면 비지 않았다는 의미이므로 -1을 반환한다.
 * 그렇지 않으면(둘이 같다면) 큐가 비었다는 메시지를 출력하고 0을 반환한다.
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
 * 큐의 front, rear가 같지 않으면 가득 차지 않았다는 의미이므로 -1을 반환한다.
 * 그렇지 않으면(둘이 같다면) 큐가 가득 찼다는 메시지를 출력하고 0을 반환한다.
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
 * 나머지 연산을 통해 rear에 현재의 다음 값을 대입한다. (원형 큐이므로)
 * isFull이 반환한 게 0이면 큐가 가득 찼다는 의미이므로 rear을 다시 원상태로 되돌리고 함수를 끝낸다.
 * isFull이 반환한 게 0이 아니라면 큐의 rear 위치에 원소를 삽입하고 메시지를 출력한다.
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
 * isEmpty가 반환한 게 0이면 큐가 비었다는 의미이므로 함수를 끝낸다.
 * isEmpty가 반환한 게 0이 아니라면 나머지 연산을 통해 front에 현재의 다음 값을 대입한다. (원형 큐이므로)
 * 큐의 front 위치의 값을 *item에 대입하고 메시지를 출력한다.
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
 * first에는 front의 현재의 다음 값을 대입한다.
 * last에는 rear의 현재의 다음 값을 대입한다.
 * first를 i에 대입하고 계속 i에 현재의 다음 값을 대입하면서 큐 내용을 출력한다.
 * i 값이 last와 같아지면 함수를 끝낸다.
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
 * i를 큐 끝까지 증가시키면서 i==front인 지점을 찾아 큐에서의 front 위치를 명시적으로 출력한다.
 * 큐에 찬 다른 원소들도 위치와 함께 출력한다.
 * front, rear 값을 따로 출력한다.
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
