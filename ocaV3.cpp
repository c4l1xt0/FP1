#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<cstdlib>

using namespace std;

const int NUM_CASILLAS = 63;
const int TURNOS_POSADA = 1;
const int TURNOS_PRISION = 2;
const int TURNOS_POZO = 3;
const int TURNOS_LABERINTO = 1;
const int TURNOS_MUERTE = 1;
const int CENTINELA = 0;
const int CASILLA_INICIAL = 1;
const int CASILLA_META = 63;
const int RETROCESO_LABERINTO = 12;
const int NUM_JUGADORES = 4;
const int MAX_JUGADORES = 4;
const int NUM_FILAS_A_DIBUJAR = 3;

const bool MODO_DEBUG = true;

typedef enum { NORMAL, OCA, PUENTE1, PUENTE2, POZO, POSADA, LABERINTO, DADO1, DADO2, CARCEL, CALAVERA } tCasilla;
typedef tCasilla tTablero[NUM_CASILLAS];
typedef int tJugadores[NUM_JUGADORES];

void iniciaTablero(tTablero tablero);
int tirarDadoManual();
int tirarDado();
int quienEmpieza();
void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& posicion);
void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& posicion);
bool cargaTablero(tTablero tablero);
void iniciaJugadores(tJugadores casillasJ, tJugadores penalizacionesJ);
int saltaACasilla(const tTablero tablero, int casillaActual);
void efectoTirada(const tTablero tablero, int& casillaJ, int& penalizacionJ);
tCasilla strToCasilla(string str);
void tirada(const tTablero tablero, int& casillaActual, int& penalizacion);
int partida(const tTablero tablero);
void mostrarTablero(const tTablero tablero, const tJugadores JugadoresJ);
string casillaAstring(tCasilla casilla);
void pintaTablero(const tTablero tablero, const tJugadores casillasJ);
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);
void pintaJugadores(const tJugadores casillasJ, int fila, int casillasPorFila);
string casillaAstring(tCasilla casilla);


int main() {

	tTablero tablero;
	srand(time(NULL));
	iniciaTablero(tablero);
	int ganador;

	if (cargaTablero(tablero)) {

		ganador = partida(tablero);

		if(ganador == 0){

			cout << "*----GANA EL JUGADOR 1----*" << endl;

		}
		else if(ganador == 1){

			cout << "*----GANA EL JUGADOR 2----*" << endl;

		}
		else if(ganador == 2){

			cout << "*----GANA EL JUGADOR 3----*" << endl;

		}
		else if(ganador == 3){

			cout << "*----GANA EL JUGADOR 4----*" << endl;

		}
	
	}


	return 0;
}

int tirarDado() {
	int numDado;
	numDado = 1 + rand() % (7 - 1);
	return numDado;
}

int quienEmpieza() {
	int quienEmpieza;
	quienEmpieza = 1 + rand() % (NUM_JUGADORES+1 - 1);
	return quienEmpieza;
}

int tirarDadoManual() {

	int dado;

	cout << "INTRODUCE EL VALOR DEL DADO: ";
	cin >> dado;

	return dado;

}

void iniciaTablero(tTablero tablero) {

	for (int i = 0; i < NUM_CASILLAS; i++) {

		tablero[i] = NORMAL;

	}

}

bool cargaTablero(tTablero tablero) {

	bool carga = false;
	int posicion;
	string nombreFichero, casillaAux;
	tCasilla casilla;
	ifstream archivo;

	cout << "INTRODUZCA EL NOMBRE DEL FICHERO: ";
	cin >> nombreFichero;
	archivo.open(nombreFichero);

	if (archivo.is_open()) {

		archivo >> posicion;

		while (posicion != 0 and posicion < NUM_CASILLAS) {

			archivo >> casillaAux;
			tablero[posicion] = strToCasilla(casillaAux);
			archivo >> posicion;

		}

		carga = true;

	}
	else {

		cout << "EL FICHERO NO SE HA PODIDO CARGAR" << endl;

	}

	return carga;

}

bool esCasillaPremio(const tTablero tablero, int casilla) {

	bool encontrado = false;


	if (tablero[casilla] == PUENTE1 or tablero[casilla] == PUENTE2 or tablero[casilla] == DADO1 or tablero[casilla] == DADO2 or tablero[casilla] == OCA) {

		encontrado = true;

	}

	return encontrado;

}



void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& posicion) {

	int contador = 0, contador1 = 0, contador2 = 0, contador3 = 0;
	bool salida = false, salida1 = false, salida2 = false, salida3 = false;
	//PARA BUSCAR LA SIGUIENTE OCA
	if (tablero[posicion] == OCA) {

		contador = posicion + 1;

		while (tablero[posicion] == OCA and !salida) {

			if (tablero[posicion] == OCA and tablero[contador] == OCA) {

				cout << "DE OCA A OCA Y TIRO PORQUE ME TOCA" << endl;
				posicion = contador;
				//tablero[posicion];
				salida = true;

			}

			contador++;

		}

	}// PARA PASAR DE PUENTE1 A PUENTE2 
	else if (tablero[posicion] == PUENTE1) {

		contador1 = posicion + 1;

		while (tablero[posicion] == PUENTE1 and !salida1) {

			if (tablero[posicion] == PUENTE1 and tablero[contador1] == PUENTE2) {

				cout << "DE PUENTE A PUENTE Y TIRO PORQUE ME LLEVA LA CORRIENTE" << endl;
				posicion = contador1;
				//tablero[posicion];
				salida1 = true;
			}

			contador1++;

		}

	}//PARA PASAR DE DADOS1 A DADOS2
	else if (tablero[posicion] == DADO1) {

		contador2 = posicion + 1;

		while (tablero[posicion] == DADO1 and !salida2) {

			if (tablero[posicion] == DADO1 and tablero[contador2] == DADO2) {

				cout << "DE DADO A DADO Y TIRO PORQUE ME HA TOCADO" << endl;
				posicion = contador2;
				//tablero[posicion];
				salida2 = true;

			}

			contador2++;

		}

	}

}

void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& posicion) {

	int contador = 0, contador1 = 0;
	bool salida = false, salida1 = false;

	//PARA PASAR DE PUENTE2 A PUENTE1
	if (tablero[posicion] == PUENTE2) {

		contador = posicion;

		while (tablero[posicion] == PUENTE2 and !salida) {

			if (tablero[posicion] == PUENTE2 and tablero[contador] == PUENTE1) {

				cout << "DE PUENTE A PUENTE Y TIRO PORQUE ME LLEVA LA CORRIENTE" << endl;
				posicion = contador;
				//tablero[posicion];
				salida = true;

			}

			contador--;

		}

	}// PARA PASAR DE DADOS2 A DADOS1
	else if (tablero[posicion] == DADO2) {

		contador1 = posicion;

		while (tablero[posicion] == DADO2 and !salida1) {

			if (tablero[posicion] == DADO2 and tablero[contador1] == DADO1) {

				cout << "DE DADO A DADO Y TIRO PORQUE ME HA TOCADO" << endl;
				posicion = contador1;
				//tablero[posicion];
				salida1 = true;

			}

			contador1--;

		}

	}

}

void iniciaJugadores(tJugadores casillasJ, tJugadores penalizacionesJ) {

	for (int i = 0; i < NUM_JUGADORES; i++) {

		casillasJ[i] = 1;

	}

	for (int j = 0; j < NUM_JUGADORES; j++) {

		penalizacionesJ[j] = 0;

	}



}

int saltaACasilla(const tTablero tablero, int casillaActual) {

	tCasilla tipo = NORMAL;

	if (tablero[casillaActual] == OCA or tablero[casillaActual] == PUENTE1 or tablero[casillaActual] == DADO1) {

		buscaCasillaAvanzando(tablero, tipo, casillaActual);
		cout << "PASAS A LA CASILLA: " << casillaActual << endl;

	}
	else if (tablero[casillaActual] == PUENTE2 or tablero[casillaActual] == DADO2) {

		buscaCasillaRetrocediendo(tablero, tipo, casillaActual);
		cout << "PASAS A LA CASILLA: " << casillaActual << endl;

	}

	return casillaActual;

}

void efectoTirada(const tTablero tablero, int& casillaJ, int& penalizacionJ) {

	if (esCasillaPremio(tablero, casillaJ)) {

		penalizacionJ++;
		casillaJ = saltaACasilla(tablero, casillaJ);

	}
	else if (tablero[casillaJ] == LABERINTO) {

		casillaJ = casillaJ - 12;
		cout << "HAS CAIDO EN EL LABERINTO, PIERDES 12 CASILLAS. CASILLA ACTUAL: " << casillaJ;

	}
	else if (tablero[casillaJ] == CALAVERA) {

		casillaJ = 1;
		cout << "HAS CAIDO EN LA MUERTE, VUELVES AL INICIO. CASILLA ACTUAL: 1" << endl;

	}
	else if (tablero[casillaJ] == POZO) {

		penalizacionJ = penalizacionJ - TURNOS_POZO;
		cout << "HAS CAIDO EN EL POZO, PIERDES 3 TURNOS." << endl;

	}
	else if (tablero[casillaJ] == POSADA) {

		penalizacionJ = penalizacionJ - TURNOS_POSADA;
		cout << "TE DUERMES EN LA POSADA, PIERDES 1 TURNO." << endl;

	}
	else if (tablero[casillaJ] == CARCEL) {

		penalizacionJ = penalizacionJ - TURNOS_PRISION;
		cout << "TE HAN ENCARCELADO, PIERDES 2 TURNOS." << endl;

	}

}

tCasilla strToCasilla(string str) {

	tCasilla casilla;

	if (str == "OCA") {

		casilla = OCA;

	}
	else if (str == "PUENTE1") {

		casilla = PUENTE1;

	}
	else if (str == "PUENTE2") {

		casilla = PUENTE2;

	}
	else if (str == "POSADA") {

		casilla = POSADA;

	}
	else if (str == "DADO1") {

		casilla = DADO1;

	}
	else if (str == "POZO") {

		casilla = POZO;

	}
	else if (str == "LABERINTO") {

		casilla = LABERINTO;

	}
	else if (str == "CARCEL") {

		casilla = CARCEL;

	}
	else if (str == "DADO2") {

		casilla = DADO2;

	}
	else if (str == "CALAVERA") {

		casilla = CALAVERA;

	}

	return casilla;

}

void tirada(const tTablero tablero, int& casillaActual, int& penalizacion) {

	int dado;
	bool fin = false;

	cout << "POSICION ACTUAL: " << casillaActual << endl;

	if (MODO_DEBUG) {

		dado = tirarDadoManual();

	}
	else if (!MODO_DEBUG) {

		dado = tirarDado();
		system("pause");

	}

	cout << "VALOR DEL DADO: " << dado << endl;
	casillaActual = casillaActual + dado;
	cout << "PASAS A LA CASILLA: " << casillaActual << endl;

	if (casillaActual<=NUM_CASILLAS) {

		efectoTirada(tablero, casillaActual, penalizacion);

	}

}

int partida(const tTablero tablero) {

	tJugadores jugadoresJ, penalizacionesJ;
	int jugadorQueTira;
	
    bool fin = false;
	iniciaJugadores(jugadoresJ, penalizacionesJ);

	cout << endl;

    pintaTablero(tablero,jugadoresJ);

	jugadorQueTira = quienEmpieza();

	cout << endl;

	if (jugadorQueTira == 1) {

		jugadorQueTira = 0;

	}
	else if (jugadorQueTira == 2) {

		jugadorQueTira = 1;

	}
	else if (jugadorQueTira == 3) {

		jugadorQueTira = 2;

	}
	else if (jugadorQueTira == 4) {

		jugadorQueTira = 3;

	}

	while (!fin) {

		if(penalizacionesJ[jugadorQueTira]>=0){

			cout << "TURNO JUGADOR " << jugadorQueTira + 1<< ": " << endl;
			tirada(tablero, jugadoresJ[jugadorQueTira], penalizacionesJ[jugadorQueTira]);
			pintaTablero(tablero,jugadoresJ);

		}
		else if(penalizacionesJ[jugadorQueTira]<0){

			penalizacionesJ[jugadorQueTira]++;
			cout << "EL JUGADOR " << jugadorQueTira + 1 << " SE LLEVA UNA PENALIZACIOND DE " << abs(penalizacionesJ[jugadorQueTira]) << " TURNO/S" << endl;

		}
		if(jugadoresJ[jugadorQueTira] < NUM_CASILLAS and !esCasillaPremio(tablero, jugadoresJ[jugadorQueTira])){
		    
		    jugadorQueTira++;
		    
		    if(jugadorQueTira==4){
		        
		        jugadorQueTira = 0;
		        
		    }
		    else if(jugadorQueTira==3 and NUM_JUGADORES==3){
		        
		        jugadorQueTira = 0;
		        
		    }
		    else if(jugadorQueTira==2 and NUM_JUGADORES==2){
		        
		        jugadorQueTira = 0;
		        
		    }
		    
		}
		else if(jugadoresJ[jugadorQueTira] >= NUM_CASILLAS){

			fin = true;

		}

	}

	
	return jugadorQueTira;

}


string casillaAstring(tCasilla casilla) {

	string casillas;

	if (casilla == NORMAL) {

		casillas = " ";

	}
	else if (casilla == OCA) {

		casillas = "OCA";

	}
	else if (casilla == PUENTE1 or casilla == PUENTE2) {

		casillas = "PNTE";

	}
	else if (casilla == DADO1 or casilla == DADO2) {

		casillas = "DADO";

	}
	else if (casilla == CALAVERA) {

		casillas = "MUER";

	}
	else if (casilla == POZO) {

		casillas = "POZO";

	}
	else if (casilla == POSADA) {

		casillas = "PSDA";

	}
	else if (casilla == LABERINTO) {

		casillas = "LBNT";

	}
	else if (casilla == CARCEL) {

		casillas = "CRCL";

	}

	return casillas;

}






int cambiaTurno(int turno) {

    //TURNO JUGADOR 1 PASA A JUGADOR 2
    
    cout << endl << turno << endl;
	if(turno==0){

	    
		turno++;
		

	}//SI HAY 3 JUGADORES, JUGADOR 2 PASA A JUGADOR 3
    else if(turno==1 and NUM_JUGADORES > 2){
	    
		turno = 2;
		
	}
	else if(turno == 1 and NUM_JUGADORES==2){
	
	//SI HAY SOLO 2 JUGADORES DEVUELVE EL TURNO AL JUGADOR 1
		turno = 0;

	}// SI HAY 4 JUGADORES MANDA EL TURNO A JUGADOR 4
	else if(turno==2 and NUM_JUGADORES > 3){
        
		turno++;

	}
	else if(turno == 2 and NUM_JUGADORES == 3){
        //SI HAY 3 JUGADORES LE MANDA EL TURNO AL JUGADOR 1
		turno = 0;

	}
    else if(turno==3){

		turno=0;

	}
	
	cout << endl << turno << endl;

	return turno;

}


/*string casillaAstring(tCasilla casilla) {
    string cadena;
    switch (casilla) {
    case NORMAL:
        cadena = " ";
        break;
    case OCA:
        cadena = "OCA";
        break;
    case DADO1:
    case DADO2:
        cadena = "DADO";
        break;
    case PUENTE1:
    case PUENTE2:
        cadena = "PNTE";
        break;
    case POSADA:
        cadena = "PSDA";
        break;
    case CALAVERA:
        cadena = "MUER";
        break;
    case LABERINTO:
        cadena = "LBRN";
        break;
    case POZO:
        cadena = "POZO";
        break;
    case CARCEL:
        cadena = "CRCL";
        break;
    }
    return cadena;
}*/

void pintaTablero(const tTablero tablero, const tJugadores casillasJ) {

   
    int casillasPorFila = CASILLA_META / NUM_FILAS_A_DIBUJAR; 
    cout << endl;
    for (int fila = 0; fila < NUM_FILAS_A_DIBUJAR; fila++) {
        pintaBorde(casillasPorFila);
        pintaNumCasilla(fila, casillasPorFila);
        pintaTipoCasilla(tablero, fila, casillasPorFila);
        pintaJugadores(casillasJ, fila, casillasPorFila);
    }
    pintaBorde(casillasPorFila);
    cout << endl;
}


void pintaBorde(int casillasPorFila) {

    for (int i = 1; i <= casillasPorFila; i++)
        cout << "|====";

    cout << "|" << endl;

}


void pintaNumCasilla(int fila, int casillasPorFila) {

    for (int i = 1; i <= casillasPorFila; i++)
        cout << "| " << setw(2) << setfill('0') << i + fila * casillasPorFila << " ";

    cout << "|" << endl;

}


void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila) {

    for (int i = 2; i <= casillasPorFila+1; i++)
        cout << "|" << setw(4) << setfill(' ') << casillaAstring(tablero[i - 1 + fila * casillasPorFila]);

    cout << "|" << endl;

}

void pintaJugadores(const tJugadores casillasJ, int fila, int casillasPorFila) {
    int casilla;

    int blancos = MAX_JUGADORES - NUM_JUGADORES;
    string b = "";
    for (int i = 0; i < blancos; i++) b = b + " ";
    cout << "|";
    for (int i = 2; i <= casillasPorFila+1; i++) {
        casilla = i - 1 + fila * casillasPorFila;
        for (int jug = 0; jug < NUM_JUGADORES; jug++) {
            if (casillasJ[jug] == casilla)
                cout << jug + 1; 
            else
                cout << " ";
        }
        cout << b;
        cout << "|";
    }
    cout << endl;

}
