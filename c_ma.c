#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

struct queue {
	double *buffer;
	size_t capacity;
	size_t head;
	size_t tail;
};

int queue_init(struct queue *q, size_t capacity) {
	q->buffer = malloc(capacity * sizeof(q->buffer[0]));
	if (!q->buffer) {
		return -1;
	}
	q->capacity = capacity;
	q->head = 0;
	q->tail = 0;
	return 0;
}

int queue_enqueue(struct queue *q, double value) {
	q->buffer[q->tail] = value;
	q->tail = (q->tail + 1) % q->capacity;
	return 0;
}

double queue_dequeue(struct queue *q) {
	double value = q->buffer[q->head];
	q->head = (q->head + 1) % q->capacity;
	return value;
}

int main(void) {
	struct queue q;
	size_t len = 3;
	queue_init(&q, len);
	size_t size = 0;
	double sum = 0.0;
	double max = 0.0;
	for (size_t i = 0 ; i < 10000000; ++i) {
		double value = (double)i;
		if (size < len) {
			size++;
			sum += value;
			queue_enqueue(&q, value);
		} else {
			double old = queue_dequeue(&q);
			queue_enqueue(&q, value);
			sum += (value - old);
			/*
			if (i % 100 != 0) {
				sum += (value - old);
			} else {
				sum = 0.0;
				for (size_t j = 0; j < q.capacity; ++j) {
					sum += q.buffer[j];
				}
			}
			*/
		}
		double ma = sum / size;
		max = (max < ma) ? ma : max;
	}
	printf("Max MA: %f\n", max);
	return 0;
}
