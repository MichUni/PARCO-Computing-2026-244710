import pandas as pd
import matplotlib.pyplot as plt
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
RESULTS_DIR = os.path.join(SCRIPT_DIR, '..', 'results')
SAVE_DIR = os.path.join(SCRIPT_DIR, 'speedup_efficiency')

# Defined fixed X-axis scale
X_TICKS = [1, 2, 4, 8, 16, 32, 64, 128]

def create_scaling_plot(df, filename, title, is_strong=True):
    if not os.path.exists(SAVE_DIR):
        os.makedirs(SAVE_DIR)

    # Create 2 subplots (Speedup on left, Efficiency on right)
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(18, 7))
    
    matrices = df['matrix'].unique()
    
    for mat in matrices:
        m_data = df[df['matrix'] == mat].sort_values('processes')
        # Filter data to only include the specified X_TICKS if they exist in CSV
        m_data = m_data[m_data['processes'].isin(X_TICKS)]
        
        procs = m_data['processes']
        t1 = m_data.iloc[0]['total_time(ms)']
        
        # Calculate Metrics
        speedup = t1 / m_data['total_time(ms)']
        efficiency = speedup / procs if is_strong else (t1 / m_data['total_time(ms)'])

        # Print the best speedup for this matrix
        max_speedup = speedup.max()
        best_proc = procs[speedup.idxmax()]
        print(f'Matrix {mat}: Best speedup = {max_speedup:.2f} at {best_proc} processes')

        # Plot Speedup
        ax1.plot(procs, speedup, marker='o', linewidth=2, label=f'Matrix_{mat}')
        # Plot Efficiency
        ax2.plot(procs, efficiency, marker='s', linewidth=2, label=f'Matrix_{mat}')

    # Apply formatting to both axes
    for ax in [ax1, ax2]:
        ax.set_xscale('log', base=2) # Using log scale base 2 for better spacing of 1, 2, 4...
        ax.set_xticks(X_TICKS)
        ax.get_xaxis().set_major_formatter(plt.ScalarFormatter())
        ax.set_xlim(0.8, 140) # Padding to see the '1' and '128' clearly
        ax.set_xlabel('Number of Processes')
        ax.grid(True, which="both", linestyle='--', alpha=0.5)
        ax.legend()

    # Specific labels
    ax1.set_title(f'{title}: Speed-up')
    ax1.set_ylabel('Speed-up ($T_1 / T_p$)')
    if is_strong:
        ax1.plot(X_TICKS, X_TICKS, color='gray', linestyle=':', label='Ideal')
        ax1.legend()

    ax2.set_title(f'{title}: Efficiency')
    ax2.set_ylabel('Efficiency')
    ax2.set_ylim(0, 1.1)

    plt.tight_layout()
    plt.savefig(os.path.join(SAVE_DIR, filename))
    plt.close()

def run_plots():
    # Process Strong Scaling
    strong_path = os.path.join(RESULTS_DIR, 'strongScalingResults.csv')
    if os.path.exists(strong_path):
        df_strong = pd.read_csv(strong_path)
        create_scaling_plot(df_strong, 'strong_scaling_analysis.png', 'Strong Scaling', is_strong=True)

    # Process Weak Scaling
    weak_path = os.path.join(RESULTS_DIR, 'weakScalingResults.csv')
    if os.path.exists(weak_path):
        df_weak = pd.read_csv(weak_path)
        create_scaling_plot(df_weak, 'weak_scaling_analysis.png', 'Weak Scaling', is_strong=False)

if __name__ == "__main__":
    run_plots()