### Comparación de Frameworks de Programación Paralela: HPX, MPI, Pthreads y OpenMP  

**Integrantes**  
- **Kevin Joaquin Chambi Tapia** (kchambit@unsa.edu.pe)  
- **Juan Diego Huamani Vilca** (jhuamaniv@unsa.edu.pe)  
- **Victor Alejandro Quicaño Miranda** (vquicano@unsa.edu.pe)  
- **Eduardo German Ruiz Mamani** (eruizm@unsa.edu.pe)  
- **Berly Joel Diaz Castro** (bdiazca@unsa.edu.pe)  

**Institución**: Universidad Nacional de San Agustín, Arequipa, Perú  

---

### Descripción  
Este proyecto analiza y compara cuatro frameworks de programación paralela: **HPX**, **MPI**, **Pthreads** y **OpenMP**, evaluando su desempeño en tres aplicaciones:  
1. **Multiplicación de matrices**  
2. **Estimación de π con Monte Carlo**  
3. **Simulación de difusión de calor**  

Se miden métricas como tiempo de ejecución, escalabilidad, uso de memoria y overhead de sincronización para ofrecer una guía sobre el uso más adecuado según el caso de estudio.

---

### Resultados Clave  
- **HPX** se destaca en escalabilidad y eficiencia en entornos distribuidos.  
- **MPI** es robusto pero tiene mayor complejidad.  
- **Pthreads** ofrece control granular, pero es menos escalable.  
- **OpenMP** simplifica el desarrollo, aunque con limitaciones en arquitecturas heterogéneas.  

---

### Estructura del Proyecto  
- **/src**: Código fuente de las implementaciones.  
- **/docs**: Documentación y resultados obtenidos.  
- **/tests**: Casos de prueba para evaluar rendimiento.  