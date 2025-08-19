# PillPicking System

An automated medicine dispensing application designed for Raspberry Pi with touch screen interface. The system manages 8 medicine boxes and automatically dispenses medications according to predefined schedules, featuring a mobile-phone-like UI built with PySide6.

## Features

- **Automated Medicine Dispensing**: Manages 8 medicine boxes with scheduled dispensing
- **Touch Screen Interface**: Mobile-style UI optimized for touch interaction
- **Schedule Management**: Create, edit, and manage medication schedules
- **Manual Dispensing**: On-demand medication dispensing outside of schedules
- **Real-time Notifications**: Visual and audio alerts for scheduled medications
- **Inventory Tracking**: Real-time pill count monitoring and updates
- **Hardware Integration**: Serial communication with STM32 controller
- **Data Persistence**: SQLite database for reliable data storage
- **Error Handling**: Comprehensive error handling and recovery mechanisms

## System Requirements

### Hardware Requirements

- **Raspberry Pi 4 or newer** (2GB RAM minimum, 4GB recommended)
- **7-inch touch screen display** (800x480 resolution recommended)
- **STM32 controller** for pill dispensing mechanism
- **USB-to-Serial adapter** for STM32 communication
- **MicroSD card** (32GB minimum, Class 10)
- **Power supply** (5V 3A for Raspberry Pi + peripherals)

### Software Requirements

- **Raspberry Pi OS** (Bullseye or newer)
- **Python 3.11** or newer
- **X11 display server** (for GUI)
- **Touch screen drivers** (if not auto-detected)

## Project Structure

```
pillpicking/
├── src/
│   ├── models/              # Data models and structures
│   ├── database/            # Database layer and operations
│   ├── hardware/            # STM32 serial communication
│   ├── ui/                  # PySide6 UI components
│   │   ├── medication_management/  # Medicine inventory UI
│   │   ├── schedule_management/    # Schedule creation/editing UI
│   │   ├── manual_dispensing/      # Manual dispensing UI
│   │   ├── notifications/          # Notification system UI
│   │   └── components/             # Reusable UI components
│   ├── business/            # Business logic layer
│   └── log/               	 # LOG functions and helpers
│   └── utils/               # Utility functions and helpers
├── tests/                   # Unit and integration tests
├── config/                  # Configuration files
├── scripts/                 # Build and deployment scripts
├── data/                    # Application data directory
├── logs/                    # Log files directory
├── main.py                  # Application entry point
├── requirements.txt         # Python dependencies
└── environment.yml          # Conda environment configuration
```

## Installation

### Quick Installation on Raspberry Pi

For Raspberry Pi users, use the automated installation script:

```bash
# Download and run the installation script
curl -sSL https://raw.githubusercontent.com/your-repo/pillpicking/main/scripts/install_raspberry_pi.sh | bash
```

Or manually:

```bash
git clone https://github.com/your-repo/pillpicking.git
cd pillpicking
chmod +x scripts/install_raspberry_pi.sh
./scripts/install_raspberry_pi.sh
```

### Manual Installation

#### Step 1: System Dependencies

**On Raspberry Pi:**
```bash
sudo apt update
sudo apt install -y python3 python3-pip python3-venv git sqlite3 \
    libffi-dev libssl-dev build-essential cmake pkg-config \
    libfreetype6-dev libpng-dev libjpeg-dev libgl1-mesa-glx
```

**On Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install -y python3 python3-pip python3-venv git sqlite3 \
    libffi-dev libssl-dev build-essential cmake pkg-config
```

#### Step 2: Clone Repository

```bash
git clone https://github.com/your-repo/pillpicking.git
cd pillpicking
```

#### Step 3: Environment Setup

**Using Conda (Recommended):**

1. Install Miniconda (if not already installed):
   ```bash
   # For Raspberry Pi (ARM64)
   wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-aarch64.sh
   bash Miniconda3-latest-Linux-aarch64.sh
   
   # For x86_64 Linux
   wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
   bash Miniconda3-latest-Linux-x86_64.sh
   ```

2. Create and activate environment:
   ```bash
   conda env create -f environment.yml
   conda activate pillpicking
   ```

**Using pip:**

1. Create virtual environment:
   ```bash
   python3 -m venv venv
   source venv/bin/activate  # On Windows: venv\Scripts\activate
   ```

2. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```

#### Step 4: Database Initialization

```bash
# The database will be automatically created on first run
# Or manually initialize:
python -c "from src.database.database_manager import DatabaseManager; DatabaseManager().initialize_database()"
```

## Configuration

### Basic Configuration

The application uses configuration files in the `config/` directory:

- `config/.env.example` - Environment variables template
- `config/raspberry_pi.conf` - Raspberry Pi specific settings

Copy and customize the environment file:
```bash
cp config/.env.example config/.env
# Edit config/.env with your settings
```

### Hardware Configuration

#### STM32 Serial Connection

1. Connect STM32 to Raspberry Pi via USB-to-Serial adapter
2. Identify the serial port:
   ```bash
   ls /dev/ttyUSB* /dev/ttyACM*
   ```
3. Update the serial port in configuration:
   ```bash
   # In config/.env or config/raspberry_pi.conf
   SERIAL_PORT=/dev/ttyUSB0
   SERIAL_BAUDRATE=115200
   ```

#### Touch Screen Setup

For official Raspberry Pi touch screen:
```bash
# Enable touch screen in /boot/config.txt
echo "dtoverlay=rpi-display" | sudo tee -a /boot/config.txt
sudo reboot
```

For other touch screens, refer to manufacturer documentation.

## Usage

### Running the Application

#### Development Mode
```bash
# Activate environment
conda activate pillpicking  # or source venv/bin/activate

# Run application
python main.py
```

#### Production Mode (Standalone Executable)
```bash
# Build executable
python scripts/build.py

# Run executable
./deployment/PillPicking
```

### Basic Operations

#### 1. Medication Management
- Access via main menu → "Manage Medications"
- Add/edit medication names and quantities
- Monitor pill counts in real-time
- Update inventory after manual additions

#### 2. Schedule Management
- Access via main menu → "Manage Schedules"
- Create new medication schedules with time and dosage
- Edit existing schedules
- Delete schedules when no longer needed

#### 3. Manual Dispensing
- Access via main menu → "Manual Dispensing"
- Select medication and quantity
- Confirm dispensing action
- Inventory automatically updated

#### 4. Notifications
- Automatic notifications appear at scheduled times
- Shows medication details and countdown
- Acknowledge to dispense or dismiss if not needed

## Development

### Setting Up Development Environment

```bash
# Clone repository
git clone git@github.com:HKCLR2021/PillPicking.git
cd pillpicking

# Create development environment
conda env create -f environment.yml
conda activate pillpicking

# Install development dependencies
pip install -e .
```

### Running Tests

```bash
python -m unittest test.log.main
python -m unittest test.database.main
```

### Code Quality

```bash
# Format code
black src/ tests/

# Lint code
flake8 src/ tests/

# Type checking (if using mypy)
mypy src/
```

### Building for Distribution

```bash
# Clean previous builds
python scripts/build.py --clean

# Build for current platform
python scripts/build.py

# Build for Raspberry Pi (cross-compilation)
python scripts/build.py --platform raspberry_pi
```

## Deployment

### Raspberry Pi Deployment

#### Automatic Startup

The installation script sets up automatic startup. To manually configure:

1. **Using systemd service:**
   ```bash
   sudo cp config/pillpicking.service /etc/systemd/system/
   sudo systemctl enable pillpicking.service
   sudo systemctl start pillpicking.service
   ```

2. **Using desktop autostart:**
   ```bash
   mkdir -p ~/.config/autostart
   cp config/pillpicking.desktop ~/.config/autostart/
   ```

#### Service Management

```bash
# Check service status
sudo systemctl status pillpicking

# View logs
journalctl -u pillpicking -f

# Restart service
sudo systemctl restart pillpicking

# Stop service
sudo systemctl stop pillpicking
```


## Troubleshooting

### Common Issues

#### 1. Application Won't Start

**Symptoms:** Application fails to launch or crashes immediately

**Solutions:**
```bash
# Check Python environment
conda activate pillpicking
python --version

# Check dependencies
pip list | grep PySide6
pip list | grep pyserial

# Check display
echo $DISPLAY
export DISPLAY=:0

# Check permissions
ls -la main.py
chmod +x main.py
```

#### 2. Touch Screen Not Working

**Symptoms:** Touch input not recognized

**Solutions:**
```bash
# Check touch screen detection
xinput list

# Calibrate touch screen
sudo apt install xinput-calibrator
xinput_calibrator

# Check X11 configuration
ls /etc/X11/xorg.conf.d/
```

#### 3. Serial Communication Errors

**Symptoms:** Cannot communicate with STM32

**Solutions:**
```bash
# Check serial ports
ls -la /dev/ttyUSB* /dev/ttyACM*

# Check permissions
sudo usermod -a -G dialout $USER
# Logout and login again

# Test serial connection
python -c "import serial; print(serial.Serial('/dev/ttyUSB0', 115200))"

# Check STM32 connection
dmesg | grep tty
```

#### 4. Database Issues

**Symptoms:** Data not saving or database errors

**Solutions:**
```bash
# Check database file
ls -la data/pillpicking.db

# Check permissions
chmod 664 data/pillpicking.db

# Backup and recreate database
cp data/pillpicking.db data/pillpicking.db.backup
rm data/pillpicking.db
python -c "from src.database.database_manager import DatabaseManager; DatabaseManager().initialize_database()"
```

#### 5. Performance Issues

**Symptoms:** Slow response or high CPU usage

**Solutions:**
```bash
# Check system resources
htop
free -h
df -h

# Optimize for Raspberry Pi
# Edit config/raspberry_pi.conf:
# reduce_animations = true
# cache_size_mb = 25
# max_threads = 1

# Check for memory leaks
ps aux | grep PillPicking
```

### Hardware Setup Issues

#### STM32 Connection

1. **Verify wiring:**
   - STM32 TX → USB-Serial RX
   - STM32 RX → USB-Serial TX
   - Common ground connection

2. **Check STM32 firmware:**
   - Ensure correct baud rate (115200)
   - Verify communication protocol implementation
   - Test with serial terminal first

3. **Power supply:**
   - Ensure adequate power for all components
   - Check voltage levels (3.3V/5V compatibility)

#### Touch Screen Issues

1. **Display not showing:**
   ```bash
   # Check HDMI/DSI connection
   tvservice -s
   
   # Check display configuration
   cat /boot/config.txt | grep display
   ```

2. **Touch not working:**
   ```bash
   # Check input devices
   cat /proc/bus/input/devices
   
   # Test touch events
   evtest /dev/input/event0
   ```

### Log Analysis

#### Application Logs

```bash
# View application logs
tail -f logs/pillpicking.log

# View system logs
journalctl -u pillpicking -f

# View X11 logs
cat ~/.xsession-errors
```

#### Debug Mode

Enable debug logging by setting environment variable:
```bash
export PILLPICKING_LOG_LEVEL=DEBUG
python main.py
```

### Getting Help

1. **Check logs first:** Most issues are logged with helpful error messages
2. **Verify hardware connections:** Many issues are hardware-related
3. **Test components individually:** Isolate the problem area
4. **Check system resources:** Ensure adequate CPU, memory, and storage
5. **Update software:** Keep system and dependencies updated

For additional support:
- Create an issue on GitHub with logs and system information
- Include hardware setup details and error messages
- Provide steps to reproduce the problem

## API Reference

### Core Classes

#### MedicationManager
```python
from src.business.medication_manager import MedicationManager

manager = MedicationManager()
manager.add_medication(box_id=1, name="Aspirin", quantity=30)
manager.dispense_medication(box_id=1, quantity=2)
```

#### ScheduleManager
```python
from src.business.schedule_manager import ScheduleManager

scheduler = ScheduleManager()
scheduler.create_schedule(time="08:00", medications=[...])
```

#### HardwareController
```python
from src.hardware.hardware_controller import HardwareController

controller = HardwareController()
controller.initialize_connection("/dev/ttyUSB0", 115200)
controller.dispense_pills(box_id=1, quantity=2)
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- PySide6 for the GUI framework
- SQLite for reliable data storage
- Raspberry Pi Foundation for the hardware platform
- Contributors and testers who helped improve the system