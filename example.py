from pysatl_tsp.core.data_providers import SimpleDataProvider
from pysatl_tsp.core.processor import MappingHandler
from pysatl_tsp.core.scrubber import LinearScrubber

# Create a data source
data = [i for i in range(100)]
#data = [str(i) for i in range(100)]
provider = SimpleDataProvider(data)

# Define a simple processing pipeline:
# 1. Create windows of 10 elements with 50% overlap
# 2. Calculate the average of each window
pipeline = (provider | LinearScrubber(window_length=10, shift_factor=0.5) | MappingHandler(map_func=lambda window: sum(window.values) / len(window)))

# Process the data
results = []
for avg in pipeline:
    results.append(avg)

print(f"Number of windows: {len(results)}")
print(f"First 3 window averages: {results[:19]}")

# Visualize results
import matplotlib.pyplot as plt
plt.figure(figsize=(10, 5))
plt.plot(results)
plt.title('Window Averages')
plt.xlabel('Window Index')
plt.ylabel('Average Value')
plt.grid(True)
plt.show()