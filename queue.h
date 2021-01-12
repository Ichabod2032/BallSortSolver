#ifndef queue_h
#define queue_h

struct Queue
{
	size_t size;
	size_t count;
	int *Items;
};

struct Queue NewQueue();
int Push(struct Queue *queue, int value);
int Pop(struct Queue *queue);
int Peek(const struct Queue queue);
void FreeQueue(struct Queue queue);

#endif
