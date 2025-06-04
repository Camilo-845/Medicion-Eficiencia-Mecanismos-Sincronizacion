# Evaluación de mecanismos de sincronización

## Descripción general

Este proyecto plantea un problema clásico de concurrencia orientado a analizar el comportamiento y la eficiencia de distintos mecanismos de sincronización (como _mutexes_, _semáforos_ y _barreras_) en un entorno de acceso compartido a recursos.

## Problema: procesamiento concurrente de transacciones bancarias

Se simula un sistema bancario en el que múltiples hilos procesan en paralelo una lista de operaciones financieras (depósitos y retiros) sobre un conjunto de cuentas. Cada transacción afecta el saldo de una única cuenta, por lo que el acceso concurrente a los datos debe estar correctamente sincronizado para evitar condiciones de carrera, garantizar la integridad de los saldos y mantener la coherencia global del sistema.

## Propósito del experimento

El objetivo es comparar el rendimiento y la escalabilidad de diferentes mecanismos de sincronización en un entorno controlado con concurrencia real, evaluando:

- **Rendimiento absoluto**: tiempo total de procesamiento bajo diferentes niveles de carga y número de hilos.
- **Sobrecarga de sincronización**: impacto de cada técnica en la eficiencia general.
- **Escalabilidad**: comportamiento al aumentar la cantidad de cuentas, transacciones y hilos.

Este problema es adecuado para este análisis porque:

- Involucra operaciones concurrentes sobre recursos compartidos (cuentas bancarias).
- Tiene una estructura simple y fácilmente paralelizable.
- Permite medir claramente los efectos de sincronización sin ruido externo.
- Es fácilmente escalable en volumen y complejidad de transacciones.

## Requerimientos

- **Sistema operativo:** Linux (recomendado)
- **Compilador:** GCC (GNU Compiler Collection)
- **Herramientas:** Make (opcional, para automatizar la compilación)
- **Librerías:** pthreads (normalmente incluida en sistemas Linux)

## Instrucciones de compilación y ejecución

Todos los archivos fuente se encuentran en la carpeta “src”. Puedes compilar y ejecutar cualquiera de las versiones del código (por ejemplo, “main.c”, “main_v2.c”, “main_v3.c”) de la siguiente manera.

### Compilación manual con gcc

Ejemplo para compilar “main.c” (o “main_v2.c”, “main_v3.c”):

```bash
gcc -o main src/main.c -lpthread
```

### Ejecución

El programa se ejecuta con la sintaxis:

```bash
./<ejecutable> <archivo> <nHilos>
```

Por ejemplo, para ejecutar “main” con el archivo de prueba “c_5t_20.txt” (ubicado en la carpeta “test_files”) y 5 hilos, ejecuta:

```bash
./main test_files/c_5t_20.txt 5
```

### Compilación usando Make (opcional)

Si tienes un archivo “Makefile” (generado en la raíz del proyecto), puedes compilar todas las versiones (por ejemplo, “main”, “main_v2”, “main_v3”) con:

```bash
make
```

Esto generará los ejecutables “main”, “main_v2” y “main_v3” (según el contenido de “src”). Luego, ejecuta el programa con la sintaxis “./<ejecutable> <archivo> <nHilos>” (por ejemplo, “./main test_files/c_5t_20.txt 5”).
