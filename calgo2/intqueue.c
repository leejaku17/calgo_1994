/*                                                                  */
/*   INTQUEUE.C  :  Circular Queue of Integer                       */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#define MAX_QUEUE_SIZE 100

int queue[MAX_QUEUE_SIZE];
int front, rear;

void init_queue(void)
{
    front = rear = 0;
}

void clear_queue(void)
{
    front = rear;
}

int put(int k)
{
    if ((rear + 1) % MAX_QUEUE_SIZE == front) /* queue is full */
    {
        printf("\n    Queue overflow.");
        return -1;
    }
    queue[rear] = k;
    rear = ++rear % MAX_QUEUE_SIZE;
    return k;
}

int get(void)
{
    int i;
    if (front == rear) /* queue is empty */
    {
        printf("\n    Queue underflow.");
        return -1;
    }
    i = queue[front];
    front = ++front % MAX_QUEUE_SIZE;
    return i;
}

void print_queue(void)
{
    int i;
    printf("\n  Queue contents : Front ----> Rear\n");
    for (i = front; i != rear; i = ++i % MAX_QUEUE_SIZE) /* caution */
        printf("%-6d", queue[i]);
}

int queue_empty(void)
{
    return front == rear;
}
