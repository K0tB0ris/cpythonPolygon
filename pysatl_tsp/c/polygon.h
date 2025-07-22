typedef struct _object PyObject;

int applyHandler(struct Handler *handler);
int sumInt(void *first, void *second);
int multInt(void *first, void *second);
struct Handler *createHandler(void *data, struct Handler *source, int (*cmp)(void *, void *),
			      void *pyobj);
void seeHandler(struct Handler *handler);
struct Handler *getSource(struct Handler *handler);
void freeHandler(struct Handler *handler);
