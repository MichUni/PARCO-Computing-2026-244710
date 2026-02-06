import os
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Get script directory
current_dir = os.path.dirname(os.path.abspath(__file__))

# Load CSV
df = pd.read_csv(os.path.join(current_dir, "../results/perfResults.csv"))

# Prepare columns
df['threads_str'] = df['threads'].astype(str)
mode_labels = {-1: 'sequential', 0: 'static', 1: 'dynamic', 2: 'guided'}
df['mode_label'] = df['mode'].map(mode_labels)

# Overall cache miss rate = cache-misses / cache-references
df['cache_miss_rate'] = df['cache-misses'] / df['cache-references']

mode_colors = {
    'sequential': 'black',
    'static': 'red',
    'dynamic': 'green',
    'guided': 'blue'
}

output_folder = os.path.join(current_dir, "cache_miss_rate")
os.makedirs(output_folder, exist_ok=True)

for matrix in df['matrix'].unique():
    subset = df[df['matrix'] == matrix]

    plt.figure(figsize=(6,5))
    sns.lineplot(
        data=subset,
        x='threads_str',
        y='cache_miss_rate',
        hue='mode_label',
        marker='o',
        palette=mode_colors,
        errorbar=None
    )

    plt.title(f"Matrix {matrix} - Overall Cache Miss Rate vs Threads")
    plt.xlabel("Threads")
    plt.ylabel("Cache Miss Rate")
    plt.grid(True, which='both', ls='--', lw=0.5)
    plt.legend(title='Mode')
    plt.tight_layout()

    plt.savefig(os.path.join(output_folder, f"matrix{matrix}_cache_miss_rate.png"), bbox_inches='tight')
    plt.close()

print(f"All cache miss rate plots saved in '{output_folder}'.")
