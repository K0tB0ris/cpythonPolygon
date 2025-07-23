from pysatl_tsp.core.c_handler import CHandler
from pysatl_tsp.core.data_providers import SimpleDataProvider


l = map(float, range(10))

dp = SimpleDataProvider(l)
ch = CHandler(dp)

for num in ch:
    print(num)
