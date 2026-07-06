# STM32F103C8T6 PWM Motor Control Project

A complete STM32F103C8T6 (Blue Pill) based motor control project featuring dual PWM control, I2C communication, and USART UART for serial communication.

## 📋 Table of Contents

- [Features](#features)
- [Hardware Setup](#hardware-setup)
- [Software Setup](#software-setup)
- [Project Structure](#project-structure)
- [Configuration](#configuration)
- [Usage](#usage)
- [Debugging](#debugging)
- [Troubleshooting](#troubleshooting)

## ✨ Features

- **Dual PWM Control**: Independent PWM control for five motors using TIM2
- **I2C Communication**: I2C interface for sensor/actuator communication
- **USART/UART**: Serial communication for debugging and control
- **GPIO Control**: Multiple GPIO pins for motor enable/disable control
- **Interrupt Handling**: Full interrupt service routines for real-time processing
- **HAL Library**: Uses STM32 HAL library for portability and ease of use

## 🔧 Hardware Setup

### Required Components

- **STM32F103C8T6** microcontroller (Blue Pill board)
- **Five DC Motors** with motor drivers
- **16MHz Crystal Oscillator** (on Blue Pill board)
- **USB Cable** for programming and serial communication
- **External Power Supply**: 6-12V for motors (do not power from USB)

## 💻 Software Setup

### Prerequisites

- **STM32CubeIDE**: Latest version
- **STM32CubeF1 Package**: Version 1.24.0 or later
- **ST-Link/V2 Debugger**: For programming and debugging
- **USB-TTL Adapter**: For serial communication (if not using Blue Pill USB)

### Installation Steps

1. **Clone the repository** (if using Git):
   ```bash
   git clone <repository-url>
   cd c8t6_PWM
   ```

2. **Import to STM32CubeIDE**:
   - Open STM32CubeIDE
   - File → Import → Existing Projects into Workspace
   - Select the `c8t6_PWM` folder
   - Click Finish

3. **Configure Device Settings**:
   - Right-click project → Properties
   - C/C++ Build → Settings → Tools
   - Add: STM32 GCC ARM EABI (under GCC ARM Embedded)
   - Set Device to: STM32F103xC
   - Set MCU Clock to: 72MHz

4. **Build and Program**:
   - Right-click project → Run As → Debug
   - Ensure ST-Link is connected
   - Wait for successful programming

## 📁 Project Structure

```
c8t6_PWM/
├── Core/
│   ├── Inc/
│   │   ├── main.h              # Main header file
│   │   ├── stm32f1xx_hal_conf.h # HAL configuration
│   │   └── stm32f1xx_it.h      # Interrupt handler declarations
│   └── Src/
│       ├── main.c              # Main application
│       ├── gpio.c              # GPIO initialization
│       ├── tim.c               # Timer/PWM initialization
│       ├── i2c.c               # I2C initialization
│       ├── usart.c             # USART initialization
│       ├── stm32f1xx_hal_msp.c # HAL memory management
│       ├── stm32f1xx_it.c      # Interrupt handlers
│       └── system_stm32f1xx.c  # System clock configuration
├── Drivers/                    # STM32 HAL library
├── .gitignore                 # Git ignore file
└── README.md                  # This file
```

## ⚙️ Configuration

### PWM Configuration

- **TIM2 Prescaler**: 72 (1Hz resolution)
- **TIM2 Period**: 1000 (PWM frequency: 72Hz)
- **Channel 1 (PA8)**: Motor A PWM
- **Channel 2 (PA9)**: Motor B PWM

### GPIO Configuration

| Pin | Name | Function |
|-----|------|----------|
| PA0 | M1_D1 | Motor A direction 1 |
| PA1 | M1_D2 | Motor A direction 2 |
| PA8 | M1_PWM | Motor A PWM |
| PA9 | M2_PWM | Motor B PWM |
| PA12 | M2_D1 | Motor B direction 1 |
| PA13 | M2_D2 | Motor B direction 2 |
| PB6 | I2C_SDA | I2C data |
| PB7 | I2C_SCL | I2C clock |
| PA9 | USART1_TX | UART TX |
| PA10 | USART1_RX | UART RX |

## 🚀 Usage

### Basic Motor Control

```c
#include "main.h"

// Set Motor A speed and direction
void set_motor_a(uint8_t speed, uint8_t direction) {
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);
    
    // Direction control
    if (direction == 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
    }
}

// Set Motor B speed and direction
void set_motor_b(uint8_t speed, uint8_t direction) {
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed);
    
    // Direction control
    if (direction == 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);
    }
}
```

### Serial Communication

The USART1 is configured at 115200 baud, 8N1. Use a serial terminal (PuTTY, TeraTerm, etc.) to receive debug output.

### I2C Communication

I2C is configured at 100kHz. Use the `i2c.c` module to communicate with I2C devices.

## 🐛 Debugging

### Common Debug Outputs

- Motor speed via PWM duty cycle
- GPIO states
- I2C communication status
- USART debug messages

### Debugging Tools

- **STM32CubeIDE Debug Console**: View serial output
- **Logic Analyzer**: Monitor GPIO and I2C signals
- **Oscilloscope**: Verify PWM waveforms

## ⚠️ Troubleshooting

### Motor Not Moving

1. Check power supply voltage (6-12V recommended)
2. Verify motor driver connections
3. Check PWM output on oscilloscope
4. Verify direction control GPIOs

### Communication Issues

1. Check USB-TTL adapter connections
2. Verify baud rate (115200)
3. Check wiring (TX ↔ RX, RX ↔ TX)
4. Try different USB port

### I2C Issues

1. Verify I2C pull-up resistors (4.7kΩ)
2. Check I2C device address
3. Use I2C scanner to detect devices
4. Check SDA/SCL signal with logic analyzer

### Build Errors

1. Ensure STM32CubeF1 package is installed
2. Check device selection (STM32F103xC)
3. Verify toolchain is configured correctly

## 📄 License

This project is provided as-is for educational purposes.

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!

## 📞 Support

For issues and questions, please create an issue in the repository.