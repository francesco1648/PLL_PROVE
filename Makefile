
SKETCH_PATH = $(CURDIR)


SKETCH_NAME = $(notdir $(SKETCH_PATH))

# Configurazione board
BOARD_FQBN = rp2040:rp2040:rpipico
OUTPUT_DIR = $(CURDIR)/build


PORT = COM12

#PORT = $(shell arduino-cli board list | findstr /r /c:"COM[0-9]*" | findstr /v "Unknown" | findstr /v "core=\"\"")

LIB_DIRS = \
    $(CURDIR)/lib/SmartMotor/src \
    $(CURDIR)/lib/AbsoluteEncoder/src \
    $(CURDIR)/lib/DynamixelSerial/src \
    $(CURDIR)/lib/Battery/src \
    $(CURDIR)/lib/Debug/src \
    $(CURDIR)/lib/Can/src \
    $(CURDIR)/lib/Display/src


CFLAGS += $(foreach dir, $(LIB_DIRS), -I$(dir))

SUCCESS_SYMBOL = " Compilazione completata con successo!"
ERROR_SYMBOL = " Errore durante la compilazione!"
COMPILATION_SYMBOL = " Compilazione in corso..."


define print_green
	@powershell -Command "Write-Host '$1' -ForegroundColor Green"
endef

define print_red
	@powershell -Command "Write-Host '$1' -ForegroundColor Red"
endef

# Compilazione
compile:
	$(call print_green, $(COMPILATION_SYMBOL))
	@arduino-cli compile --fqbn $(BOARD_FQBN) --build-path $(OUTPUT_DIR) $(SKETCH_PATH)



# Upload
upload_bootsel:

	@echo "Controllando se il file .uf2 esiste..."

	@echo "Il file .uf2 esiste: $(OUTPUT_DIR)/$(SKETCH_NAME).ino.uf2"
	@echo "Uploading .uf2 file to Raspberry Pi Pico..."
	@echo "Copia il file su E:\..."
	@echo "Copia del file .uf2 su E:\ utilizzando PowerShell..."
	@powershell -Command "Copy-Item '$(OUTPUT_DIR)/$(SKETCH_NAME).ino.uf2' -Destination 'E:\' -Force"

upload:


	@echo "Uploading .bin file to Raspberry Pi Pico..."
	@arduino-cli upload -p $(PORT) --fqbn $(BOARD_FQBN) $(SKETCH_PATH)




compile_fast:
	arduino-cli compile --fqbn $(BOARD_FQBN) "$(SKETCH_PATH)"

clean:
	$(DEL) /F /Q "$(OUTPUT_DIR)\*"
	$(RMDIR) /S /Q "$(OUTPUT_DIR)"
	$(call print_green, "Cartella di build pulita.")


monitor:
	arduino-cli monitor -p $(PORT) -c baudrate=115200


all: compile upload


build:

	arduino-cli compile --fqbn $(BOARD_FQBN) --output-dir $(OUTPUT_DIR) "$(SKETCH_PATH)"
	@echo "Build completato: $(OUTPUT_DIR)/$(SKETCH_NAME).uf2"

help:
	@echo "Comandi disponibili:"
	@echo "  make compile      - Compila il progetto"
	@echo "  make upload       - Carica il progetto sulla Raspberry Pi Pico"
	@echo "  make monitor      - Avvia il monitor seriale"
	@echo "  make all          - Compila e carica il progetto in un solo passaggio"
	@echo "  make help         - Mostra questa guida"
	@echo "  make com_port     - Stampa la porta COM utilizzata"
	@echo "  make auto_com_port - Rileva automaticamente la porta COM del Raspberry Pi Pico o Arduino"


auto_com_port:
	@echo "La porta COM rilevata automaticamente e : $(PORT)"
port:
	@echo "Elenco delle porte COM rilevate dal sistema:"
	@arduino-cli board list
