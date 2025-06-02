# Medicion-Eficiencia-Mecanismos-Sincronizacion

Exploración y análisis de comportamiento de diferentes mecanismos de sincronización en un problema de concurrencia, evaluando eficiencia bajo distintas condiciones de ejecución

## Descripción del problema

### Procesamiento de Transacciones Bancarias

Dado un archivo de texto que contien un listado de transacciones bancarias par distintas cuentas. Con el siguiente formato

- N lineas de cuentas a procesar
- ID_CUENTA SALDO_INICIAL
- ID_CUENTA TIPO_TRANSACCION MONTO <ID_CUENTA>

  donde:

  - `ID_CUENTA`: Entero que identifica la cuenta bancaria (ej: `12345` o `ACC001`)
  - `TIPO_TRANSACCION`: puede ser `DEP` para depósito, `RET` para retiro o `TRA` para transeferencia entre una cuenta a otra
  - `MONTO`: cantidad numérica (float o entero) que indica cuánto se deposita, retirar o transferir
  - `ID_CUENTA`: Entero que identifica la cuenta hacia donde se va a transferir (solo si el tipo de transacción es transferencia)

- Ejemplo

  ```
  3
  1001 2000.32
  1002 4034.21
  1003 5042.09
  6
  1001 DEP 500.00
  1002 RET 200.50
  1001 RET 100.00
  1003 DEP 1000.00
  1002 DEP 300.00
  1001 TRA 800.00 1002
  ```

Procesar todas las transacciones y actualizar el saldo de cada cuenta

Al finalizar obtener el saldo final de cada cuenta
