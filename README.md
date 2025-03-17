# LossManus - Enhanced Loss Prevention Log

![M5Stack CoreS3](https://img.shields.io/badge/Hardware-M5Stack%20CoreS3-blue)
![Version](https://img.shields.io/badge/Version-1.0.0-green)
![PlatformIO](https://img.shields.io/badge/Framework-PlatformIO-orange)
![LVGL](https://img.shields.io/badge/UI-LVGL%208.4.0-purple)

An advanced loss prevention logging system designed for the M5Stack CoreS3 hardware platform. This application provides a comprehensive solution for retail and inventory management with a focus on loss prevention tracking.

## Features

- **Intuitive Touch Interface**: User-friendly UI built with LVGL 8.4.0
- **Comprehensive Logging**: Track gender, color, and item details for loss prevention incidents
- **Offline Operation**: Full functionality even without network connectivity
- **Data Synchronization**: Sync logs with remote servers when connected
- **Power Management**: Optimized for battery life with power-saving features
- **Secure Storage**: Local database with backup and recovery options
- **Customizable Settings**: Adjust brightness, volume, and other preferences

## Hardware Requirements

- M5Stack CoreS3 device
- SD Card (optional, for extended storage)
- WiFi connectivity (optional, for synchronization)

## Software Architecture

The project follows a modular architecture with clear separation of concerns:

1. **Hardware Abstraction Layer (HAL)**: Isolates hardware-specific code
2. **Data Management Layer**: Handles storage, retrieval, and processing
3. **User Interface Layer**: Provides responsive and intuitive UI
4. **Application Layer**: Implements core application logic
5. **Connectivity Layer**: Manages external communications

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) with Visual Studio Code
- M5Stack CoreS3 device

### Installation

1. Clone this repository:
   ```
   git clone https://github.com/ReptilePvP/LossManus.git
   ```

2. Open the project in PlatformIO:
   ```
   cd LossManus
   code .
   ```

3. Build and upload to your M5Stack CoreS3:
   - Connect your M5Stack CoreS3 via USB
   - Click the PlatformIO upload button or run:
     ```
     pio run -t upload
     ```

### Configuration

The main configuration options are available in `src/config.h`. You can customize:

- Debug settings
- Hardware parameters
- Power management options
- Data storage configuration
- WiFi and connectivity settings
- UI preferences

## Usage

1. **Power on** your M5Stack CoreS3 device
2. Navigate through the **main menu** using the touch interface
3. Select options for **gender**, **color**, and **item** details
4. **Confirm** entries to save them to the local database
5. **Sync** data with remote servers when connected to WiFi
6. View and manage **logs** through the logs screen
7. Adjust **settings** as needed

## Project Structure

```
LossManus/
├── src/                          # Source code
│   ├── Manus.ino                 # Main entry point
│   ├── config.h                  # Configuration parameters
│   ├── version.h                 # Version information
│   ├── architecture.md           # Architecture documentation
│   └── [Various modules]         # Organized by functionality
├── include/                      # Header files
├── lib/                          # External libraries
├── test/                         # Test files
└── platformio.ini                # PlatformIO configuration
```

## Dependencies

- [M5CoreS3](https://github.com/m5stack/M5CoreS3) - Core library for M5Stack CoreS3
- [M5Unified](https://github.com/m5stack/M5Unified) - Unified library for M5Stack devices
- [M5GFX](https://github.com/m5stack/M5GFX) - Graphics library for M5Stack
- [LVGL](https://github.com/lvgl/lvgl) - Light and Versatile Graphics Library
- [ArduinoJson](https://arduinojson.org/) - JSON handling for Arduino
- [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO) - I/O library for various buses

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- M5Stack for the excellent CoreS3 hardware
- LVGL team for the powerful UI library
- All contributors to the project

## Contact

For questions or support, please open an issue on the GitHub repository.
