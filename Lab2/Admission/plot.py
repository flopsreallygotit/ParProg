import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Read the data
df = pd.read_csv('statistics.csv')

# Clean up function names by removing 'sorters::'
df['Function'] = df['Function'].str.replace('sorters::', '')

# Create three plots
plt.figure(figsize=(18, 6))

# Plot 1: Boxplot of times by function and size
plt.subplot(1, 3, 1)
sns.boxplot(data=df, x='Size', y='Time', hue='Function')
plt.title('Distribution of Execution Times')
plt.yscale('log')  # Log scale due to large differences
plt.ylabel('Time (log scale)')
plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')

# Plot 2: Mean time by function and size
plt.subplot(1, 3, 2)
mean_times = df.groupby(['Size', 'Function'])['Time'].mean().unstack()
mean_times.plot(kind='bar', logy=True, ax=plt.gca())
plt.title('Mean Execution Time by Size and Function')
plt.ylabel('Mean Time (log scale)')
plt.xlabel('Input Size')
plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')

# Plot 3: Line plot showing performance trends
plt.subplot(1, 3, 3)
for func in df['Function'].unique():
    subset = df[df['Function'] == func]
    means = subset.groupby('Size')['Time'].mean()
    stds = subset.groupby('Size')['Time'].std()
    plt.errorbar(means.index, means, yerr=stds, label=func, marker='o')
plt.xscale('log')
plt.yscale('log')
plt.title('Performance Trends with Error Bars')
plt.xlabel('Input Size (log scale)')
plt.ylabel('Time (log scale)')
plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')

plt.tight_layout()
plt.savefig('sorting_performance_comparison.png', bbox_inches='tight')
