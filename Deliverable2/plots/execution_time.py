import pandas as pd
import matplotlib.pyplot as plt
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
RESULTS_DIR = os.path.join(SCRIPT_DIR, '..', 'results')
# Target subfolder
SAVE_DIR = os.path.join(SCRIPT_DIR, 'comm_vs_comp_time')

def plot_bars():
    if not os.path.exists(SAVE_DIR):
        os.makedirs(SAVE_DIR)

    df_s = pd.read_csv(os.path.join(RESULTS_DIR, 'strongScalingResults.csv'))
    df_w = pd.read_csv(os.path.join(RESULTS_DIR, 'weakScalingResults.csv'))
    
    # Strong Scaling
    mats = df_s['matrix'].unique()
    fig, axes = plt.subplots(1, len(mats), figsize=(22, 6))
    if len(mats) == 1: axes = [axes]
    
    for i, mat in enumerate(mats):
        data = df_s[df_s['matrix'] == mat].sort_values('processes')
        axes[i].bar(data['processes'].astype(str), data['max_computation(ms)'], label='Comp.', color='#2c3e50')
        axes[i].bar(data['processes'].astype(str), data['max_commutation(ms)'], 
                    bottom=data['max_computation(ms)'], label='Comm.', color='#3498db')
        axes[i].set_title(mat)
        axes[i].set_xlabel('Processes')
        if i == 0: axes[i].set_ylabel('Time (ms)')
        axes[i].legend()
    
    plt.tight_layout()
    plt.savefig(os.path.join(SAVE_DIR, 'strong_scaling_bars.png'))
    plt.close()

    # Weak Scaling
    plt.figure(figsize=(10, 6))
    plt.bar(df_w['processes'].astype(str), df_w['max_computation(ms)'], label='Comp.', color='#2c3e50')
    plt.bar(df_w['processes'].astype(str), df_w['max_commutation(ms)'], 
            bottom=df_w['max_computation(ms)'], label='Comm.', color='#3498db')
    plt.title('Weak Scaling Performance')
    plt.ylabel('Time (ms)')
    plt.legend()
    plt.savefig(os.path.join(SAVE_DIR, 'weak_scaling_bars.png'))
    plt.close()

if __name__ == "__main__":
    plot_bars()