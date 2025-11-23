import os
import pandas as pd

# Get script directory
current_dir = os.path.dirname(os.path.abspath(__file__))

# Load CSV
df = pd.read_csv(os.path.join(current_dir, "../results/timingsResults.csv"))

mode_labels = {-1: 'sequential', 0: 'static', 1: 'dynamic', 2: 'guided'}
df['mode_label'] = df['mode'].map(mode_labels)

# Remove sequential mode
filtered_df = df[df['mode_label'] != 'sequential']

# Group by mode, threads, chunkSize and sum times across matrices
grouped = filtered_df.groupby(['mode_label', 'threads', 'chunkSize'])['time'].sum().reset_index()

# Find combination with minimum total time
best_overall = grouped.loc[grouped['time'].idxmin()]

print("Best combination across all matrices:")
print(f"Mode: {best_overall['mode_label']}")
print(f"Threads: {best_overall['threads']}")
print(f"Chunk Size: {best_overall['chunkSize']}")
print(f"Total time across all matrices: {best_overall['time']:.4f}\n")

# Now get the times for this combination in each individual matrix
matrix_times = filtered_df[
    (filtered_df['mode_label'] == best_overall['mode_label']) &
    (filtered_df['threads'] == best_overall['threads']) &
    (filtered_df['chunkSize'] == best_overall['chunkSize'])
]

print("Execution times per matrix for this combination:")
for _, row in matrix_times.iterrows():
    print(f"Matrix {row['matrix']}: {row['time']:.4f} s")
