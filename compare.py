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
            self.handler = tsp_init_handler(ffi.NULL, source.handler, tsp_op_addFive, ffi.NULL)
        else:
            self.handler = tsp_init_handler(ffi.NULL, ffi.NULL, tsp_op_addFive, ffi.NULL)

    def __iter__(self) -> Iterator[float | None]:
        if self.source is None:
            raise ValueError("Source is not set")
        self.src_itr = iter(self.source)
        self.handler.py_iter = ffi.cast("void *", id(self.src_itr))
        return self

    def __next__(self):
        res = tsp_next_chain(self.handler, 64)
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
            yield elem + 5.0


# Create a data source
data = [float(i) for i in range(100000)]
for i in range(7):
    data = data + data
provider = SimpleDataProvider(data)

import time
rel_result = []
start_time = time.monotonic()
cnt = 10
pipe1 = (provider | SumHandler())
pipe2 = (provider | CSumHandler())

pipeline1 = (pipe1 | MAHandler(7))
pipeline2 = (pipe2 | MAHandler(7))
pipeline3 = (pipe2 | CMAHandler(7))
for avg in pipeline1:
    pass
for avg in pipeline2:
    pass
for avg in pipeline3:
    pass

for i in range(cnt):
    pipe1 = (pipe1 | SumHandler())
    pipe2 = (pipe2 | CSumHandler())
    pipeline1 = (pipe1 | MAHandler(7))
    pipeline2 = (pipe2 | MAHandler(7))
    pipeline3 = (pipe2 | CMAHandler(7))
    time1 = time.monotonic()
    for avg in pipeline1:
        pass
    time2 = time.monotonic()
    for avg in pipeline2:
        pass
    time3 = time.monotonic()
    for avg in pipeline3:
        pass
    time4 = time.monotonic()
    rel_result.append(time2 - time1)
    rel_result.append(time3 - time2)
    rel_result.append(time4 - time3)
    print(f"pass experiment {i+1}")

print(f"pipe1 contain only n python modules and python MAHandler")
print(f"pipe2 contain n c modules and python MAHandler")
print(f"pipe1 contain only c modules and CMAHandler")
print("------------------------")

for i in range(cnt):
    print(f"experiment with {i+2} modules")
    print(f"pipe1 to pipe2: {rel_result[i*3+0]/rel_result[i*3+1]}")
    print(f"pipe3 to pipe2: {rel_result[i*3+2]/rel_result[i*3+1]}")
    print(f"pipe1 to pipe3: {rel_result[i*3+0]/rel_result[i*3+2]}")
    print("----------------------------------------")