import matplotlib.pyplot as plt
import numpy as np
import os

puntos = [1_000, 10_000, 100_000, 1_000_000]

tiempo_4_hilos = [0.000316233, 0.002951428, 0.027384781, 0.306476599]
precision_4_hilos = [3.084, 3.1272, 3.14976, 3.143064]

tiempo_8_hilos = [0.000284205, 0.002746196, 0.030816369, 0.282416686]
precision_8_hilos = [3.14, 3.15, 3.1478, 3.141992]

tiempo_16_hilos = [0.000326203, 0.002980446, 0.027168185, 0.305298247]
precision_16_hilos = [3.144, 3.1512, 3.14328, 3.139464]

os.makedirs('resultados_graficos', exist_ok=True)

fig, axs = plt.subplots(1, 2, figsize=(15, 6))

axs[0].plot(puntos, tiempo_4_hilos, marker='o', label='4 hilos')
axs[0].plot(puntos, tiempo_8_hilos, marker='o', label='8 hilos')
axs[0].plot(puntos, tiempo_16_hilos, marker='o', label='16 hilos')
axs[0].set_xscale('log')
axs[0].set_yscale('log')
axs[0].set_title('Número de puntos vs Tiempo de ejecución')
axs[0].set_xlabel('Número de puntos')
axs[0].set_ylabel('Tiempo de ejecución (segundos)')
axs[0].grid(True, which="both", linestyle='--', linewidth=0.5)
axs[0].legend()

axs[1].plot(puntos, precision_4_hilos, marker='o', label='4 hilos')
axs[1].plot(puntos, precision_8_hilos, marker='o', label='8 hilos')
axs[1].plot(puntos, precision_16_hilos, marker='o', label='16 hilos')
axs[1].set_xscale('log')
axs[1].set_title('Número de puntos vs Precisión')
axs[1].set_xlabel('Número de puntos')
axs[1].set_ylabel('Precisión (valor de Pi estimado)')
axs[1].grid(True, which="both", linestyle='--', linewidth=0.5)
axs[1].axhline(y=np.pi, color='r', linestyle='--', label='Valor real de Pi')
axs[1].legend()

plt.tight_layout()
plt.savefig('resultados_graficos/montecarlo.png')

plt.show()