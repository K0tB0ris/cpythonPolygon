from EvenNumbers import EvenNumbers
from pysatl_tsp._c.lib import *
import cffi

ffi = cffi.FFI()
t = EvenNumbers(20)
int_ptr1 = ffi.new("int*")
int_ptr2 = ffi.new("int*")
int_ptr1[0] = 10
int_ptr2[0] = 5
print(sumInt(ffi.cast("void*", int_ptr1), ffi.cast("void*", int_ptr2)))
ptr_t = ffi.cast("void *", id(t))
tt = createHandler(ffi.NULL, ffi.NULL, sumInt, ptr_t)
seeHandler(tt)
print()

print("Elements of the class")
for elem in t:
    print(elem)
t.current = 0

print("First Iteration. Use +2")
while (applyHandler(tt) != -1):
    pass

print("Second Iteration. Use *2")
t.current = 0
freeHandler(tt)
tt = createHandler(ffi.NULL, ffi.NULL, multInt, ptr_t)
while (applyHandler(tt) != -1):
    pass
