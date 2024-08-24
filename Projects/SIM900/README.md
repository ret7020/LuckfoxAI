# Lite SIM900 lib
Connect to Internet via sim900 UART module. Connection schematic:

Luckfox have a 3V logic, so you can connect it to module without voltage divider or logic level converters.
```
SIM900 PIN | Luckfox

RX -> TX_UART_4
TX -> RX_UART_4
GND -> GND
```

Use external power-supply to run module

Enable uart4 via config: 

```
Add `UART4_M1_STATUS=1` to file: /etc/luckfox.cfg 
```

Or `luckfox-config` -> Advanced Options -> UART
