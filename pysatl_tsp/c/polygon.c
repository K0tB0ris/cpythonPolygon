#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include "polygon.h"

#define error(...) (fprintf(stderr, __VA_ARGS__))


int sumInt(void *first, void *second) {
	int *one = (int *)first;
	int *two = (int *)second;
	int sum = 0;
	sum = *one + *two;
	return sum;
}

int multInt(void *first, void *second) {
	int *one = (int *)first;
	int *two = (int *)second;
	int sum = 0;
	sum = *one * *two;
	return sum;
}

void seeHandler(struct Handler *handler) {
	struct Handler tt = *handler;
	printf("%p %p %p %p \n", tt.data, tt.source, tt.cmp, tt.module);
}

int applyHandler(struct Handler *handler) {
	if (handler == NULL) {
		error("Handler pointer is NULL \n");
		return -1;
	}
	if (handler->module == NULL) {
		error("PythonModule pointer is NULL \n");
		return -1;
	}
	PyGILState_STATE gstate = PyGILState_Ensure();
	PyObject *obj = (PyObject *)handler->module;
	PyObject *pInstance = obj;
	PyObject *pIterator = PyObject_GetIter(pInstance);
	PyObject *pItem;
	int one = 2;
	if ((pItem = PyIter_Next(pIterator)) != NULL) {
		int tmp = PyLong_AsLong(pItem);
		tmp = handler->cmp((void *)&tmp, (void *)&one);
		printf("%d \n", tmp);
		PyGILState_Release(gstate);
		return tmp;
	}
	PyGILState_Release(gstate);
	return -1;
}

struct Handler *createHandler(void *data, struct Handler *source, int (*cmp)(void *, void *),
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
	return obj;
}

void freeHandler(struct Handler *handler) {
	free(handler);
	handler = NULL;
}

struct Handler *getSource(struct Handler *handler) {
	if (handler == NULL) {
		error("Handler pointer is NULL \n");
		return NULL;
	}
	return handler->source;
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
