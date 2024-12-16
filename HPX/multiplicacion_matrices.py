import matplotlib.pyplot as plt
import os

output_dir = 'resultados_graficos'
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

n_values = [256, 512, 1024, 2048, 4096]
hpx_4_threads = [0.0214126, 0.1704340, 1.4263700, 26.9064000, 316.9850000]
hpx_8_threads = [0.0211468, 0.176334, 1.42856, 16.6652, 268.44]

plt.figure(figsize=(10, 6))

plt.plot(n_values, hpx_4_threads, marker='o', label='HPX (4 hilos)')
plt.plot(n_values, hpx_8_threads, marker='o', label='HPX (8 hilos)')

plt.title('Tiempos de Ejecución de HPX para la Multiplicación de Matrices', fontsize=14)
plt.xlabel('Tamaño de la Matriz (N)', fontsize=12)
plt.ylabel('Tiempo de Ejecución (segundos)', fontsize=12)
plt.xlim(0, 5000)
plt.ylim(0, 350)
plt.xticks(range(0, 5001, 500))
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend(fontsize=12)
plt.tight_layout()

output_file = os.path.join(output_dir, 'multiplicacion_matrices.png')
plt.savefig(output_file)
plt.close()