## Algoritmos de codificación REED_SOLOMON


### Data Codework
Para empezar a trabajar la función Reed-Solomon debemos ejecutar desde antes una función llamada Data Codework

 ##### Parámetros necesarios de la función:
 Estos son los datos que le vamos a mandar a la función, mismos que vamos a manipular hasta obtener nuestro objetivo
 - URL
 - DataCodework
 - bitsDataCodework
 - binario

 Así:
 ```cpp
vector <bitset<8>> binario(26);
vector <int> DataCodework(44);
vector <int> bitsDataCodework(224);
 ```

### Objetivo
El objetivo de la función es obtener un vector de 44 elementos, en ese caso, enteros. A todo esto, algunos de los vectores usados en la función son definidos en ella, sin embargo, fueron declarados antes del `int main ()` para una declaración global.

```cpp
void data_codework (string url, vector<int> &DataCodework, vector<int> &bitsDataCodework, vector<bitset<8>>& binario) {
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
```

```cpp
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
```
### Polinomio primitivo
#### En el código
Se utiliza un polinomio primitivo para realizar las operaciones en el campo GF(2^8).

En el código de la función Reed_Solomon, el polinomio primitivo se define de la siguiente manera:
```int p = 285;```
Este p = 285 es el valor que representa el polinomio primitivo utilizado en el campo GF(2^8). El valor de 285 corresponde al polinomio binario x^8 + x^4 + x^3 + x + 1, que es un polinomio primitivo comúnmente utilizado en muchos algoritmos de corrección de errores, incluido Reed-Solomon.

#### Campo de Galois
El concepo clave es un **_conjunto cerrado_**,

### ¿Qué hace el polinomio primitivo?
- Reducir los resultados: El polinomio primitivo se usa para garantizar que los cálculos no se desborden fuera del rango permitido (0-255, que es el rango de un byte).

- Generar secuencias: El polinomio asegura que las operaciones dentro del campo GF(2^8) sigan siendo cíclicas y adecuadas para la corrección de errores.

- Evitar repeticiones: Un polinomio primitivo asegura que las secuencias generadas no se repitan antes de lo necesario, lo que es fundamental para la fiabilidad de la corrección de errores en los códigos Reed-Solomon.
