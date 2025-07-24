from collections.abc import Iterator
from typing import Any
from pysatl_tsp.core import Handler
from pysatl_tsp._c.lib import *
import cffi

ffi = cffi.FFI()


class CSumHandler(Handler[float, float]):

    def __init__(self, source: Handler[Any, float] | None = None):
        super().__init__(source)
        if source is not None:
            self.handler = createHandler(ffi.NULL, source.handler, sumFloat, ffi.NULL)
        else:
            self.handler = createHandler(ffi.NULL, ffi.NULL, sumFloat, ffi.NULL)

    def __iter__(self) -> Iterator[float | None]:
        if self.source is None:
            raise ValueError("Source is not set")
        self.src_itr = iter(self.source)
        self.handler.module = ffi.cast("void*", id(self.src_itr))
        self.buf = []
        return self

    def __next__(self):
        if (self.buf):
            return self.buf.pop(0)
        else:
            length = ffi.new("int [1]")
            res = operationchain(self.handler, length, 5, ffi.NULL)
            if res != ffi.NULL:
                self.buf = ffi.unpack(res, length[0])
                ffi.gc(res, free)
                return self.buf.pop(0)
            else:
                raise StopIteration

    def __del__(self):
        freeHandler(self.handler)


class CMultHandler(Handler[float, float]):

    def __init__(self, source: Handler[Any, float] | None = None):
        super().__init__(source)
        if source is not None:
            self.handler = createHandler(ffi.NULL, source.handler, multFloat, ffi.NULL)
        else:
            self.handler = createHandler(ffi.NULL, ffi.NULL, multFloat, ffi.NULL)

    def __iter__(self) -> Iterator[float | None]:
        if self.source is None:
            raise ValueError("Source is not set")
        self.src_itr = iter(self.source)
        self.handler.module = ffi.cast("void*", id(self.src_itr))
        self.buf = []
        return self

    def __next__(self):
        if (self.buf):
            return self.buf.pop(0)
        else:
            length = ffi.new("int [1]")
            res = operationchain(self.handler, length, 5, ffi.NULL)
            if res != ffi.NULL:
                self.buf = ffi.unpack(res, length[0])
                ffi.gc(res, free)
                return self.buf.pop(0)
            else:
                raise StopIteration

    def __del__(self):
        freeHandler(self.handler)


from pysatl_tsp.core.data_providers import CSimpleDataProvider

data = [1.5]
for i in range(1, 10):
    data.append(data[0] + i)
print(data)
provider = CSimpleDataProvider(data)
for elem in provider:
    print(elem, end="  ")
print("\n-----------------------")

pipeline = (provider | CSumHandler() | CMultHandler() | CSumHandler())
for elem in pipeline:
    print(elem, end=" ")
print()
