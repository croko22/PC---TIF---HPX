import pandas as pd
import matplotlib.pyplot as plt
import os

output_dir = 'resultados_graficos'
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

data = pd.read_csv('difusion_calor_resultados_hpx.csv')

fig, ax1 = plt.subplots(figsize=(10, 6))

for iteration in data['iterations'].unique():
    subset = data[data['iterations'] == iteration]
    ax1.plot(subset['N'], subset['execution_time'], marker='o', label=f'Iteraciones={iteration}')
    
ax1.set_xlabel('Tamaño de la rejilla (N) / Número de iteraciones')
ax1.set_ylabel('Tiempo de ejecución (segundos) - N', color='tab:blue')
ax1.set_title('Tiempo de ejecución vs Tamaño de la rejilla y Número de iteraciones (HPX)')
ax1.set_xscale('log')
ax1.set_yscale('log')
ax1.grid(True)
ax1.tick_params(axis='y', labelcolor='tab:blue')

ax2 = ax1.twinx()

for N in data['N'].unique():
    subset = data[data['N'] == N]
    ax2.plot(subset['iterations'], subset['execution_time'], marker='o', label=f'N={N}', linestyle='--')
    
ax2.set_ylabel('Tiempo de ejecución (segundos) - Iteraciones', color='tab:orange')
ax2.set_yscale('log')
ax2.tick_params(axis='y', labelcolor='tab:orange')

fig.tight_layout()

plt.savefig(os.path.join(output_dir, 'difusion_calor.png'))
plt.close()