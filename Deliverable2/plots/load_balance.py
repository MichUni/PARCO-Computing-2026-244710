import pandas as pd
import matplotlib.pyplot as plt
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
RESULTS_FILE = os.path.join(SCRIPT_DIR, '..', 'results', 'strongScalingResults.csv')
SAVE_DIR = os.path.join(SCRIPT_DIR, 'load_balance')
X_TICKS = [1, 2, 4, 8, 16, 32, 64, 128]

def plot_load():
    if not os.path.exists(SAVE_DIR):
        os.makedirs(SAVE_DIR)

    df = pd.read_csv(RESULTS_FILE)
    plt.figure(figsize=(10, 6))
    
    matrices = df['matrix'].unique()
    for mat in matrices:
        m_data = df[df['matrix'] == mat].sort_values('processes')
        # Filter for your specific scaling steps
        m_data = m_data[m_data['processes'].isin(X_TICKS)]
        
        plt.plot(m_data['processes'], m_data['avg_non_zero_values'], 
                 marker='o', linewidth=2, label=f'Mat: {mat}')

    # Formatting
    ax = plt.gca()
    ax.set_xscale('log', base=2)
    # Using log scale for Y as well is often helpful for NNZ scaling 
    # but I'll keep it linear unless you prefer log-log.
    ax.set_xticks(X_TICKS)
    ax.get_xaxis().set_major_formatter(plt.ScalarFormatter())

    plt.xlabel('Number of Processes')
    plt.ylabel('Average NNZ per Rank')
    plt.title('Strong Scaling: Average Workload Distribution')
    plt.legend(title="Matrices", loc='best')
    plt.grid(True, which="both", linestyle='--', alpha=0.5)
    
    plt.tight_layout()
    plt.savefig(os.path.join(SAVE_DIR, 'average_nnz_per_rank.png'))
    plt.close()

if __name__ == "__main__":
    plot_load()