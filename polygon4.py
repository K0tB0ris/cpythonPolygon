from collections.abc import Iterator
from typing import Any
from pysatl_tsp.core import Handler
from pysatl_tsp._c.lib import *
import cffi

ffi = cffi.FFI()


class CPIHandler(Handler[float, float]):

    def __init__(self, source: Handler[Any, float] | None = None):
        super().__init__(source)
        if source is not None:
            self.handler = createHandler(ffi.NULL, source.handler, sumFloat, ffi.cast("void *", id(self)))
        else:
            self.handler = createHandler(ffi.NULL, ffi.NULL, sumFloat, ffi.cast("void *", id(self)))

    def __iter__(self) -> Iterator[float | None]:
        if self.source is None:
            raise ValueError("Source is not set")

        length = ffi.new("int *")
        res = applyOperation(self.handler, length)
        tmp = ffi.unpack(res, length[0])
        ffi.gc(res, free)
        yield from tmp


from pysatl_tsp.core.data_providers import CSimpleDataProvider

data = [1.5]
for i in range(1, 10):
    data.append(data[0] + i)
print(data)
provider = CSimpleDataProvider(data)
pipeline = (provider | CPIHandler())
for elem in pipeline:
    print(elem, end=" ")
print()
