import pkgutil

# Monkey patch for Python 3.12
if not hasattr(pkgutil, "ImpImporter"):

    class MockImpImporter:
        def __init__(self, *args, **kwargs):
            pass

    pkgutil.ImpImporter = MockImpImporter

import numpy

if not hasattr(numpy, "NaN"):
    numpy.NaN = numpy.nan
