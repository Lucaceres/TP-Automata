#include<stdio.h>
#include<string.h>

//definicion formal de un automata deterministico
#define MAX_CAR 50
#define MAX_EST 50
#define MAX_LON_NOM 50
#define MAX_STR_IN 100
#define MAX_LON_PALABRA 50

struct AF{
    char alfabeto[MAX_CAR][MAX_CAR];
    char estados[MAX_EST]; //el nombre de los estados son caracteres
    char estado_inicial;
    char estados_finales[MAX_EST];
    //cada posicion de las columnas corresponde con la posicion de los caracteres del alfabeto
    //cada posicion de las filas corresponde con la posicion de los nombres de los estados
    char transiciones[MAX_EST][MAX_CAR];
};



//Automatas
struct AF decimal = {
    {{'+','-'},{'1','2','3','4','5','6','7','8','9'},{'0'}},//alfabeto
    {'A','B','E','F','G'},//estados
    'A',//estado_inicial
    {'E','F'},//estados_finales
    {
            {'B','E','F'},
            {'G','E','F'},
            {'G','E','E'},
            {'G','G','G'},
            {'G','G','G'} 
     }//transiciones
};

struct AF octal = {
    {{'0'},{'1','2','3','4','5','6','7'}},//alfabeto
    {'A','B','D','E'},//estados
    'A',//estado_inicial
    {'D'},//estados_finales
    {
            {'B','E'},
            {'D','D'},
            {'D','D'},
            {'E','E'} 
     }//transiciones
};

struct AF hexadecimal = {
    {{'0'},{'1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','A','B','C','D','E','F'},{'x','X'}},//alfabeto
    {'G','H','I','J','K'},//estados
    'G',//estado_inicial
    {'J'},//estados_finales
    {
            {'H','K','K'},
            {'K','K','I'},
            {'J','J','K'},
            {'J','J','K'},
            {'K','K','K'} 
     }//transiciones
};

//CONTADORES
struct contador_AF{
    char nombre_automata[MAX_LON_NOM];
    struct AF automata;
    int palabras_reconocidas;  
};


//funciones string utiles:
int str_vacio(char *str);//indica si el string esta vacio.
int fin_str(char caracter);//indica si es fin de cadena '\0'.
char *str_reset(char *str);//resetea todo los valores a '\0'.
char *strcatchar(char *str_dest, char caracter); //concatena un caracter al string.

//funciones para el automata:
int reconoce(struct AF automata, char *palabra);//indica si reconoce la palabra
char transicion(struct AF automata, char estado_partida, char caracter); //devuelve el estado de trancicion
int posicion_alfabeto(struct AF automta, char caracter);//devuelve la posicion del primer conjunto del alfabeto que contenga el caracter
int posicion_str(char *str, char caracter);//devuelve la posicion de la primera ocurrencia del caracter en str

//funciones para la lectura de los caracteres de entrada
int get_palabra(char *str_dest); //escribe la sigiente palabra a leer y devuelve 0 si es la ultima palabra.
void sumar_reconocidos(struct contador_AF *contadores, int cant_contadores, char *palabra); //recibe una palbra y actualiza los contadores de la lista de contadores.
void mostrar_contadores(struct contador_AF *contadores, int cant_contadores); //muestra en pantalla la cantidad de palabras reconocidas por cada automata


int main(){
    char palabra[MAX_LON_PALABRA];//string para cada palabra

    //lista de contadores
    const int cant_contadores = 3;
    struct contador_AF contadores[] = {
        {
            "DECIMAL", //nombre.
            decimal, //automata
            0 // contador inicializado en 0.
        },
        {
            "OCTAL", //nombre.
            octal, //automata
            0 // contador inicializado en 0.
        },
        {
            "HEXADECIMAL", //nombre.
            hexadecimal, //automata
            0 // contador inicializado en 0.               
        }
    };
    
    printf("\n\t\t***DETECTOR DE NUMEROS***\n");
    printf("cadena de palabras(separadas por \"$\"): ");

    //mientras siga habiendo palabras contamos los automatas que lo reconocen.
    while(get_palabra(palabra)){
        sumar_reconocidos(contadores,cant_contadores,palabra);
    }
    sumar_reconocidos(contadores,cant_contadores,palabra);
    
    //mostramos los conteos
    printf("\n\t\t***NUMEROS RECONOCIDOS***\n");
    mostrar_contadores(contadores,cant_contadores);

    return 0;
}

int str_vacio(char *str){
    return (!strlen(str));
}

int fin_str(char caracter){
    return (caracter == '\0');
}

char *str_reset(char *str){
    for(int i=0; !fin_str(str[i]); i++){
        str[i] = '\0';
    }

    return str;
}

char *strcatchar(char* str_dest, char caracter){
    char str_aux[2] = {caracter,'\0'};    
    return strcat(str_dest,str_aux);
}

int reconoce(struct AF automata, char *palabra){
    char estado_actual = automata.estado_inicial;
    
    for(int i=0; !fin_str(palabra[i]); i++){
        estado_actual = transicion(automata,estado_actual,palabra[i]);
    }

    return (posicion_str(automata.estados_finales, estado_actual) != -1);
}

char transicion(struct AF automata, char estado_partida, char caracter){
    //la posicion correspondiente en la tabla de transiciones
    int pos_estado_partida = posicion_str(automata.estados,estado_partida);
    int pos_caracter = posicion_alfabeto(automata,caracter);

    if(pos_estado_partida != -1 && pos_caracter != -1){
        return automata.transiciones[pos_estado_partida][pos_caracter];
    }
        
    return '\0';
}

int posicion_alfabeto(struct AF automata, char caracter){
    for(int i=0; !str_vacio(automata.alfabeto[i]); i++){
        if(posicion_str(automata.alfabeto[i],caracter) != -1){
            return i;
        }    
    }

    return -1;
}

int posicion_str(char *str, char caracter){
    for(int i=0; !fin_str(str[i]); i++){
        if(str[i] == caracter){
            return i;
        }
    }
    return -1;
}

int get_palabra(char *str_dest){
    int c = getchar();
    
    str_reset(str_dest);
    while(c != EOF && c != '\n' && c != '$'){
        strcatchar(str_dest,c);        
        c = getchar();
    }
    
    if(c == EOF || c == '\n'){
        return 0;
    }else{
        return 1;    
    }
}

void sumar_reconocidos(struct contador_AF *contadores, int cant_contadores, char *palabra){
    for(int i=0; i<cant_contadores; i++){
        if(reconoce(contadores[i].automata,palabra)){
            contadores[i].palabras_reconocidas++;
        }
    }
}

void mostrar_contadores(struct contador_AF *contadores, int cant_contadores){
    for(int i=0; i<cant_contadores; i++){
        printf("%s : %i\n", contadores[i].nombre_automata,contadores[i].palabras_reconocidas); 
    }
}
