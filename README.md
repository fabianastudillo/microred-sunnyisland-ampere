# Micro-Red Sunny Island & Ampere Square Integration

*[🇺🇸 English Version](README.en.md)*

Este proyecto implementa la integración de inversores **Sunny Island** y **Ampere Square** al sistema SCADA del laboratorio de Micro-Red de la Universidad de Cuenca, utilizando Raspberry Pi como dispositivo intermediario (Gateway) y el protocolo Modbus TCP para la comunicación.

## Descripción del Proyecto

El sistema permite el monitoreo en tiempo real y control de inversores solares mediante:
- **Gateway YASDI** para inversores Sunny Island (CommonShellUIMain.c)
- **Servidor Modbus TCP** para comunicación con el sistema SCADA (Server.py)
- Integración con LabVIEW para visualización y control

### Arquitectura del Sistema

```
[Inversor Sunny Island] ←→ [RS485/USB] ←→ [Raspberry Pi Gateway] ←→ [Modbus TCP] ←→ [Sistema SCADA LabVIEW]
[Inversor Ampere Square] ←→ [Ethernet] ←→ [Red Local] ←→ [Modbus TCP] ←→ [Sistema SCADA LabVIEW]
```

## Componentes Principales

### 1. CommonShellUIMain.c - Gateway YASDI

**Descripción**: Aplicación en C que utiliza la biblioteca YASDI (Yet Another SMA Data Implementation) para comunicarse con inversores Sunny Island mediante protocolo SMA Data. Este archivo está basado en el archivo del mismo nombre encontrado en [libyasdi/shell](https://github.com/pknowledge/libyasdi/tree/main/shell).

**Funcionalidades**:
- Detección automática de dispositivos Sunny Island
- Lectura de canales SPOT (valores en tiempo real)
- Lectura de canales PARAM (parámetros de configuración)
- Escritura de parámetros de configuración
- Generación de archivos de datos para el servidor Modbus

### 2. Server.py - Servidor Modbus TCP

**Descripción**: Servidor Modbus TCP desarrollado en Python que actúa como puente entre los datos del Gateway YASDI y el sistema SCADA.

**Funcionalidades**:
- Servidor Modbus TCP en puerto 502
- Lectura de archivos de datos del Gateway YASDI
- Configuración remota de parámetros
- Logging de eventos y errores
- Manejo de registros de entrada (Input Registers)

## Requisitos del Sistema

### Hardware
- Raspberry Pi (recomendado: RPi 4 o superior)
- Adaptador RS485 a USB para Sunny Island
- Conexión Ethernet para comunicación Modbus TCP
- Inversores Sunny Island y/o Ampere Square

### Software
- **Sistema Operativo**: Raspberry Pi OS (Debian-based)
- **Compilador**: GCC con soporte para C99
- **Python**: Versión 3.7 o superior
- **Bibliotecas**:
  - YASDI library (libYASDI)
  - pyModbusTCP
  - numpy (para procesamiento de datos)

## Instalación y Configuración

### 1. Preparación del Sistema

```bash
# Actualizar el sistema
sudo apt update && sudo apt upgrade -y

# Instalar dependencias del compilador
sudo apt install build-essential gcc make -y

# Instalar Python y pip
sudo apt install python3 python3-pip -y
```

### 2. Instalación de Dependencias Python

```bash
# Instalar bibliotecas de Python requeridas
pip3 install pyModbusTCP numpy

# Crear directorio de trabajo
mkdir -p /home/rpi/Desktop
```

### 3. Configuración de YASDI

La aplicación requiere un archivo `yasdi.ini` para la configuración de drivers. Crear el archivo con el siguiente contenido:

```ini
[DriverModules]
Driver0=yasdi_drv_serial

[COM1]
Device=/dev/ttyUSB0
Protocol=SMANet
Baudrate=1200
```

### 4. Instalación de YASDI (si es necesario)

Si las bibliotecas YASDI no están instaladas, puedes compilarlas desde el código fuente:

```bash
# Clonar la implementación de libyasdi
git clone https://github.com/pknowledge/libyasdi.git
cd libyasdi

# Compilar e instalar YASDI
make
sudo make install

# Actualizar las bibliotecas del sistema
sudo ldconfig
```

### 5. Compilación del Gateway YASDI

```bash
# Compilar CommonShellUIMain.c
gcc -o CommonShellUIMain CommonShellUIMain.c -lyasdi -lyasdimaster -lpthread

# O usar flags específicos si es necesario
gcc -std=c99 -o CommonShellUIMain CommonShellUIMain.c \
    -I/usr/include/yasdi \
    -L/usr/lib/yasdi \
    -lyasdi -lyasdimaster -lpthread
```

**Nota**: Una implementación de referencia de YASDI está disponible en: https://github.com/pknowledge/libyasdi.git

## Uso del Sistema

### 1. Ejecución del Gateway YASDI

```bash
# Ejecutar con detección automática
./CommonShellUIMain yasdi.ini autodetect

# O ejecutar manualmente
./CommonShellUIMain yasdi.ini
```

**Archivos de salida generados**:
- `/home/rpi/Desktop/SPOTCHANNELS.txt` - Valores en tiempo real
- `/home/rpi/Desktop/PARAMCHANNELS.txt` - Parámetros de configuración
- `/home/rpi/Desktop/SetInformation.txt` - Información de configuración
- `/home/rpi/Desktop/LoggYasdiProgram.txt` - Log del programa YASDI

### 2. Ejecución del Servidor Modbus

```bash
# Ejecutar el servidor Modbus TCP
python3 Server.py
```

**Configuración de red**: Editar las direcciones IP en Server.py antes de ejecutar:
```python
# Cambiar "192.168.xxx.xxx" por la IP de la Raspberry Pi
server = ModbusServer("192.168.1.100", 502, no_block=True)
serverAddress = ("192.168.1.100", 5000)
```

### 3. Configuración de Parámetros

Para configurar parámetros del Sunny Island desde el sistema SCADA:

1. **Desde LabVIEW**: Enviar comando TCP al puerto 5000 en formato:
   ```
   xxxxxx;[CHANNEL_ID];[VALUE]
   ```

2. **Ejemplo**: Para configurar el canal 22 con valor 50.5:
   ```
   xxxxxx;22;50.5
   ```

## Archivos de Configuración

### yasdi.ini
Archivo de configuración principal para el driver YASDI. Define los puertos de comunicación y protocolos utilizados.

### Canales Monitoreados

**SPOT Channels (18 canales)**:
- 192, 193, 194: Tensiones de fase
- 202, 206, 210: Corrientes de línea
- 214, 215, 219: Potencias activas
- 236, 237, 238: Frecuencias
- Y otros parámetros de estado

**PARAM Channels (29 canales)**:
- 22, 23, 24, 25, 26: Parámetros de tensión
- 9, 10, 17, 18, 19: Parámetros de corriente
- Y otros parámetros de configuración

## Estructura de Registros Modbus

- **Registros 0-17**: Valores SPOT (tiempo real)
- **Registros 18-46**: Valores PARAM (configuración)
- **Multiplicador**: Todos los valores se multiplican por 100 para preservar decimales

## Logs y Monitoreo

### Archivos de Log
- `/home/rpi/Desktop/LoggModbusServer.log` - Log del servidor Modbus
- `/home/rpi/Desktop/LoggYasdiProgram.txt` - Log del programa YASDI

### Verificación del Estado
```bash
# Verificar procesos en ejecución
ps aux | grep -E "(CommonShell|Server_V7)"

# Verificar puertos abiertos
netstat -tulpn | grep -E "(502|5000)"

# Verificar archivos de datos
ls -la /home/rpi/Desktop/*.txt
```

## Solución de Problemas

### Problemas Comunes

1. **Error de permisos en /dev/ttyUSB0**:
   ```bash
   sudo chmod 666 /dev/ttyUSB0
   # O agregar usuario al grupo dialout
   sudo usermod -a -G dialout $USER
   ```

2. **Bibliotecas YASDI no encontradas**:
   - Verificar instalación de libYASDI
   - Añadir rutas al LD_LIBRARY_PATH si es necesario

3. **Error de conexión Modbus**:
   - Verificar configuración de IP
   - Comprobar firewall y reglas iptables
   - Verificar que el puerto 502 esté disponible

4. **Archivos de datos vacíos**:
   - Verificar comunicación RS485 con el inversor
   - Comprobar configuración yasdi.ini
   - Revisar logs para errores de detección de dispositivos

### Depuración

```bash
# Verificar comunicación serie
dmesg | grep ttyUSB

# Probar comunicación básica
echo "test" > /dev/ttyUSB0

# Monitorear logs en tiempo real
tail -f /home/rpi/Desktop/LoggModbusServer.log
tail -f /home/rpi/Desktop/LoggYasdiProgram.txt
```

## Contribución

Este proyecto forma parte del trabajo de titulación "Integración de los inversores Sunny Island y Ampere Square al sistema de monitoreo SCADA existente en el laboratorio de Micro-Red de la Universidad de Cuenca".

### Autores
- Desarrollado en el contexto del laboratorio de Micro-Red
- Universidad de Cuenca
- Basado en la biblioteca YASDI de SMA Solar Technology AG
- **CommonShellUIMain.c** adaptado de la implementación original en [pknowledge/libyasdi](https://github.com/pknowledge/libyasdi/)

## Licencia

- **CommonShellUIMain.c**: Basado en YASDI (LGPL 2.1) - Adaptado del archivo original en [libyasdi/shell](https://github.com/pknowledge/libyasdi/tree/main/shell)
- **Server.py**: Código desarrollado para el proyecto de titulación

## Referencias

- [SMA Solar Technology - YASDI](https://www.sma.de)
- [libyasdi - YASDI Library Implementation](https://github.com/pknowledge/libyasdi.git)
- Documentación de Modbus TCP/IP
- Trabajo de Titulación: "Integración de inversores al sistema SCADA"