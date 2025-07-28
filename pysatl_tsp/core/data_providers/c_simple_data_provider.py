from collections.abc import Iterable, Iterator

from .abstract import DataProvider

from pysatl_tsp._c.lib import *
import cffi

ffi = cffi.FFI()


class CSimpleDataProvider(DataProvider[float]):

    def __init__(self, data: Iterable[float]) -> None:
        ffi = cffi.FFI()
        super().__init__()
        self.data = data
        self.handler = createHandler(ffi.NULL, ffi.NULL, addFive, ffi.NULL)

    def __iter__(self) -> Iterator[float]:
        """Create an iterator over the provided data collection.

        This method simply yields items from the data collection that was passed
        during initialization, making them available to subsequent handlers in
        the processing pipeline.

        :return: An iterator yielding items from the data collection
        """

        yield from self.data
