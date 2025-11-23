import os
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

current_dir = os.path.dirname(os.path.abspath(__file__))

csv_path = os.path.join(current_dir, "../results/timingsResults.csv")
df = pd.read_csv(csv_path)

# Map mode numbers to descriptive names
mode_labels = {
    -1: 'sequential',
     0: 'static',
     1: 'dynamic',
     2: 'guided'
}
df['mode_label'] = df['mode'].map(mode_labels)

# Compute min time per matrix/thread/mode
df = df.groupby(['matrix', 'threads', 'mode_label'], as_index=False)['time'].mean()

# Convert threads to categorical strings *after* grouping
df['threads_str'] = df['threads'].astype(str)

# Sort numerically
df = df.sort_values(['matrix', 'threads'])

# Define colors for each mode
mode_colors = {
    'sequential': 'black',
    'static': 'red',
    'dynamic': 'green',
    'guided': 'blue'
}

# Determine y-axis bounds
y_min = df['time'].min()
y_max = df['time'].max()

output_folder = os.path.join(current_dir, "execution_time")
os.makedirs(output_folder, exist_ok=True)

for matrix in df['matrix'].unique():
    subset = df[df['matrix'] == matrix]

    plt.figure(figsize=(6,5))

    sns.lineplot(
        data=subset,
        x='threads_str',
        y='time',
        hue='mode_label',
        marker='o',
        errorbar=None,
        palette=mode_colors
    )

    plt.title(f'matrix {matrix}')
    plt.xlabel('threads')
    plt.ylabel('execution time (s)')
    plt.ylim(y_min, y_max)
    plt.grid(True, ls="--", lw=0.5)
    plt.legend(title='Mode')

    plt.tight_layout()

    filename = os.path.join(output_folder, f"matrix{matrix}.png")
    plt.savefig(filename)
    plt.close()

print(f"All plots saved in folder '{output_folder}'.")
