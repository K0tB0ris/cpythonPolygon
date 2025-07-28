#define PY_SSIZE_T_CLEAN
#include "handler.h"
#include <Python.h>
#include <stdio.h>

struct Handler *createHandler(void *data, struct Handler *source, float (*operation)(void *),
			      void *pyobj) {
	struct Handler *obj = malloc(sizeof(struct Handler));
	if (obj == NULL) {
		fprintf(stderr, "The function could not allocate memory for Handler \n");
		return NULL;
	}
	obj->data = data;
	obj->operation = operation;
	obj->iterator = (PyObject *)pyobj;
	obj->source = source;
	obj->bufsize = 0;
	obj->buffer = NULL;
	return obj;
}

void freeHandler(struct Handler *handler) {
	if (handler->buffer != NULL) {
		free(handler->buffer);
	}
	free(handler);
}

void seeHandler(struct Handler *handler) {
	struct Handler tt = *handler;
	printf("%p %p %p %p \n", tt.data, tt.source, tt.operation, tt.iterator);
}

/* nextItem apply operation to the next element from the iterator */

float *nextItem(struct Handler *handler, int buf_size) {
	// check handler existence
	if (handler == NULL) {
		fprintf(stderr, "Handler pointer is NULL \n");
		return NULL;
	}

	float *res = NULL;
	// return next element, if buffer is not empty
	if (handler->bufsize > 0) {
		res = (float *)handler->buffer;
		handler->bufsize--;
		return &res[buf_size - handler->bufsize - 1];
	}

	// create buffer, if it doesn't exist
	if (handler->buffer == NULL) {
		handler->buffer = (void *)malloc(buf_size * sizeof(float));
	}

	// Setting up future work with Python iterator
	PyGILState_STATE gstate = PyGILState_Ensure();
	PyObject *pIterator = handler->iterator;
	Py_INCREF(pIterator);
	PyObject *pItem;

	// Get elements from iterator into buffer
	res = (float *)handler->buffer;
	for (int j = handler->bufsize; j < buf_size; j++) {
		if ((pItem = PyIter_Next(pIterator)) != NULL) {
			handler->bufsize++;
			float tmp = (float)PyFloat_AsDouble(pItem);
			tmp = handler->operation((void *)&tmp);
			res[handler->bufsize - 1] = tmp;
			Py_DECREF(pItem);
		} else {
			break;
		}
	}
	Py_DECREF(pIterator);
	PyGILState_Release(gstate);

	// return NULL, if we don't get elements from iterator
	if (handler->bufsize == 0) {
		return NULL;
	}

	handler->bufsize--;
	return &res[buf_size - handler->bufsize - 1];
}

/* operationchain implements a chain of operations
 * Works recursively
 * Follows the chain until it will meet handler(NULL, float)
 * And then sequentially applies operations to the data
 */

float *operationchain(struct Handler *handler, int buf_size) {
	if (handler->source == NULL) {
		// Find handler(NULL, float)
		return nextItem(handler, buf_size);
	} else {
		// create buffer, if it doesn't exist
		if (handler->buffer == NULL) {
			handler->buffer = (void *)malloc(buf_size * sizeof(float));
		}

		// return next element, if buffer is not empty
		float *res = (float *)handler->buffer;
		if (handler->bufsize > 0) {
			handler->bufsize--;
			return &res[buf_size - handler->bufsize - 1];
		}

		// Apply operation to the previous results
		for (int i = 0; i < buf_size; i++) {
			float *prev = operationchain(handler->source, buf_size);
			if (prev != NULL) {
				handler->bufsize++;
				float tmp = prev[0];
				tmp = handler->operation((void *)&tmp);
				res[handler->bufsize - 1] = tmp;
			} else {
				break;
			}
		}

		// return NULL, if we don't have elements in buffer
		if (handler->bufsize == 0) {
			return NULL;
		}

		handler->bufsize--;
		return &res[buf_size - handler->bufsize - 1];
	}
}
