#  Impostazioni di base
SKETCH_PATH = $(CURDIR)
SKETCH_NAME = $(notdir $(SKETCH_PATH))

# Configurazione board
BOARD_FQBN = rp2040:rp2040:rpipico
OUTPUT_DIR = $(CURDIR)/build/output
BUILD_DIR = $(CURDIR)/build
LIBS_DIR = $(CURDIR)/lib
INCLUDE_DIR = $(CURDIR)/include

LIBRARY_PATHS = $(wildcard $(LIBS_DIR)/*/src)
LIBRARY_FLAGS = $(addprefix --library ,$(LIBRARY_PATHS))

INCLUDE_PATHS = $(INCLUDE_DIR) $(LIBRARY_PATHS)
CFLAGS += $(foreach dir, $(INCLUDE_PATHS), -I$(dir))
CXXFLAGS += $(foreach dir, $(INCLUDE_PATHS), -I$(dir))

SUCCESS_SYMBOL = " Compilazione completata con successo! "
ERROR_SYMBOL = " Errore durante la compilazione! "
COMPILATION_SYMBOL = " Compilazione in corso... "

define print_green
	@powershell -Command "Write-Host '$1' -ForegroundColor Green"
endef

define print_red
	@powershell -Command "Write-Host '$1' -ForegroundColor Red"
endef

PORT ?= $(shell arduino-cli board list | findstr "Raspberry Pi Pico" | for /f "tokens=1" %%a in ('more') do @echo %%a)

.DEFAULT:
	@echo "Comando non valido: '$@'"
	@echo "Usa 'make help' per vedere l'elenco dei comandi disponibili."
	@$(MAKE) help

#  Compilazione
compile: clean_all
	$(call print_green, $(COMPILATION_SYMBOL))
	@arduino-cli compile --fqbn $(BOARD_FQBN) --build-path $(BUILD_DIR) $(SKETCH_PATH) --output-dir $(OUTPUT_DIR) $(LIBRARY_FLAGS) \
		$(foreach dir, $(INCLUDE_PATHS), --build-property "compiler.cpp.extra_flags=-I$(dir)")

compile_fast:
	@arduino-cli compile --fqbn $(BOARD_FQBN) "$(SKETCH_PATH)"

#  Upload del file .bin
upload:
	@echo "Controllare di aver inserito la porta COM corretta. La porta COM attuale e': $(PORT)"
	@if exist "$(OUTPUT_DIR)/$(SKETCH_NAME).ino.bin" ( \
		echo "Uploading .bin file to Raspberry Pi Pico..." & \
		arduino-cli upload -p $(PORT) --fqbn $(BOARD_FQBN) --input-dir $(OUTPUT_DIR) $(SKETCH_NAME).ino.bin \
	) else ( \
		echo " File .bin non trovato. Eseguire 'make compile' prima di caricare il codice." \
	)

# Upload del file .uf2 in modalità BOOTSEL
upload_bootsel:
	@if exist "$(OUTPUT_DIR)/$(SKETCH_NAME).ino.uf2" ( \
		echo "Uploading .uf2 file to Raspberry Pi Pico..." & \
		powershell -Command "Copy-Item '$(OUTPUT_DIR)/$(SKETCH_NAME).ino.uf2' -Destination 'E:\' -Force" \
	) else ( \
		echo " File .uf2 non trovato. Eseguire 'make compile' prima di caricare il codice." \
	)

#  Pulizia della cartella di build
clean_all:
	@echo BUILD_DIR è: "$(BUILD_DIR)"
	@if exist "$(BUILD_DIR)\output" ( \
		echo La cartella di build esiste. & \
		rd /s /q "$(BUILD_DIR)" & \
		echo Contenuto della cartella di output rimosso. \
	) else ( \
		echo La cartella di output non esiste. \
	)


clean_output:
	@echo "Pulizia in corso..."
	@if exist "$(BUILD_DIR)/output" ( \
		echo "Rimuovendo i file nella cartella di build..." \
		rd /s /q "$(BUILD_DIR)/output" \
		echo "Contenuto della cartella di output rimosso." \
	) else ( \
		echo "La cartella di output non esiste." \
	)
	$(call print_green, "Contenuto della cartella di output pulito. 🧹")




#  Monitor seriale
monitor:
	arduino-cli monitor -p $(PORT) -c baudrate=115200

#  Guida ai comandi
help:
	@echo "Comandi disponibili:"
	@echo "  make compile       - Compila il progetto"
	@echo "  make compile_fast  - Compilazione veloce senza librerie aggiuntive"
	@echo "  make upload        - Carica il progetto sulla Raspberry Pi Pico"
	@echo "  make upload_bootsel - Carica il file .uf2 manualmente su E:/"
	@echo "  make monitor       - Avvia il monitor seriale"
	@echo "  make all           - Compila e carica il progetto in un solo passaggio"
	@echo "  make clean         - Pulisce i file di compilazione"
	@echo "  make help          - Mostra questa guida"
	@echo "  make auto_com_port - Rileva automaticamente la porta COM del Raspberry Pi Pico"

#  Stampa la porta COM rilevata
auto_com_port:
	@echo "La porta COM rilevata automaticamente è: $(PORT)"

#  Elenca tutte le porte COM disponibili
port:
	@echo "Elenco delle porte COM rilevate dal sistema:"
	@arduino-cli board list
