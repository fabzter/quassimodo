SRC_DIR=src
BIN_DIR=bin

all: doc test bin

doc: Doxyfile
	doxygen

test: 
	

bin: 
	cd $(SRC_DIR)/Grafico && make
	cd $(SRC_DIR)/Reglas && make
	cd $(SRC_DIR)/Opciones && make
	cd $(SRC_DIR)/Aplicacion && make

clean:
	cd $(SRC_DIR)/AgenteWrapper && make clean
	cd $(SRC_DIR)/Aplicacion && make clean
	cd $(SRC_DIR)/Grafico && make clean
	cd $(SRC_DIR)/Reglas && make clean
	cd $(SRC_DIR)/Opciones && make clean
	cd $(BIN_DIR) && ${RM} ./*.pyc

.PHONY: doc bin test all clean

