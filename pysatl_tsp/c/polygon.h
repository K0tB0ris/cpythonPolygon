typedef struct _object PyObject;

struct Handler {
	void *data;
	void *buffer;
	int bufsize;
	struct Handler *source;
	float (*cmp)(void *);
	void *module;
};

struct Handler *createHandler(void *data, struct Handler *source, float (*cmp)(void *),
			      void *pyobj);
void seeHandler(struct Handler *handler);
struct Handler *getSource(struct Handler *handler);
void freeHandler(struct Handler *handler);
void free(void *);
float *applyIter(struct Handler *handler, int *length);
float *nextBuffer(struct Handler *handler, int buf_size);
float *operationchain(struct Handler *handler,int buf_size);
