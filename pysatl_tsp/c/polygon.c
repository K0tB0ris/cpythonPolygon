#define PY_SSIZE_T_CLEAN
#include "polygon.h"
#include <Python.h>
#include <stdio.h>

#define error(...) (fprintf(stderr, __VA_ARGS__))

struct Handler *createHandler(void *data, struct Handler *source, float (*cmp)(void *),
			      void *pyobj) {
	struct Handler *obj = malloc(sizeof(struct Handler));
	if (obj == NULL) {
		error("The function could not allocate memory for Handler \n");
		return NULL;
	}
	obj->data = data;
	obj->cmp = cmp;
	obj->module = pyobj;
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

struct Handler *getSource(struct Handler *handler) {
	if (handler == NULL) {
		error("Handler pointer is NULL \n");
		return NULL;
	}
	return handler->source;
}

void *getPipelineFirst(struct Handler *handler) {
	if (handler->source == NULL) {
		return handler->module;
	}
	return getPipelineFirst(handler->source);
}

void seeHandler(struct Handler *handler) {
	struct Handler tt = *handler;
	printf("%p %p %p %p \n", tt.data, tt.source, tt.cmp, tt.module);
}

/* applyIter берет из  хендлера итератор
 * Применяет свою операцию к элементам из итератора
 */

float *applyIter(struct Handler *handler, int *length) {
	if (handler == NULL) {
		error("Handler pointer is NULL \n");
		return NULL;
	}
	if (handler->source == NULL) {
		error("Handler source pointer is NULL \n");
		return NULL;
	}
	PyGILState_STATE gstate = PyGILState_Ensure();
	PyObject *pIterator = (PyObject *)handler->module;
	Py_INCREF(pIterator);
	PyObject *pItem;
	float *res = NULL;
	int i = 0;
	if ((pItem = PyIter_Next(pIterator)) != NULL) {
		i++;
		res = (float *)realloc(res, (i + 1) * sizeof(float));
		float tmp = (float)PyFloat_AsDouble(pItem);
		tmp = handler->cmp((void *)&tmp);
		res[i - 1] = tmp;
		Py_DECREF(pItem);
	}
	Py_DECREF(pIterator);
	PyGILState_Release(gstate);
	*length = i;
	return res;
}

/* nextBuffer аналогичен applyIter
 * Но достает по несколько элементов из итератора
 */

float *nextBuffer(struct Handler *handler, int buf_size) {
	// check handler existence
	if (handler == NULL) {
		error("Handler pointer is NULL \n");
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
	
	//Setting up future work with Python iterator
	PyGILState_STATE gstate = PyGILState_Ensure();
	PyObject *pIterator = (PyObject *)handler->module;
	Py_INCREF(pIterator);
	PyObject *pItem;
	
	// Get elements from iterator into buffer
	res = (float *)handler->buffer;
	for (int j = handler->bufsize; j < buf_size; j++) {
		if ((pItem = PyIter_Next(pIterator)) != NULL) {
			handler->bufsize++;
			float tmp = (float)PyFloat_AsDouble(pItem);
			tmp = handler->cmp((void *)&tmp);
			res[handler->bufsize - 1] = tmp;
			Py_DECREF(pItem);
		} else {
			break;
		}
	}
	Py_DECREF(pIterator);
	PyGILState_Release(gstate);
	
	// return NULL. if we don't get elements from iterator
	if (handler->bufsize == 0) {
		return NULL;
	}
	handler->bufsize--;
	return &res[buf_size - handler->bufsize - 1];
}

/* operationchain реализуют цепочку операций
 * Работает рекурсивно
 * Идет по цепочке, пока не встретит handler(NULL, float)
 * И дальше последовательно применяет к данным операции
 */

float *operationchain(struct Handler *handler, int buf_size) {
	if (handler->source->source == NULL) { // Нашли handler(NULL, float)
		if (handler == NULL) {
			error("Handler pointer is NULL \n");
			return NULL;
		}
		float *res = NULL;
		if (handler->bufsize > 0) {
			res = (float *)handler->buffer;
			handler->bufsize--;
			return &res[buf_size - handler->bufsize - 1];
		}

		if (handler->buffer == NULL) {
			handler->buffer = (void *)malloc(buf_size * sizeof(float));
		}

		PyGILState_STATE gstate = PyGILState_Ensure();
		PyObject *pIterator = (PyObject *)handler->module;
		Py_INCREF(pIterator);
		PyObject *pItem;
		res = (float *)handler->buffer;
		for (int j = handler->bufsize; j < buf_size; j++) {
			if ((pItem = PyIter_Next(pIterator)) != NULL) {
				handler->bufsize++;
				float tmp = (float)PyFloat_AsDouble(pItem);
				tmp = handler->cmp((void *)&tmp);
				res[handler->bufsize - 1] = tmp;
				Py_DECREF(pItem);
			} else {
				break;
			}
		}
		Py_DECREF(pIterator);
		PyGILState_Release(gstate);
		if (handler->bufsize == 0) {
			return NULL;
		}
		handler->bufsize--;
		return &res[buf_size - handler->bufsize - 1];
	} else {
		// Рекурсивно идём по source, пока не дойдем до handler(NULL, float)
		if (handler->buffer == NULL) {
			handler->buffer = (void *)malloc(buf_size * sizeof(float));
		}

		float *res = (float *)handler->buffer;
		if (handler->bufsize > 0) {
			handler->bufsize--;
			return &res[buf_size - handler->bufsize - 1];
		}
		for (int i = 0; i < buf_size; i++) {
			float *prev = operationchain(handler->source, buf_size);
			if (prev != NULL) {
				handler->bufsize++;
				float tmp = prev[0];
				tmp = handler->cmp((void *)&tmp);
				res[handler->bufsize - 1] = tmp;
			} else {
				break;
			}
		}
		if (handler->bufsize == 0) {
			return NULL;
		}
		handler->bufsize--;
		return &res[buf_size - handler->bufsize - 1];
	}
}

/*
int main() {
	struct Handler test = {NULL, NULL, sumInt, NULL};
	int one = 1;
	int two = 2;
	// scanf("%d %d", &one, &two);
	printf("magic %d \n", test.cmp((void *)&one, (void *)&two));

	int tt = 1;
	printf("%d tt\n", &tt);
	printf("%d null \n", NULL);

	int t1 = 1;

	struct Handler ttt = {(void *)&t1, &test, sumInt, NULL};
	printf("%d %d %d %d \n", ttt.data, ttt.source, ttt.cmp, ttt.module);

	initHadler(&ttt, NULL, NULL, sumInt, NULL);
	printf("%d %d %d %d \n", ttt.data, ttt.source, ttt.cmp, ttt.module);

	PyObject *pName, *pModule, *pClass, *pInstance;
	Py_Initialize();

	// Add current directory to sys.path (if needed)
	PyObject *sys_module = PyImport_ImportModule("sys");
	PyObject *path_list = PyObject_GetAttrString(sys_module, "path");
	PyList_Append(path_list, PyUnicode_FromString("."));
	Py_DECREF(sys_module);
	Py_DECREF(path_list);

	pName = PyUnicode_FromString("Pythonpolygon"); // Replace with your module name
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != NULL) {
		pClass =
		    PyObject_GetAttrString(pModule, "EvenNumbers"); // Replace with your class name
		Py_DECREF(pModule);
		long c_arg = 36;
		if (pClass != NULL) {
			pInstance = PyObject_CallObject(pClass, Py_BuildValue("(i)", c_arg));

			if (pInstance != NULL) {
				struct Handler polygon = {(void *)&t1, &test, sumInt, NULL};
				printf("%d %d %d %d \n", polygon.data, polygon.source, &polygon.cmp,
				       polygon.module);

				initHadler(&polygon, NULL, NULL, sumInt, pInstance);

				printf("%d %d %d %d \n", polygon.data, polygon.source, &polygon.cmp,
				       polygon.module);


				int tmp = 0;
				while ((tmp = use_addOne(&polygon)) != -1) {
					printf("%d \n", tmp);
				}

				Py_DECREF(pInstance);
			} else {
				PyErr_Print();
			}
		} else {
			PyErr_Print();
		}
	} else {
		PyErr_Print();
	}

	Py_Finalize();
}
*/
