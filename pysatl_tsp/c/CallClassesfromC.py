class EvenNumbers:
    def __init__(self, max_numb):
        self.arr = range(0, max_numb + 1, 2)
        self.current = 0

    def __iter__(self):
        """
        Returns the iterator object itself.
        This method is called when iter() is used on an instance of this class.
        """
        # self.current = 0
        return self

    def changecurrent(self, numb):
        self.current = numb

    def __next__(self):
        """
        Returns the next item in the sequence.
        This method is called by the next() function or implicitly by a for loop.
        """

        if self.current < len(self.arr):
            self.current += 1  # Increment by 2 to get the next even number
            return self.arr[self.current - 1]
        else:
            raise StopIteration  # Signal the end of iteration


from pysatl_tsp._c.lib import *
import cffi

ffi = cffi.FFI()
t = EvenNumbers(11)
int_ptr1 = ffi.new("int*")
int_ptr2 = ffi.new("int*")
int_ptr1[0] = 10
int_ptr2[0] = 5
print(sumInt(ffi.cast("void*", int_ptr1), ffi.cast("void*", int_ptr2)))
ptr = ffi.new_handle(t)
print()
print(f"{id(t)} id(class example)")
print(f"{id(ffi.NULL)} id(ffi.null)")
print(f"{ffi.cast("long", ptr)} ||| {ptr} ||| {0x748cec062150}")
ptr_t = ffi.cast("void *", id(t))
print(f"{ptr_t} ||| {ffi.cast("long", ptr_t)} ")
print()
tt = createHandler(ffi.NULL, ffi.NULL, sumInt, ptr_t)
poly = createHandler(ffi.NULL, ffi.NULL, sumInt, ffi.new_handle(t))
seeHandler(poly)
print("\n\n")


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
