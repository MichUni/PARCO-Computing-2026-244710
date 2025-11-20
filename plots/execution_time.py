import os
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Get the directory where this Python script is located
current_dir = os.path.dirname(os.path.abspath(__file__))

# Load the CSV (relative to script folder)
csv_path = os.path.join(current_dir, "../results/timingsResults.csv")
df = pd.read_csv(csv_path)

# Convert threads to string for categorical x-axis
df['threads_str'] = df['threads'].astype(str)

# Map mode numbers to descriptive names
mode_labels = {
    -1: 'sequential',
     0: 'static',
     1: 'dynamic',
     2: 'guided'
}
df['mode_label'] = df['mode'].map(mode_labels)

# Get the min value for the different chunk sizes
df = df.groupby(['matrix', 'threads_str', 'mode_label'], as_index=False)['time'].min()

# Define colors for each mode
mode_colors = {
    'sequential': 'black',
    'static': 'red',
    'dynamic': 'green',
    'guided': 'blue'
}

# Determine global min and max for y-axis
y_min = df['time'].min()
y_max = df['time'].max()

# Create a folder to save plots (inside the script folder)
output_folder = os.path.join(current_dir, "execution_time")
os.makedirs(output_folder, exist_ok=True)

# Plot each matrix and save as PNG
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
    plt.grid(True, which="both", ls="--", lw=0.5)
    plt.legend(title='Mode')
    
    plt.tight_layout()
    
    # Save the plot as PNG inside the folder
    filename = os.path.join(output_folder, f"matrix{matrix}.png")
    plt.savefig(filename)
    plt.close()  # Close the figure to free memory

print(f"All plots saved in folder '{output_folder}'.")
