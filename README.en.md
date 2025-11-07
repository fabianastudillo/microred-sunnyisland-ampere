# Micro-Grid Sunny Island & Ampere Square Integration

*[🇪🇸 Versión en Español](README.md)*

This project implements the integration of **Sunny Island** and **Ampere Square** inverters into the SCADA system of the Micro-Grid laboratory at the University of Cuenca, using Raspberry Pi as an intermediary device (Gateway) and Modbus TCP protocol for communication.

## Project Description

The system enables real-time monitoring and control of solar inverters through:
- **YASDI Gateway** for Sunny Island inverters (CommonShellUIMain.c)
- **Modbus TCP Server** for SCADA system communication (Server_V7.py)
- LabVIEW integration for visualization and control

### System Architecture

```
[Sunny Island Inverter] ←→ [RS485/USB] ←→ [Raspberry Pi Gateway] ←→ [Modbus TCP] ←→ [LabVIEW SCADA System]
[Ampere Square Inverter] ←→ [Ethernet] ←→ [Local Network] ←→ [Modbus TCP] ←→ [LabVIEW SCADA System]
```

## Main Components

### 1. CommonShellUIMain.c - YASDI Gateway

**Description**: C application that uses the YASDI library (Yet Another SMA Data Implementation) to communicate with Sunny Island inverters via SMA Data protocol.

**Features**:
- Automatic detection of Sunny Island devices
- Reading SPOT channels (real-time values)
- Reading PARAM channels (configuration parameters)
- Writing configuration parameters
- Data file generation for Modbus server

### 2. Server_V7.py - Modbus TCP Server

**Description**: Modbus TCP server developed in Python that acts as a bridge between YASDI Gateway data and the SCADA system.

**Features**:
- Modbus TCP server on port 502
- Reading data files from YASDI Gateway
- Remote parameter configuration
- Event and error logging
- Input register handling

## System Requirements

### Hardware
- Raspberry Pi (recommended: RPi 4 or higher)
- RS485 to USB adapter for Sunny Island
- Ethernet connection for Modbus TCP communication
- Sunny Island and/or Ampere Square inverters

### Software
- **Operating System**: Raspberry Pi OS (Debian-based)
- **Compiler**: GCC with C99 support
- **Python**: Version 3.7 or higher
- **Libraries**:
  - YASDI library (libYASDI)
  - pyModbusTCP
  - numpy (for data processing)

## Installation and Configuration

### 1. System Preparation

```bash
# Update system
sudo apt update && sudo apt upgrade -y

# Install compiler dependencies
sudo apt install build-essential gcc make -y

# Install Python and pip
sudo apt install python3 python3-pip -y
```

### 2. Python Dependencies Installation

```bash
# Install required Python libraries
pip3 install pyModbusTCP numpy

# Create working directory
mkdir -p /home/rpi/Desktop
```

### 3. YASDI Configuration

The application requires a `yasdi.ini` file for driver configuration. Create the file with the following content:

```ini
[DriverModules]
Driver0=yasdi_drv_serial

[COM1]
Device=/dev/ttyUSB0
Protocol=SMANet
Baudrate=1200
```

### 4. YASDI Installation (if needed)

If YASDI libraries are not installed, you can compile them from source code:

```bash
# Clone the libyasdi implementation
git clone https://github.com/pknowledge/libyasdi.git
cd libyasdi

# Compile and install YASDI
make
sudo make install

# Update system libraries
sudo ldconfig
```

### 5. YASDI Gateway Compilation

```bash
# Compile CommonShellUIMain.c
gcc -o CommonShellUIMain CommonShellUIMain.c -lyasdi -lyasdimaster -lpthread

# Or use specific flags if needed
gcc -std=c99 -o CommonShellUIMain CommonShellUIMain.c \
    -I/usr/include/yasdi \
    -L/usr/lib/yasdi \
    -lyasdi -lyasdimaster -lpthread
```

**Note**: A reference implementation of YASDI is available at: https://github.com/pknowledge/libyasdi.git

## System Usage

### 1. Running the YASDI Gateway

```bash
# Run with automatic detection
./CommonShellUIMain yasdi.ini autodetect

# Or run manually
./CommonShellUIMain yasdi.ini
```

**Generated output files**:
- `/home/rpi/Desktop/SPOTCHANNELS.txt` - Real-time values
- `/home/rpi/Desktop/PARAMCHANNELS.txt` - Configuration parameters
- `/home/rpi/Desktop/SetInformation.txt` - Configuration information
- `/home/rpi/Desktop/LoggYasdiProgram.txt` - YASDI program log

### 2. Running the Modbus Server

```bash
# Run Modbus TCP server
python3 Server_V7.py
```

**Network configuration**: Edit IP addresses in Server_V7.py before running:
```python
# Change "192.168.xxx.xxx" to Raspberry Pi IP
server = ModbusServer("192.168.1.100", 502, no_block=True)
serverAddress = ("192.168.1.100", 5000)
```

### 3. Parameter Configuration

To configure Sunny Island parameters from the SCADA system:

1. **From LabVIEW**: Send TCP command to port 5000 in format:
   ```
   xxxxxx;[CHANNEL_ID];[VALUE]
   ```

2. **Example**: To configure channel 22 with value 50.5:
   ```
   xxxxxx;22;50.5
   ```

## Configuration Files

### yasdi.ini
Main configuration file for YASDI driver. Defines communication ports and protocols used.

### Monitored Channels

**SPOT Channels (18 channels)**:
- 192, 193, 194: Phase voltages
- 202, 206, 210: Line currents
- 214, 215, 219: Active powers
- 236, 237, 238: Frequencies
- And other status parameters

**PARAM Channels (29 channels)**:
- 22, 23, 24, 25, 26: Voltage parameters
- 9, 10, 17, 18, 19: Current parameters
- And other configuration parameters

## Modbus Register Structure

- **Registers 0-17**: SPOT values (real-time)
- **Registers 18-46**: PARAM values (configuration)
- **Multiplier**: All values are multiplied by 100 to preserve decimals

## Logging and Monitoring

### Log Files
- `/home/rpi/Desktop/LoggModbusServer.log` - Modbus server log
- `/home/rpi/Desktop/LoggYasdiProgram.txt` - YASDI program log

### Status Verification
```bash
# Check running processes
ps aux | grep -E "(CommonShell|Server_V7)"

# Check open ports
netstat -tulpn | grep -E "(502|5000)"

# Check data files
ls -la /home/rpi/Desktop/*.txt
```

## Troubleshooting

### Common Issues

1. **Permission error on /dev/ttyUSB0**:
   ```bash
   sudo chmod 666 /dev/ttyUSB0
   # Or add user to dialout group
   sudo usermod -a -G dialout $USER
   ```

2. **YASDI libraries not found**:
   - Verify libYASDI installation
   - Add paths to LD_LIBRARY_PATH if necessary

3. **Modbus connection error**:
   - Verify IP configuration
   - Check firewall and iptables rules
   - Verify port 502 is available

4. **Empty data files**:
   - Verify RS485 communication with inverter
   - Check yasdi.ini configuration
   - Review logs for device detection errors

### Debugging

```bash
# Check serial communication
dmesg | grep ttyUSB

# Test basic communication
echo "test" > /dev/ttyUSB0

# Monitor logs in real-time
tail -f /home/rpi/Desktop/LoggModbusServer.log
tail -f /home/rpi/Desktop/LoggYasdiProgram.txt
```

## Quick Setup Commands

```bash
# Complete initial setup
make setup

# Start complete system
./startup.sh start

# Check status
./startup.sh status

# Stop system
./startup.sh stop
```

## Automated Scripts

The project includes several automated scripts for easy management:

### Makefile Targets
- `make build` - Compile YASDI Gateway
- `make setup` - Complete system setup
- `make install` - Install files to destination directory
- `make test` - Run basic tests
- `make clean` - Clean compiled files

### Startup Script
- `./startup.sh start` - Start complete system
- `./startup.sh stop` - Stop complete system
- `./startup.sh restart` - Restart system
- `./startup.sh status` - Show system status

## Contributing

This project is part of the thesis work "Integration of Sunny Island and Ampere Square inverters into the existing SCADA monitoring system in the Micro-Grid laboratory of the University of Cuenca".

### Authors
- Developed in the context of the Micro-Grid laboratory
- University of Cuenca
- Based on YASDI library by SMA Solar Technology AG

## License

- **CommonShellUIMain.c**: Based on YASDI (LGPL 2.1)
- **Server.py**: Code developed for the thesis project

## References

- [SMA Solar Technology - YASDI](https://www.sma.de)
- [libyasdi - YASDI Library Implementation](https://github.com/pknowledge/libyasdi.git)
- Modbus TCP/IP Documentation
- Thesis Work: "Integration of inverters into SCADA system"

## File Structure

```
microred-sunnyisland-ampere/
├── README.md                 # Spanish documentation
├── README.en.md             # English documentation
├── CommonShellUIMain.c      # YASDI Gateway (C application)
├── Server.py            # Modbus TCP Server (Python)
├── ReadTextFile.py         # Text processing module (Python)
├── yasdi.ini               # YASDI configuration file
├── Makefile                # Build automation
├── startup.sh              # System startup script
└── TT-AraujoTigre/         # Thesis LaTeX documentation
    ├── main.tex
    ├── sections/
    ├── fig/
    └── ...
```

## Support

For technical support or questions about this project:
1. Check the troubleshooting section
2. Review log files for error messages
3. Consult the thesis documentation in `TT-AraujoTigre/` folder
4. Verify all dependencies are properly installed