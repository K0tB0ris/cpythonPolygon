#define PY_SSIZE_T_CLEAN
#include "handler.h"
#include <Python.h>
#include <stdio.h>

struct tsp_handler *tsp_init_handler(void *data, struct tsp_handler *src,
				     double (*operation)(struct tsp_handler *handler, void *),
				     void *pyobj) {
	struct tsp_handler *obj = malloc(sizeof(struct tsp_handler));
	if (obj == NULL) {
		fprintf(stderr, "Could not allocate memory for Handler \n");
		return NULL;
	}
	obj->data = data;
	obj->operation = operation;
	obj->py_iter = (PyObject *)pyobj;
	obj->src = src;
	obj->buf_size = 0;
	obj->buffer = NULL;
	return obj;
}

void tsp_free_handler(struct tsp_handler *handler) {
	if (handler->buffer != NULL) {
		free(handler->buffer);
	}
	free(handler);
}

/* tsp_next_buffer apply operation to the next element from the iterator */

double *tsp_next_buffer(struct tsp_handler *handler, int buf_size) {
	// check handler existence
	if (handler == NULL) {
		fprintf(stderr, "Handler pointer is NULL \n");
		return NULL;
	}

	double *res = NULL;
	// return next element, if buffer is not empty
	if (handler->buf_size > 0) {
		res = (double *)handler->buffer;
		handler->buf_size--;
		return &res[buf_size - handler->buf_size - 1];
	}

	// create buffer, if it doesn't exist
	if (handler->buffer == NULL) {
		handler->buffer = (void *)malloc(buf_size * sizeof(double));
	}

	// Setting up future work with Python iterator
	PyGILState_STATE gstate = PyGILState_Ensure();
	PyObject *pIterator = handler->py_iter;
	Py_INCREF(pIterator);
	PyObject *pItem;

	// Get elements from iterator into buffer
	res = (double *)handler->buffer;
	for (int j = handler->buf_size; j < buf_size; j++) {
		if ((pItem = PyIter_Next(pIterator)) != NULL) {
			handler->buf_size++;
			double tmp = PyFloat_AsDouble(pItem);
			tmp = handler->operation(handler, (void *)&tmp);
			res[handler->buf_size - 1] = tmp;
			Py_DECREF(pItem);
		} else {
			break;
		}
	}
	Py_DECREF(pIterator);
	PyGILState_Release(gstate);

	// return NULL, if we don't get elements from iterator
	if (handler->buf_size == 0) {
		return NULL;
	}

	handler->buf_size--;
	return &res[buf_size - handler->buf_size - 1];
}

/* tsp_next_chain implements a chain of operations
 * Works recursively
 * Follows the chain until it will meet handler(NULL, float)
 * And then sequentially applies operations to the data
 */

double *tsp_next_chain(struct tsp_handler *handler, int buf_size) {
	if (handler->src == NULL) {
		// Find handler(NULL, float)
		return tsp_next_buffer(handler, buf_size);
	} else {
		// create buffer, if it doesn't exist
		if (handler->buffer == NULL) {
			handler->buffer = (void *)malloc(buf_size * sizeof(double));
		}

		// return next element, if buffer is not empty
		double *res = (double *)handler->buffer;
		if (handler->buf_size > 0) {
			handler->buf_size--;
			return &res[buf_size - handler->buf_size - 1];
		}

		// Apply operation to the previous results
		for (int i = 0; i < buf_size; i++) {
			double *prev = tsp_next_chain(handler->src, buf_size);
			if (prev != NULL) {
				handler->buf_size++;
				double tmp = prev[0];
				tmp = handler->operation(handler, (void *)&tmp);
				res[handler->buf_size - 1] = tmp;
			} else {
				break;
			}
		}

		// return NULL, if we don't have elements in buffer
		if (handler->buf_size == 0) {
			return NULL;
		}

		handler->buf_size--;
		return &res[buf_size - handler->buf_size - 1];
	}
}
