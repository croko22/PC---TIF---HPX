import pandas as pd
import matplotlib.pyplot as plt
import os

# Crear carpeta para guardar las imágenes
output_dir = 'resultados_graficos'
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

def plot_combined(csv_files, titles, output_file):
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20, 8))

    for csv_file, title in zip(csv_files, titles):
        data = pd.read_csv(csv_file)

        # Gráfico de tiempo de ejecución
        ax1.plot(data['num_points'], data['execution_time'], marker='o', linestyle='-', label=title)
        ax1.set_xlabel('Número de Puntos')
        ax1.set_ylabel('Tiempo de Ejecución (segundos)')
        ax1.set_xscale('log')
        ax1.set_yscale('log')
        ax1.legend()
        ax1.grid(True)

        # Gráfico de precisión
        ax2.plot(data['num_points'], data['precision'], marker='x', linestyle='--', label=title)
        ax2.axhline(y=0, color='r', linestyle='--', label='Valor Real de π')
        ax2.set_xlabel('Número de Puntos')
        ax2.set_ylabel('Precisión')
        ax2.set_xscale('log')
        ax2.legend()
        ax2.grid(True)

    ax1.set_title('Tiempo de Ejecución vs Número de Puntos')
    ax2.set_title('Precisión vs Número de Puntos')

    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, output_file))
    plt.close()

# Graficar y guardar resultados combinados para 4, 8 y 16 hilos en una sola imagen
plot_combined(
    ['pi_4_threads.csv', 'pi_8_threads.csv', 'pi_16_threads.csv'],
    ['4 Hilos', '8 Hilos', '16 Hilos'],
    'pi_combined.png'
)