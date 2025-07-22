from EvenNumbers import EvenNumbers
from pysatl_tsp._c.lib import *
import cffi

ffi = cffi.FFI()
t = EvenNumbers(11)

#Пытаемся создать хендлер без ручного выдления памяти
ptr_t = ffi.cast("void *", id(t))
handler = ffi.new("struct Handler *", [ffi.NULL, ffi.NULL, sumInt, ptr_t])
#Ошибка, cffi не может вычислить размер хендлера

