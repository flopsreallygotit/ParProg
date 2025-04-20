import pandas as pd
import matplotlib.pyplot as plt

def plot_statistics():
    df = pd.read_csv('statistics.csv')

    df['Function'] = df['Function'].str.replace('sorters::', '')

    plt.figure(figsize=(18, 6))

    plt.subplot(1, 3, 2)
    mean_times = df.groupby(['Size', 'Function'])['Time'].mean().unstack()
    mean_times.plot(kind='bar', logy=True, ax=plt.gca())
    plt.title('Mean Execution Time by Size and Function')
    plt.ylabel('Mean Time (log scale)')
    plt.xlabel('Input Size')
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')

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

if __name__ == "__main__":
    plot_statistics()
