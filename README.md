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
