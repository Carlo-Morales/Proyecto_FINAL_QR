#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <algorithm>
#include <math.h>

using namespace std;
string url;
vector <int> ascii(26);
vector <bitset<8>> binario(26);
vector <int> DataCodework(44);
vector <int> bitsDataCodework(224);
vector <int> bitsDeParidad(128);  // Vector para almacenar los bits de paridad


const int TAMANO_QR = 27;
const int BLANCO = 0;
const int BLANCO1 = 2;
const int NEGRO = 1;
const int NEGRO1 = 3;
const int VACIO = -1;


int matriz_qr[TAMANO_QR][TAMANO_QR]; 
int i, j;


int string_a_binario (string url, vector<int>&ascii, vector<bitset<8>>&binario){
        // CONVERSION ASCII
    //si necesitamos hacer padding
    if (url.size()<26){
        for (int i = 0; i<url.size(); i++){
            ascii[i] = (int)url[i];
            binario[i] = bitset<8>(ascii[i]); //Almacenamiento de los binarios en un bitset dentro del vector
        }
        int k = 0;
        for (int i = url.size(); i < 26; i++){
            if (k == 0){
                ascii[i] = 236;
                k = 1;
            }
            else {
                ascii[i] = 17;
                k = 0;
            }
            binario[i] = bitset<8>(ascii[i]); //Almacenamiento de los binarios en un bitset dentro del vector
        }
    }
    //si no necesitamos hacer padding
    else {
        for (int i = 0; i < 26; i++){
            ascii[i] = (int)url[i];
            binario[i] = bitset<8>(ascii[i]); //Almacenamiento de los binarios en un bitset dentro del vector
        }
    }
    return binario.size(); //return del binario
}
void data_codework (string url, vector<int> &DataCodework, vector<int> &bitsDataCodework, vector<int> &ascii, vector<bitset<8>>& binario) {
    // Rellenado del vector de bits
    bitsDataCodework[0] = 0;
    bitsDataCodework[1] = 1;
    bitsDataCodework[2] = 0;
    bitsDataCodework[3] = 0;

    bitset<8> tam = url.size();
    int k = 7;
    for (int i = 4; i < 12; i++) {
        bitsDataCodework[i] = tam[k--];
    }

    // URL + Padding (si es necesario)
    int cont_letras = 0;
    int cont_bits = 7;
    for (int i = 12; i < 220; i++) {
        bitsDataCodework[i] = binario[cont_letras][cont_bits];
        cont_bits--;
        if (cont_bits < 0) {
            cont_bits = 7;
            cont_letras++;
        }
    }
    // Terminador
    for (int i = 220; i < 224; i++) {
        bitsDataCodework[i] = 0;
    }
    // Rellenado del vector ASCII
    int pos = 0;
    int cont1 = 7;
    int cont2 = 0;
    bitset<8> temp;
    for (int i = 0; i < 224; i++) {
        temp[cont1--] = bitsDataCodework[i];
        pos++;
        // Si ya llenamos los 8 bits
        if (pos == 8) {
            DataCodework[cont2++] = temp.to_ulong();
            temp.reset();
            cont1 = 7;
            pos = 0;
        }
    }
    // Si quedaron bits sin completar
    if (pos != 0) {
        DataCodework[cont2++] = temp.to_ulong();
    }
    // Imprimir resultados (solo para ver los datos)
    cout << "\nData Codework: ";
    for (int i = 0; i < 28; i++) {
        cout << DataCodework[i] << " ";
    }
    cout << endl;
}

vector<int> Reed_Solomon(vector<int>& DataCodework){
    int p = 285;
    int pgen[17] = {1,59,13,104,189,68,209,30,8,163,65,41,229,98,50,36,59};
    vector <int> bytesDeParidad;
    for(int i=0;i<28;i++){
        int c = DataCodework[i];
        if (c != 0){
            for(int j=0; j<17;j++){
                int num = pgen[j];
                int aux = c;
                int mul = 0;

                for(int k=0;k<8;k++){
                    if (aux & 1){
                        mul ^= num;
                    }
                    int mayor = num & 128;
                    num <<= 1;
                    if(mayor){
                        num ^= p;
                    }
                    num &= 255;
                    aux >>= 1;
                    
                }
                DataCodework[i+j] ^= mul;
            }
        }
    }
    for (int i=28;i<44;i++){
        bytesDeParidad.push_back(DataCodework[i]);
    }
    return bytesDeParidad;
}

vector<int> bytesAbits(const vector<int>& BytesCorreccion) {
    vector<int> bits128;
    bits128.reserve(128); // optimización

    for (int num : BytesCorreccion) {
        for (int i = 7; i >= 0; i--) {
            int bit = (num >> i) & 1;
            bits128.push_back(bit);
        }
    }

    return bits128;
}


void inicializarMatriz(){
    for ( i = 0; i < TAMANO_QR; i++) {
        for ( j = 0; j < TAMANO_QR; j++) {
            matriz_qr[i][j] = VACIO; 
        }
    }
}

void dibujarBordeExterno(){
    int BORDE_ANCHO = 1;
    for (i = 0; i < BORDE_ANCHO; i++){
        for (j = 0; j < TAMANO_QR; j++){
            matriz_qr[i][j] = BLANCO1;
        }
    }
    for (i = TAMANO_QR - BORDE_ANCHO ; i < TAMANO_QR; i++){
        for (j = 0; j < TAMANO_QR; j++){
            matriz_qr[i][j] = BLANCO1;
        }
    }
    for (j = 0; j < BORDE_ANCHO; j++){
        for (i = 0; i < TAMANO_QR; i++){
            matriz_qr[i][j] = BLANCO1; 
        }
    }
        for (j = TAMANO_QR - BORDE_ANCHO ; j < TAMANO_QR; j++){
        for (i = 0; i < TAMANO_QR; i++){
            matriz_qr[i][j] = BLANCO1; 
        }
    }
}
void dibujarFinderPattern(int FILA_INICIO, int COLUMNA_INCIO) {
    for (int i = 0; i < 7; ++i) { 
        for (int j = 0; j < 7; ++j) { 
            if (i == 0 || i == 6 || j == 0 || j == 6 || (i > 1 && i < 5 && j > 1 && j < 5)) 
            {
                matriz_qr[FILA_INICIO + i][COLUMNA_INCIO + j] = NEGRO1; 
            } else {
                matriz_qr[FILA_INICIO + i][COLUMNA_INCIO + j] = BLANCO1; 
            }
        }
    }
}
void dibujarFinderPattern_2(int FILA_INICIO, int COLUMNA_INCIO){
    for (int i=FILA_INICIO; i < 9; i++){
            matriz_qr[FILA_INICIO + i][COLUMNA_INCIO] = BLANCO1;
    }
        for (int j=FILA_INICIO; j < 9; j++){
            matriz_qr[COLUMNA_INCIO][FILA_INICIO + j] = BLANCO1;
    }
}
void dibujarFinderPattern_3(int FILA_INICIO, int COLUMNA_INCIO){
    for (int j=COLUMNA_INCIO; j <= TAMANO_QR - 1; j++){
            matriz_qr[FILA_INICIO][j] = BLANCO1;
    }
}
void dibujarFinderPattern_4(int FILA_INICIO, int COLUMNA_INCIO){
    for (int i=FILA_INICIO; i <= TAMANO_QR - 1; i++){
            matriz_qr[i][COLUMNA_INCIO] = BLANCO1;
    }
}
void PatronDeTemporizador(){ 
    const int INDICE_FIJO = 7;       
    for (int j = 9; j <= 19; ++j) {
            if (j % 2 == 1) {
                matriz_qr[INDICE_FIJO][j] = NEGRO1;
            } else {
                matriz_qr[INDICE_FIJO][j] = BLANCO1;
            }
    }
    for (int i = 9; i <= 19; ++i) {
            if (i % 2 == 1) {
                matriz_qr[i][INDICE_FIJO] = NEGRO1;
            } else {
                matriz_qr[i][INDICE_FIJO] = BLANCO1;
            }
    }
}
void PatronDeAlineacion(){
    for (int j = 17; j <= 21;  j++){
        for (int i = 17; i <= 21; i++){
            if (i == 17 || i == 21 || j == 17 || j == 21 || (i == 19 && j == 19)) 
            {
                matriz_qr[i][j] = NEGRO1; 
            } else {
                matriz_qr[i][j] = BLANCO1; 
            }
        }
        }
    }
void Elementonegro1() {
    matriz_qr[18][9] = NEGRO1; 
}

//FRANJAS DE FORMATO
void FranjasDeFormato(int fila, int columna){
    for (int i = fila; i<= fila + 8; i++){
        if (i==1 || i==2 || i==9 || i==19 || i==24 || i==26) matriz_qr[i][9] = BLANCO1;
        else matriz_qr[i][9] = NEGRO1;
    }
    for (int j = columna; j <= columna + 7; j++){
        if (j==2 || j==3 || j==4 || j==5 ||j==6 || j==8 || j==9|| j==18 || j==24 || j==25) matriz_qr[9][j] = BLANCO1;
        else matriz_qr[9][j] = NEGRO1;
    }
}

//Binario y tamaño de url
void InferiorDerecha (string url){
    //definir lenguaje
    matriz_qr[25][25] = BLANCO1;
    matriz_qr[25][24] = NEGRO1;
    matriz_qr[24][25] = BLANCO1;
    matriz_qr[24][24] = BLANCO1;
    //Tamaño de la url
    bitset<8> tam = url.size();
    int j = 6;
    for (int i = 23; i >= 20; i--){
        if (tam[j] == 1) matriz_qr[i][24] = NEGRO1;
        else matriz_qr[i][24] = BLANCO1;
        j -= 2;
    }
    int s = 7;
    for (int k = 23; k >= 20; k--){
        if (tam[s] == 1) matriz_qr[k][25] = NEGRO1;
        else matriz_qr[k][25] = BLANCO1;
        s -= 2;
    }
}

void zigzaig(string url, vector<bitset<8>>& binario, vector<int>& bitsDeParidad) {
    // Funciones de elementos fijos
    inicializarMatriz();
    dibujarBordeExterno();
    dibujarFinderPattern(1, 1); 
    dibujarFinderPattern(1, 19);  
    dibujarFinderPattern(19, 1); 
    dibujarFinderPattern_2(0, 8);
    dibujarFinderPattern_2(0, 18);
    dibujarFinderPattern_2(18, 0);
    dibujarFinderPattern_3(8, 19);
    dibujarFinderPattern_4(19, 8);
    PatronDeTemporizador();
    PatronDeAlineacion();
    Elementonegro1();
    FranjasDeFormato(1, 1);
    FranjasDeFormato(18, 18);
    InferiorDerecha(url);

    int cont_paridad = 0;  // Variable para contar los bits de paridad
    int i, j, k, s, a, contk, conts;
    i = 25;
    j = 25;
    int posicion = 1; // 1 significa que viene de abajo 
    a = 1;
    int direccion = 1; // 1 significa dirección de subir
    int cont_letras = 0;
    int cont_bits2 = 0;
    int cont_bits = 7;
    int y = 0;
    int separacion[4] = {0, 0, 0, 0};

    // Llenado de la matriz QR con los bits de la URL
    while (j >= 0) { // Primer while para saber que estamos dentro del rango que queremos rellenar
        if (matriz_qr[i][j] == VACIO) {
            // RELLENAR
            if (cont_letras < binario.size()) { // Nos aseguramos de solo imprimir los bits totales del arreglo binario
                matriz_qr[i][j] = binario[cont_letras][cont_bits]; // Rellenamos la matriz
                cont_bits--; // Avanza al siguiente bit
                if (cont_bits < 0) {
                    cont_bits = 7;
                    cont_letras++;
                }
            }
            else if (cont_bits2 < 4) {
                matriz_qr[i][j] = separacion[y];
                y++;
                cont_bits2++;
            } else if (cont_paridad < 128){
                matriz_qr[i][j] = bitsDeParidad[cont_paridad++];
            }
        }

        if (direccion == 1) { // Primer IF: para saber si estamos subiendo
            if (i >= 1) { // IF 1.1 para saber si no hemos llegado al límite superior
                if (posicion == 0) { // IF 1.2: posición 0 que significa que tenemos que hacer un movimiento diagonal
                    j = j + 1; // Nos movemos una unidad a la derecha
                    i = i - 1; // Nos movemos una unidad hacia arriba
                    posicion = 1; // 1 significa que viene de abajo
                }   
                else { // ELSE 1.2: posición 1 que significa que tenemos que hacer un movimiento a la izquierda  
                    j = j - 1; // Nos movemos una unidad a la izquierda
                    posicion = 0; // 0 significa que viene de derecha
                }
            }   
            else { // ELSE 1.1 para cambiar la dirección en la que vamos 
                direccion = 0; // 4 significa que comenzamos a bajar
                i = i + 1;   // Nos movemos una unidad hacia abajo (para hacer un ajuste en la coordenada "i")
                j = j - 2;
                posicion = 1;
            }   
        }   
        else { // Primer ELSE: para saber que estamos bajando
            if (i <= 25) { // IF 2.1 para saber si no hemos llegado al límite inferior
                if (posicion == 0) { // IF 2.3: posición 0 que significa que tenemos que hacer un movimiento en diagonal
                    j = j + 1; // Nos movemos una unidad a la derecha
                    i = i + 1;  // Nos movemos una unidad hacia abajo
                    posicion = 1; // 1 significa que viene de abajo
                }   
                else {  // ELSE 2.3: posición 1 que significa que tenemos que hacer un movimiento hacia la izquierda
                    j = j - 1; // Nos movemos una unidad hacia la izquierda
                    posicion = 0; // 0 significa que viene de derecha
                }
            }  
            else { // ELSE 2.1 para saber cambiar de dirección
                direccion = 1; // 3 significa que comenzamos a subir
                i = i - 1;    // Nos movemos una unidad hacia arriba para hacer el ajuste en la coordenada "i"
                j = j - 2;
                posicion = 1;
            } 
        } 
    }

}

// Funciones
void imprimirQR() {
    cout << "\n";
    for (int i = 0; i < TAMANO_QR; ++i) {
        for (int j = 0; j < TAMANO_QR; ++j) {
            if (matriz_qr[i][j] == NEGRO || matriz_qr[i][j] == NEGRO1) {
                cout << "  ";
            } else if (matriz_qr[i][j] == BLANCO || matriz_qr[i][j] == BLANCO1) {
                cout << "██";
            } else {
                cout << "--"; 
            }
        }
        cout << "\n";
    }
}

//Mascara
void mascara(){
    //LLENADO DE LA MATRIZ EN ZIGZAG
    for (int i = 1; i <= 25; ++i) {
        for (int j = 1; j <= 25; ++j) {
            if(matriz_qr[i][j]==BLANCO  || matriz_qr[i][j]==NEGRO){
                if(j==1 || j==4 || j==10 || j==13 || j==16 || j==19 || j==22 || j==25){
                    if (matriz_qr[i][j]==BLANCO)
                    {
                        matriz_qr[i][j]=NEGRO;
                    }    else{
                        matriz_qr[i][j]=BLANCO;
                        }
                }
            }
        }
    }
}

// INT MAIN
int main() {

    vector<int> BytesCorreccion;



    cout << "INTRODUCE UNA LIGA\n";
    cin >> url;
    cout << "\n" << url.size() << "\n";

    // STRING A BINARIO
    int x = string_a_binario(url, ascii, binario); // Función mandada a llamar

    // Impresión de la liga en binario para fines prácticos
    for (int i = 0; i < binario.size(); i++) {
        cout << binario[i] << "  ";
    }

    //lLamada a DataCodework 
    data_codework(url, DataCodework, bitsDataCodework, ascii, binario);

    //Ponemos los bytes de reedsolomon en un vector
    BytesCorreccion = Reed_Solomon(DataCodework);
 
    //Convertimos esos bytes a bits
    bitsDeParidad = bytesAbits(BytesCorreccion);

    // Llamada a la función zigzaig para crear el QR, pasando los bitsDeParidad
    zigzaig(url, binario, bitsDeParidad);

    //Aplicamos la mascara
    mascara();
    // Imprimir el código QR
    imprimirQR();

    // Prueba recorrido bitset
    return 0;
}