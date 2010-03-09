all: Reglas.so

boost_cflags = -ftemplate-depth-100 -DBOOST_PYTHON_STATIC_LIB \
               -isystem /usr/include/python2.6 -I.
boost_libs = -Wl,-rpath-link,. -L/usr/lib/python2.6/config -Bstatic -lboost_python -Bdynamic

Reglas_libs = -L. -L../lib -Wl,-R. -Wl,-R../lib -lReglas
Reglas_cflags = -I../

MODULES = Agente.o Tablero.o container_conversions.o Pieza.o Celda.o Enums.o \
            Barrera.o Excepciones.o
            
Excepciones.o: Excepciones.cpp
	g++ -c $(boost_cflags) $(Reglas_cflags) -fpic -fPIC Excepciones.cpp

Enums.o: Enums.cpp
	g++ -c $(boost_cflags) $(Reglas_cflags) -fpic -fPIC Enums.cpp

Pieza.o: Pieza.cpp
	g++ -c $(boost_cflags) $(Reglas_cflags) -fpic -fPIC Pieza.cpp

Celda.o: Celda.cpp
	g++ -c $(boost_cflags) $(Reglas_cflags) -fpic -fPIC Celda.cpp

Barrera.o: Barrera.cpp
	g++ -c $(boost_cflags) $(Reglas_cflags) -fpic -fPIC Barrera.cpp

Agente.o: Agente.cpp
	g++ -c $(boost_cflags) $(Reglas_cflags) -fpic -fPIC Agente.cpp
	
Tablero.o: Tablero.cpp
	g++ -c $(boost_cflags) $(Reglas_cflags) -fpic -fPIC Tablero.cpp
	
container_conversions.o: container_conversions.cpp container_conversions.h
	g++ -c $(boost_cflags) $(Reglas_cflags) -fpic -fPIC container_conversions.cpp
	
main.o: main.cpp
	g++ -c $(boost_cflags) $(Reglas_cflags) -fpic -fPIC main.cpp
	
Reglas.so: main.o $(MODULES)
	g++ -o Reglas.so -fpic -module -shared  -Wl,-rpath . \
	$(boost_libs) $(Reglas_libs) $(MODULES) main.o
	cp Reglas.so ../lib
	
clean:
	${RM} *.so *.o
	${RM} ../lib/Reglas.so
