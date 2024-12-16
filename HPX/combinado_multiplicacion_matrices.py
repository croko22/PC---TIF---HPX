import matplotlib.pyplot as plt
import os
import numpy as np

# Datos para 4 hilos
n_4hilos = [256, 512, 1024, 2048, 4096]
HPX_4hilos = [0.0214126, 0.1704340, 1.4263700, 26.9064000, 316.9850000]
OpenMP_4hilos = [0.0180000, 0.1270000, 1.1880000, 22.5480000, 495.4140000]

# Datos para 8 hilos
n_8hilos = [256, 512, 1024, 2048, 4096]
HPX_8hilos = [0.0211468, 0.176334, 1.42856, 16.6652, 268.44]
OpenMP_8hilos = [0.017, 0.137, 0.805, 18.87, 527.289]

output_dir = 'resultados_graficos'
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

plt.figure(figsize=(10, 6))

plt.plot(n_4hilos, HPX_4hilos, label="HPX 4 hilos", marker='o', color='tab:blue', linestyle='-', linewidth=2)
plt.plot(n_4hilos, OpenMP_4hilos, label="OpenMP 4 hilos", marker='x', color='tab:orange', linestyle='--', linewidth=2)
plt.plot(n_8hilos, HPX_8hilos, label="HPX 8 hilos", marker='o', color='tab:green', linestyle='-', linewidth=2)
plt.plot(n_8hilos, OpenMP_8hilos, label="OpenMP 8 hilos", marker='x', color='tab:red', linestyle='--', linewidth=2)

plt.xscale('log')
plt.yscale('log')

plt.xticks([256, 512, 1024, 2048, 4096], ['256', '512', '1024', '2048', '4096'])

plt.yticks([0.01, 0.1, 1, 10, 100, 1000, 10000, 100000], ['0.01', '0.1', '1', '10', '100', '1000', '10000', '100000'])

plt.xlabel('Tamaño de la matriz (n)', fontsize=12)
plt.ylabel('Tiempo de ejecución (segundos)', fontsize=12)
plt.title('Multiplicación de matrices: HPX vs OpenMP', fontsize=14)

plt.legend()

plt.grid(True, which="both", ls="--")

plt.tight_layout()

plt.savefig(os.path.join(output_dir, 'combinado_multiplicacion_matrices.png'))

plt.show()