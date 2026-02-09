import pandas as pd
import matplotlib.pyplot as plt
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
RESULTS_DIR = os.path.join(SCRIPT_DIR, '..', 'results')
SAVE_DIR = os.path.join(SCRIPT_DIR, 'gflops')
X_TICKS = [1, 2, 4, 8, 16, 32, 64, 128]

def plot_gflops():
    if not os.path.exists(SAVE_DIR):
        os.makedirs(SAVE_DIR)

    # --- 1. Strong Scaling GFLOPS ---
    strong_path = os.path.join(RESULTS_DIR, 'strongScalingResults.csv')
    if os.path.exists(strong_path):
        df_s = pd.read_csv(strong_path)
        plt.figure(figsize=(10, 6))
        
        # We ensure a unique line per matrix
        matrices = df_s['matrix'].unique()
        for mat in matrices:
            m_data = df_s[df_s['matrix'] == mat].sort_values('processes')
            m_data = m_data[m_data['processes'].isin(X_TICKS)]
            plt.plot(m_data['processes'], m_data['gflops'], marker='s', linewidth=2, label=f'Mat: {mat}')

        ax = plt.gca()
        ax.set_xscale('log', base=2)
        ax.set_xticks(X_TICKS)
        ax.get_xaxis().set_major_formatter(plt.ScalarFormatter())
        
        plt.xlabel('Number of Processes')
        plt.ylabel('GFLOPS')
        plt.title('Strong Scaling: GFLOPS Throughput')
        
        # Explicitly placing legend
        plt.legend(title="Matrices", loc='best')
        plt.grid(True, which="both", linestyle='--', alpha=0.5)
        
        plt.tight_layout()
        plt.savefig(os.path.join(SAVE_DIR, 'strong_gflops.png'))
        plt.close()

    # --- 2. Weak Scaling GFLOPS ---
    weak_path = os.path.join(RESULTS_DIR, 'weakScalingResults.csv')
    if os.path.exists(weak_path):
        df_w = pd.read_csv(weak_path)
        plt.figure(figsize=(10, 6))
        
        df_w = df_w.sort_values('processes')
        df_w = df_w[df_w['processes'].isin(X_TICKS)]
        
        plt.plot(df_w['processes'], df_w['gflops'], marker='o', color='green', linewidth=2, label='Weak Scaling Trend')

        ax = plt.gca()
        ax.set_xscale('log', base=2)
        ax.set_xticks(X_TICKS)
        ax.get_xaxis().set_major_formatter(plt.ScalarFormatter())
        
        plt.xlabel('Number of Processes')
        plt.ylabel('GFLOPS')
        plt.title('Weak Scaling: GFLOPS Throughput')
        plt.legend(loc='best')
        plt.grid(True, which="both", linestyle='--', alpha=0.5)
        
        plt.tight_layout()
        plt.savefig(os.path.join(SAVE_DIR, 'weak_gflops.png'))
        plt.close()

if __name__ == "__main__":
    plot_gflops()