#include "polygon.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdlib.h>

struct tsp_handler *tsp_handler_init(void *py_object, struct tsp_handler *src,
                                     int (*transform)(struct tsp_handler *, int)) {
	struct tsp_handler* handler = malloc(sizeof(*handler));
	if (!handler) {
		return NULL;
	}
	handler->py_object = py_object;
	handler->source = src;
	handler->transform = transform;
	return handler;
}

void tsp_handler_free(struct tsp_handler *handler) {
	free(handler);
}

int tsp_handle_int_add(struct tsp_handler *handler, int value) {
	(void)handler;
	return value + 5;
}

int tsp_next_int(struct tsp_handler *handler) {
	int value;
	if (handler->source) {
		value = tsp_next_int(handler->source);
	} else {
		value = tsp_python_next_int(handler->py_object);
	}
	return handler->transform(handler, value);
}
