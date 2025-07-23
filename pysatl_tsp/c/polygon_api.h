struct tsp_handler; 

int tsp_next_int(struct tsp_handler *handler);
struct tsp_handler *tsp_handler_init(void *py_object, struct tsp_handler *src,
                                     int (*transform)(struct tsp_handler *, int));
void tsp_handler_free(struct tsp_handler *handler);

int tsp_handle_int_add(struct tsp_handler *handler, int value);

extern "Python+C" int tsp_python_next_int(void *py_object);
