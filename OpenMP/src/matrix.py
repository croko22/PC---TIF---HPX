import pandas as pd
import matplotlib.pyplot as plt
import os

# Crear carpeta para guardar las imágenes
output_dir = 'resultados_graficos'
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

def plot_combined(csv_files, titles, output_file):
    plt.figure(figsize=(12, 8))

    for csv_file, title in zip(csv_files, titles):
        data = pd.read_csv(csv_file)
        plt.plot(data['N'], data['Time'], marker='o', linestyle='-', label=title)

    plt.title('Multiplicación de Matrices - Tiempo de Ejecución')
    plt.xlabel('Tamaño de la Matriz (N)')
    plt.ylabel('Tiempo de Ejecución (segundos)')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(output_dir, output_file))
    plt.close()

# Graficar y guardar resultados combinados para 4 y 8 hilos en una sola imagen
plot_combined(
    ['matrix_times_4_threads.csv', 'matrix_times_8_threads.csv'],
    ['4 Hilos', '8 Hilos'],
    'matrix_times_combined.png'
)