SRC_DIR=src
BIN_DIR=bin

all: doc test bin

doc: Doxyfile
	doxygen

test: 
	cd $(SRC_DIR)/Ejecutable && make
	cd $(SRC_DIR)/PruebaGrafico && make

bin: 
	cd $(SRC_DIR)/Grafico && make
	cd $(SRC_DIR)/Reglas && make
	cd $(SRC_DIR)/Scripting && make

clean:
	cd $(SRC_DIR)/AgenteWrapper && make clean
	cd $(SRC_DIR)/Ejecutable && make clean
	cd $(SRC_DIR)/PruebaGrafico && make clean
	cd $(SRC_DIR)/Grafico && make clean
	cd $(SRC_DIR)/Reglas && make clean
	cd $(SRC_DIR)/Scripting && make clean
	cd $(BIN_DIR) && ${RM} ./*.pyc

.PHONY: doc bin test all clean

