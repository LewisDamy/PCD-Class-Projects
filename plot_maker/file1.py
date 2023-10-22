import matplotlib.pyplot as plt

# Data
data_mutual = {
    "SOMA": [20, 40, 60, 80, 100, 200, 500, 1000],
    "exec_time_mutual": [14.92, 28.9, 36.88, 51.9, 64.98, 129.92, 320.54, 686.25],
    "cpu_usage_mutual": [0.09, 0.18, 0.2, 0.23, 0.24, 0.49, 1.26, 2.52],
}

data_critical = {
    "SOMA": [20, 40, 60, 80, 100, 200, 500, 1000],
    "exec_time_critical": [40.07, 96.02, 131.72, 179.82, 220.08, 441.21, 1038.74, 2088.04],
    "cpu_usage_critical": [0.06, 0.29, 0.29, 0.46, 0.46, 0.98, 2.43, 3.8],
}

# Create plots
plt.figure(figsize=(10, 5))
plt.plot(data_mutual["SOMA"], data_mutual["exec_time_mutual"], marker='o', label='Mutual Area')
plt.plot(data_critical["SOMA"], data_critical["exec_time_critical"], marker='o', label='Critical Area')
plt.xlabel("SOMA")
plt.ylabel("Execution Time")
plt.legend()
plt.title("Execution Time Comparison")
plt.grid(True)

# Save the plot as an image file
plt.savefig("execution_time_comparison.png")

# Clear the plot
plt.clf()

# Create another plot
plt.figure(figsize=(10, 5))
plt.plot(data_mutual["SOMA"], data_mutual["cpu_usage_mutual"], marker='o', label='Mutual Area')
plt.plot(data_critical["SOMA"], data_critical["cpu_usage_critical"], marker='o', label='Critical Area')
plt.xlabel("SOMA")
plt.ylabel("CPU Usage")
plt.legend()
plt.title("CPU Usage Comparison")
plt.grid(True)

# Save the second plot as an image file
plt.savefig("cpu_usage_comparison.png")
