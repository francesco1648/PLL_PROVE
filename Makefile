SKETCH_PATH = $(CURDIR)
SKETCH_NAME = $(notdir $(SKETCH_PATH))

# Configurazione board
BOARD_FQBN = rp2040:rp2040:rpipico
OUTPUT_DIR = $(CURDIR)/build/output

# Rilevamento del sistema operativo
ifeq ($(OS),)
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S), Linux)
    OS := Linux
  endif
  ifeq ($(UNAME_S), Darwin)
    OS := Darwin
  endif
  ifeq ($(UNAME_S),)
    OS := Windows_NT
  endif
endif


CLEAN_DIR = $(CURDIR)/build

# 📁 Percorsi librerie e include
LIBS_DIR = $(CURDIR)/lib
INCLUDE_DIR = $(CURDIR)/include
LIBRARY_PATHS = $(wildcard $(LIBS_DIR)/*/src)
LIBRARY_FLAGS = $(addprefix --library ,$(LIBRARY_PATHS))
INCLUDE_PATHS = $(INCLUDE_DIR) $(LIBRARY_PATHS)

# 🖥️ Comandi specifici per OS
ifeq ($(OS), Windows_NT)
    RM = rmdir /s /q
    COPY = powershell -Command "Copy-Item"
    FIND_PORT = powershell -Command "& {arduino-cli board list | Select-String -Pattern 'Pico' | ForEach-Object {($_ -split '\\s+')[0]}}"
    GREEN = powershell -Command "Write-Host '$1' -ForegroundColor Green"
    RED = powershell -Command "Write-Host '$1' -ForegroundColor Red"
else
    RM = rm -rf
    COPY = cp
    FIND_PORT = sh -c "arduino-cli board list | grep 'Pico' | awk '{print $$1}'"
    GREEN = echo "\033[32m$1\033[0m"
    RED = echo "\033[31m$1\033[0m"
endif

# 🔌 Rilevamento automatico della porta COM
PORT = COM12

define print_green
	@powershell -Command "Write-Host '$1' -ForegroundColor Green"
endef

define print_red
	@powershell -Command "Write-Host '$1' -ForegroundColor Red"
endef

# 🛠️ Compilazione
compile:
	$(call GREEN, " Compilazione in corso...")

ifeq ($(OS), Windows_NT)
	@if not exist "$(OUTPUT_DIR)" mkdir "$(OUTPUT_DIR)"
else
	@mkdir -p "$(OUTPUT_DIR)"
endif

	@arduino-cli compile --fqbn $(BOARD_FQBN) --build-path $(OUTPUT_DIR) $(SKETCH_PATH) --output-dir $(OUTPUT_DIR) $(LIBRARY_FLAGS) \
		$(foreach dir, $(INCLUDE_PATHS), --build-property "compiler.cpp.extra_flags=-I$(dir)")

	$(call GREEN, " Compilazione completata con successo!")

compile_fast:
	@arduino-cli compile --fqbn $(BOARD_FQBN) "$(SKETCH_PATH)"

# 🚀 Upload sulla Raspberry Pi Pico
upload:
	@echo "Uploading .bin file to Raspberry Pi Pico..."
	@arduino-cli upload -p $(PORT) --fqbn $(BOARD_FQBN) $(SKETCH_PATH)

#  Upload manuale in modalità BOOTSEL
upload_bootsel:
	@echo " Copia del file .uf2 sulla Raspberry Pi Pico..."
ifeq ($(OS), Windows_NT)
	@powershell -Command "Copy-Item '$(OUTPUT_DIR)/$(SKETCH_NAME).ino.uf2' -Destination 'E:\' -Force"
else
	@cp "$(OUTPUT_DIR)/$(SKETCH_NAME).ino.uf2" /media/*/RPI-RP2/ || cp "$(OUTPUT_DIR)/$(SKETCH_NAME).ino.uf2" /run/media/*/RPI-RP2/
endif

# pulizia cartella di build
clean:
	$(call GREEN, "🧹 Pulizia in corso...")

ifeq ($(OS), Windows_NT)
	@if exist "$(CLEAN_DIR)" rmdir /s /q "$(CLEAN_DIR)"
else
	@rm -rf "$(CLEAN_DIR)"
endif

	$(call GREEN, "🧼 Cartella di build pulita!")

# 📡 Monitor seriale
monitor:
	@echo "📡 Connessione al monitor seriale sulla porta $(PORT)..."
	@arduino-cli monitor -p $(PORT) -c baudrate=115200

# 🔄 Compilazione + Upload in un solo passaggio
all: clean compile upload

# ℹ️ Guida ai comandi disponibili
help:
	@echo "📖 Comandi disponibili:"
	@echo "  make compile       -  Compila il progetto"
	@echo "  make compile_fast  - Compilazione veloce senza librerie aggiuntive"
	@echo "  make upload        -  Carica il firmware sulla Raspberry Pi Pico"
	@echo "  make upload_bootsel - Carica il file .uf2 manualmente su E:/ o /media"
	@echo "  make monitor       -  Avvia il monitor seriale"
	@echo "  make all           -  Compila e carica il progetto in un solo passaggio"
	@echo "  make clean         -  Pulisce i file di compilazione"
	@echo "  make help          -  Mostra questa guida"
	@echo "  make connected_com_port - Mostra la COM impostata per il Raspberry Pi Pico"
	@echo "  make list_com_port - Elenca tutte le porte COM disponibili"

# 🔍 Stampa la porta COM rilevata
connected_com_port:
	@echo " La porta COM attualmente impostata è: $(PORT)"

# 🔍 Elenca tutte le porte COM disponibili
list_com_port:
	@echo " Elenco delle porte COM rilevate:"
	@arduino-cli board list
