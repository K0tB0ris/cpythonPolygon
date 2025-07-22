from EvenNumbers import EvenNumbers
from pysatl_tsp._c.lib import *
import cffi

ffi = cffi.FFI()
t = EvenNumbers(11)

# Как предлагает CFFI сделать указатель на Питоновский обьект
ptr = ffi.new_handle(t)
# Как в теории должен создавать
poly = createHandler(ffi.NULL, ffi.NULL, sumInt, ptr)

# Как создаю обьект я
ptr_t = ffi.cast("void *", id(t))
tt = createHandler(ffi.NULL, ffi.NULL, sumInt, ptr_t)

# Немного информации для сравнения
print(f"{id(t)} id(class example) || {ptr_t}")
print(f"{ffi.cast("long", ptr)} || {ptr} \n")
#Непонятно как достать обьект из данного указателя

print("Хендлер через оригинальный способ")
seeHandler(poly)
print("Хендлер через мой способ")
seeHandler(tt)
print("\n")

print("Elements of the class")
for elem in t:
    print(elem)
t.current = 0

print("First Iteration. Use my handler")
while (applyHandler(tt) != -1):
    pass

print("Second Iteration. Use original handler")
t.current = 0
while (applyHandler(poly) != -1):
    # Тут ловим SEGFAULT
    pass
