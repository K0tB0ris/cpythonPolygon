from __future__ import annotations

from collections.abc import Iterator
from typing import (
    Any,
)

from pysatl_tsp.core.handler import Handler, Pipeline, V
from pysatl_tsp._c import ffi, lib


@ffi.def_extern()
def tsp_python_next_int(handle) -> int:
    handler: CHandler = ffi.from_handle(handle)
    source = handler.source_iter
    assert source is not None
    print("Callback get next int")
    return next(source)


class CHandler(Handler[int, int]):
    """Abstract base class for time series processing handlers.

    This class implements a Chain of Responsibility pattern for processing time series data.
    Each Handler can be connected to a source handler and process its output data.
    Handlers can be combined using the pipe operator (|) to create processing pipelines.

    :param source: The handler to use as a data source, defaults to None
    """

    def __init__(self, source: Handler[Any, int] | None = None):
        """Initialize a handler with an optional source.

        :param source: The handler to use as a data source, defaults to None
        """
        self._source = source
        self.source_iter = iter(source) if source is not None else None
        self._handle = ffi.new_handle(self)
        self._c_handler = lib.tsp_handler_init(self._handle, ffi.NULL, lib.tsp_handle_int_add)

    @property
    def source(self) -> Handler[Any, int] | None:
        """Get the source handler that provides input data to this handler.

        :return: The source handler or None if this is a root handler
        """
        return self._source

    @source.setter
    def source(self, value: Handler[Any, int]) -> None:
        """Set the source handler for this handler.

        :param value: The handler to use as a data source
        :raises RuntimeError: If the source has already been set
        """
        if self._source is not None:
            raise RuntimeError("Cannot change already setted source")
        self._source = value

    def __iter__(self) -> Iterator[int]:
        """Create an iterator over the output data produced by this handler.

        Each subclass must implement this method to define how data is processed.

        :return: An iterator yielding processed data items
        """
        while True:
            try:
                val = lib.tsp_next_int(self._c_handler)
            except StopIteration:
                raise StopIteration
            yield val

    def __or__(self, other: Handler[int, V]) -> Pipeline[int, V]:
        """Combine this handler with another handler using the pipe operator.

        This allows for the creation of processing pipelines using syntax like:
        handler1 | handler2 | handler3

        :param other: The next handler in the pipeline
        :return: A Pipeline object connecting this handler to the other handler
        """
        return Pipeline(self, other)

    def __del__(self):
        lib.tsp_handler_free(self._c_handler)

