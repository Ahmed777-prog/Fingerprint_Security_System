# ==========================================
# Microcontroller & Project Configuration
# ==========================================
MCU = atmega32
F_CPU = 8000000UL
BUILD_DIR = Build
TARGET = $(BUILD_DIR)/Fingerprint_Security_System

# ==========================================
# Toolchain Commands
# ==========================================
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -std=gnu99

# ==========================================
# Sources & Includes
# ==========================================
# 1. Find all .c files
SRC = $(wildcard *.c) \
      $(wildcard APP/*.c) \
      $(wildcard HAL/*/*.c) \
      $(wildcard MCAL/*/*.c) \
      $(wildcard MCAL/*/*/*.c) \
      $(wildcard LIB/*/*.c) \
      $(wildcard OS/FREERTOS/*.c) \
      $(wildcard OS/FREERTOS/portable/MemMang/*.c) \
      $(wildcard OS/FREERTOS/portable/GCC/ATMega323/*.c)

# 2. Extract unique directories from SRC and add LIB and FreeRTOS includes
INC_DIR = $(sort $(dir $(SRC))) LIB OS/FREERTOS/include OS/FREERTOS/portable/GCC/ATMega323

# 2. Extract unique directories from SRC and add LIB
INC_DIR = $(sort $(dir $(SRC))) LIB OS/FREERTOS/include

# 3. Add -I prefix to all directories
INC = $(addprefix -I,$(INC_DIR))

# ==========================================
# Object Files & VPATH
# ==========================================
# Extract only the filename and append .o inside BUILD_DIR
OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRC)))

# Tell Make where to search for .c files (very important for layered architecture)
VPATH = $(dir $(SRC)) .

# ==========================================
# Build Targets
# ==========================================
all: $(BUILD_DIR) $(TARGET).elf $(TARGET).hex

# Create Build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Step 1: Compilation (Compile .c to .o)
$(BUILD_DIR)/%.o: %.c
	@echo "Compiling $< ..."
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Step 2: Linking (Link all .o to .elf)
$(TARGET).elf: $(OBJ)
	@echo "Linking to generate .elf ..."
	@$(CC) $(CFLAGS) $(OBJ) -o $@

# Step 3: Hex Generation (Convert .elf to .hex)
$(TARGET).hex: $(TARGET).elf
	@echo "Generating Hex File ..."
	@$(OBJCOPY) -O ihex -R .eeprom $< $@
	@echo ">>> Build Successful! Hex file is ready in $(BUILD_DIR) <<<"

# ==========================================
# Clean
# ==========================================
clean:
	@echo "Cleaning Build directory ..."
	@rm -rf $(BUILD_DIR)

flash:
	avrdude -c usbasp -p m32 -U flash:w:$(TARGET).hex:i