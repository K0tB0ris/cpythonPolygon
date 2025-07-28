from collections.abc import Iterator
from typing import Any
from pysatl_tsp.core import Handler
from pysatl_tsp._c.lib import createHandler
from pysatl_tsp._c.lib import freeHandler
from pysatl_tsp._c.lib import nextItem
from pysatl_tsp._c.lib import operationchain
from pysatl_tsp._c.lib import addFive
from pysatl_tsp._c.lib import multFive
from pysatl_tsp.core.data_providers import SimpleDataProvider
import cffi

ffi = cffi.FFI()


class CSumHandler(Handler[float, float]):

    def __init__(self, source: Handler[Any, float] | None = None):
        super().__init__(source)
        if source is not None:
            self.handler = createHandler(ffi.NULL, source.handler, addFive, ffi.NULL)
        else:
            self.handler = createHandler(ffi.NULL, ffi.NULL, addFive, ffi.NULL)

    def __iter__(self) -> Iterator[float | None]:
        if self.source is None:
            raise ValueError("Source is not set")
        self.src_itr = iter(self.source)
        self.handler.iterator = ffi.cast("void*", id(self.src_itr))
        return self

    def __next__(self):
        print("next")
        res = nextItem(self.handler, 5)
        if res != ffi.NULL:
            return res[0]
        else:
            raise StopIteration

    def __del__(self):
        freeHandler(self.handler)


class CMultHandler(Handler[float, float]):

    def __init__(self, source: Handler[Any, float] | None = None):
        super().__init__(source)
        if source is not None:
            self.handler = createHandler(ffi.NULL, source.handler, multFive, ffi.NULL)
        else:
            self.handler = createHandler(ffi.NULL, ffi.NULL, multFive, ffi.NULL)

    def __iter__(self) -> Iterator[float | None]:
        if self.source is None:
            raise ValueError("Source is not set")
        self.src_itr = iter(self.source)
        self.handler.iterator = ffi.cast("void*", id(self.src_itr))
        return self

    def __next__(self):
        res = operationchain(self.handler, 5)
        if res != ffi.NULL:
            return res[0]
        else:
            raise StopIteration

    def __del__(self):
        freeHandler(self.handler)



data = [1.2]
for i in range(1, 10):
    data.append(data[0] + i)
print(data)
provider = SimpleDataProvider(data)
for elem in provider:
    print(elem, end=" ")
print("\n-----------------------")

pipeline = (provider | CSumHandler() | CSumHandler() | CSumHandler()  )
for elem in pipeline:
    print(elem)
print("--------------------------------------------------------\n")

pipeline1 = (provider | CSumHandler() | CSumHandler()| CMultHandler())
for elem in pipeline1:
    print(elem, end=" ")
print("\n\n--------------------------------------------------------\n")

