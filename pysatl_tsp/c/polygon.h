typedef struct _object PyObject;

struct Handler {
	void *data;
	struct Handler *source;
	float (*cmp)(void *, void *);
	void *module;
};

struct Handler *createHandler(void *data, struct Handler *source, float (*cmp)(void *, void *),
			      void *pyobj);
void seeHandler(struct Handler *handler);
struct Handler *getSource(struct Handler *handler);
void freeHandler(struct Handler *handler);
void free(void *);
float *applyOperation(struct Handler *handler, int *length);
int applyHandler(struct Handler *handler);
float sumFloat(void *first, void *second);
float multFloat(void *first, void *second);
float sumInt(void *first, void *second);
float multInt(void *first, void *second);
float *applyIter(struct Handler *handler, int *length);
float sumdiv(void *first, void *second);
