import os
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Get script directory
current_dir = os.path.dirname(os.path.abspath(__file__))

# Load CSV
df = pd.read_csv(os.path.join(current_dir, "../results/timingsResults.csv"))

mode_labels = {-1: 'sequential', 0: 'static', 1: 'dynamic', 2: 'guided'}
df['mode_label'] = df['mode'].map(mode_labels)

output_folder = os.path.join(current_dir, "execution_time_heatmaps")
os.makedirs(output_folder, exist_ok=True)

# Separate sequential and other modes
sequential_df = df[df['mode_label'] == 'sequential']
filtered_df = df[df['mode_label'] != 'sequential']

for matrix in filtered_df['matrix'].unique():
    # Get sequential time for this matrix
    seq_time = sequential_df[sequential_df['matrix'] == matrix]['time'].values
    seq_time = seq_time[0] if len(seq_time) > 0 else None
    
    for mode in filtered_df['mode_label'].unique():
        subset = filtered_df[(filtered_df['matrix'] == matrix) & (filtered_df['mode_label'] == mode)]

        if subset.empty:
            continue

        pivot = subset.pivot(index='chunkSize', columns='threads', values='time')
        pivot = pivot.sort_index(ascending=True)

        # Add a "Sequential" column on the left with the same sequential time
        if seq_time is not None:
            pivot.insert(0, 'seq', [seq_time]*len(pivot))

        plt.figure(figsize=(10,6))
        sns.heatmap(pivot, annot=True, fmt=".4f", cmap="viridis")

        plt.gca().invert_yaxis()
        plt.title(f"Matrix {matrix} - Mode: {mode} - Execution Time")
        plt.xlabel("Threads (seq on left)")
        plt.ylabel("Chunk Size")
        plt.tight_layout()

        plt.savefig(os.path.join(output_folder, f"matrix{matrix}_{mode}_heatmap.png"), bbox_inches='tight')
        plt.close()

print(f"Heatmaps saved in '{output_folder}' (sequential mode included on the left).")
