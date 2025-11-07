# SPDX-FileCopyrightText: © 2022 Uri Shaked <uri@wokwi.com>
# SPDX-License-Identifier: MIT

# --- Configuration ---
SOURCES = soil-sensor.c 
OUT_DIR = custom-chips-bin

# Define exact paths for the final output files
TARGET_WASM = $(OUT_DIR)/soil-sensor-chip.wasm
TARGET_JSON = $(OUT_DIR)/soil-sensor-chip.json

# --- Main Targets ---
.PHONY: all
all: $(TARGET_WASM) $(TARGET_JSON)

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)

# --- Rules ---

# Ensure the output directory exists before trying to build into it
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Compile the WASM file
# Uses | $(OUT_DIR) to ensure directory exists first
$(TARGET_WASM): $(SOURCES) wokwi-api.h | $(OUT_DIR)
	clang --target=wasm32-unknown-wasi --sysroot /opt/wasi-libc -nostartfiles -Wl,--import-memory -Wl,--export-table -Wl,--no-entry -Werror -o $@ $(SOURCES)

# Copy the JSON file to the output directory
# $< is the source (soil-sensor.chip.json), $@ is the target (TARGET_JSON)
$(TARGET_JSON): ../soil-sensor.chip.json | $(OUT_DIR)
	cp $< $@
