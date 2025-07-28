typedef struct _object PyObject;

struct Handler {
	void *data;
	void *buffer;
	int bufsize;
	struct Handler *source;
	float (*operation)(void *);
	PyObject *iterator;
};

struct Handler *createHandler(void *data, struct Handler *source, float (*operation)(void *),
			      void *pyobj);
void freeHandler(struct Handler *handler);
float *nextItem(struct Handler *handler, int buf_size);
float *operationchain(struct Handler *handler, int buf_size);
