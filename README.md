# NanoRadar_MR72
NanoRadar MR72
Basic calling for NanoRadar_MR72

These drivers use Uart to communicate, 4 pins are required to interface.
### MR72 Mr72(MRUART72); //Define MR72 to class Mr72
```
  Mr72.IncomingNewMessage(); //Call IncomingNewMessage From Mr72

  Mr72.Distance_Uart_sl; //Left Value
  Mr72.Distance_Uart_sc; //Center Value
  Mr72.Distance_Uart_sr; //Right Value
```
```
[env:teensy40]
platform = teensy
board = teensy40
framework = arduino
```
