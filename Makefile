SRC_DIR=src
BIN_DIR=bin
INSTALL_DIR=/opt/quassimodo
EXE_PATH=/usr/bin/quassimodo

all: doc test bin

doc: Doxyfile
	doxygen

test: 
	cd $(SRC_DIR)/Aplicacion && make

bin: 
	cd $(SRC_DIR)/Aplicacion && make
	cd $(SRC_DIR)/Grafico && make
	cd $(SRC_DIR)/Reglas && make
	cd $(SRC_DIR)/Scripting && make
	cd $(SRC_DIR)/Opciones && make

install: bin
	mkdir -p $(INSTALL_DIR) &&\
	cp -rf ./* $(INSTALL_DIR) &&\
	echo "cd $(INSTALL_DIR) && ./bin/aplicacion" > $(EXE_PATH) &&\
	chmod +x $(EXE_PATH)

uninstall:
	rm -rf $(INSTALL_DIR)
	rm -rf $(EXE_PATH)

clean:
	cd $(SRC_DIR)/AgenteWrapper && make clean
	cd $(SRC_DIR)/Aplicacion && make clean
	cd $(SRC_DIR)/Grafico && make clean
	cd $(SRC_DIR)/Reglas && make clean
	cd $(SRC_DIR)/Scripting && make clean
	cd $(SRC_DIR)/Opciones && make clean
	cd $(BIN_DIR) && ${RM} ./*.pyc

.PHONY: doc bin test all clean install

