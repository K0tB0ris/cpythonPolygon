from pysatl_tsp.core.data_providers import SimpleDataProvider
from pysatl_tsp.core.processor import MappingHandler
from pysatl_tsp.core.scrubber import LinearScrubber
from pysatl_tsp.implementations.processor.sma_handler import MAHandler
from pysatl_tsp.implementations.processor.sma_handler import CMAHandler
from pysatl_tsp.core import Handler
from collections.abc import Iterator
from typing import Any
from pysatl_tsp._c.lib import *
import cffi

ffi = cffi.FFI()


class CSumHandler(Handler[float, float]):

    def __init__(self, source: Handler[Any, float] | None = None):
        super().__init__(source)
        if source is not None:
            self.handler = tsp_init_handler(ffi.NULL, source.handler, tsp_op_multFive, ffi.NULL)
        else:
            self.handler = tsp_init_handler(ffi.NULL, ffi.NULL, tsp_op_multFive, ffi.NULL)

    def __iter__(self) -> Iterator[float | None]:
        if self.source is None:
            raise ValueError("Source is not set")
        self.src_itr = iter(self.source)
        self.handler.py_iter = ffi.cast("void *", id(self.src_itr))
        return self

    def __next__(self):
        res = tsp_next_buffer(self.handler, 64)
        if res != ffi.NULL:
            return res[0]
        else:
            raise StopIteration

    def __del__(self):
        tsp_free_handler(self.handler)


class SumHandler(Handler[float, float]):

    def __init__(self, source: Handler[Any, float] | None = None):
        super().__init__(source)

    def __iter__(self) -> Iterator[float | None]:
        if self.source is None:
            raise ValueError("Source is not set")
        for elem in self.source:
            yield elem * 5.0




# Create a data source
data = [float(i) for i in range(1000000)]
for i in range(6):
    data = data + data
provider = SimpleDataProvider(data)


# Choose pipeline

#pipeline = (provider | SumHandler() | MAHandler(3))
#pipeline = (provider | CSumHandler() | MAHandler(3))
pipeline = (provider | CSumHandler() | CMAHandler(3))
# Process the data
max_ma = 0.0
# results = []
for avg in pipeline:
    max_ma = avg
print(max_ma)
