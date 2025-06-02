# Eficiencia mecanismos de sincronización

Exploración y análisis del comportamiento de diferentes mecanismos de sincronización en un problema de concurrencia, evaluando su eficiencia bajo distintas condiciones de ejecución.

## Descripción del problema

### Procesamiento de transacciones bancarias

Se simula un sistema bancario donde múltiples cuentas reciben operaciones financieras (depósitos, retiros y transferencias) distribuidas a lo largo de varios días. El objetivo es procesar concurrentemente estas transacciones, garantizando consistencia y orden diario, para evaluar diferentes mecanismos de sincronización (mutexes, semáforos, barreras, etc.).

### Formato del archivo de entrada

El archivo contiene primero la información de las cuentas bancarias, seguida de los bloques de transacciones organizadas por día:

1. Una línea con el número de cuentas.
2. `n` líneas, cada una con:

   ```
   <id_cuenta> <saldo_inicial>
   ```

3. Una línea con el número total de días con transacciones (`D`).
4. Para cada uno de los `D` días:

   - Una línea con el número de transacciones de ese día (`T`).
   - `T` líneas con transacciones del día, con alguno de los siguientes formatos:

     - **Depósito o retiro**:

       ```
       <id_cuenta> <tipo_transaccion> <monto>
       ```

     - **Transferencia**:

       ```
       <id_cuenta_origen> tra <monto> <id_cuenta_destino>
       ```

#### Tipos de transacción:

- `dep`: depósito
- `ret`: retiro
- `tra`: transferencia

#### Ejemplo de entrada:

```
3
1001 2000.32
1002 4034.21
1003 5042.09
3
2
1003 dep 1000.00
1002 ret 200.50
1
1001 dep 500.00
3
1001 ret 100.00
1002 dep 300.00
1001 tra 800.00 1002
```

### Reglas del procesamiento:

- Las transacciones deben ejecutarse **en orden por día**.
- Todas las transacciones de un día deben completarse **antes** de procesar las del día siguiente.
- El sistema debe asegurar que las operaciones concurrentes sobre las cuentas se ejecuten de manera **segura y consistente**, sin condiciones de carrera.
- Al finalizar cada día, debe estar disponible el **balance actualizado** de todas las cuentas.

### Objetivo del experimento

Evaluar y comparar el desempeño de distintos mecanismos de sincronización (como `pthread_mutex`, `pthread_barrier`, `sem_t`, etc.) al procesar estas transacciones en escenarios con distinta carga y concurrencia.
