import matplotlib.pyplot as plt
import pandas as pd

# Path to the data file, same as in trace.py
FILEPATH = "dat\SlowingDownNeutron_trajectory.dat"

# Load the data using pandas
data = pd.read_csv(FILEPATH)

# Extract all x and y coordinates from the dataframe
x = data['x'].values
y = data['y'].values

# Create a figure for the plot
plt.figure(figsize=(10, 8))

# Create a 2D histogram (heatmap) of the neutron positions.
# 'bins' determines the grid size for the histogram.
# 'cmap' sets the color scheme.
# 'cmin=1' ensures that bins with no neutron presence are not colored.
plt.hist2d(x, y, bins=100, cmap='viridis', cmin=1)

# Add a color bar to serve as a legend for the density
cbar = plt.colorbar()
cbar.set_label('Neutron Presence Density')

# Set the title and labels for the plot
plt.title("Map of Neutron Presence Density", fontsize=16)
plt.xlabel("x (cm)", fontsize=14)
plt.ylabel("y (cm)", fontsize=14)

# Use an equal aspect ratio to represent the space correctly
plt.axis('equal')

# Add a grid for better readability
plt.grid(True, linestyle='--', alpha=0.5)

# Show the final plot
plt.show()