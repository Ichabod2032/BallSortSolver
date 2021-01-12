#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

struct Queue NewQueue()
{
	struct Queue q;
	q.Items = calloc(4, sizeof(q.Items));

	q.count = 0;
	q.size = 4;

	return q;
}

int Push(struct Queue *queue, int value)
{
	if (value == 0) return 0;

	if (queue->count == queue->size)
	{
		size_t newSize = queue->size * 2;
		int *tems = calloc(newSize, sizeof(int));
		memmove(tems, queue->Items, sizeof(int) * queue->count);

		if (queue->Items) free(queue->Items);

		queue->Items = tems;
		queue->size = newSize;
	}

	queue->Items[queue->count] = value;
	queue->count++;
	return value;
}

int Pop(struct Queue *queue)
{
	if (queue->count == 0) return 0;

	int index = queue->count - 1;
	int ret = queue->Items[index];
	queue->Items[index] = 0;
	queue->count--;
	return ret;
}

int Peek(const struct Queue queue)
{
	if (queue.count == 0) return 0;

	return queue.Items[queue.count - 1];
}

void FreeQueue(struct Queue queue)
{
	if (queue.Items)
	{
		free(queue.Items);
	}
}
