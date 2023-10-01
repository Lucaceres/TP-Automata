#include <stdio.h>
#include <string.h>

// definicion formal de un automata deterministico
#define MAX_CAR 50
#define MAX_EST 50
#define MAX_LON_NOM 50
#define MAX_STR_IN 100
#define MAX_LON_PALABRA 50
#define MAX_LON_NUM 7

struct AF
{
    char alfabeto[MAX_CAR][MAX_CAR];
    char estados[MAX_EST]; // el nombre de los estados son caracteres
    char estado_inicial;
    char estados_finales[MAX_EST];
    // cada posicion de las columnas corresponde con la posicion de los caracteres del alfabeto
    // cada posicion de las filas corresponde con la posicion de los nombres de los estados
    char transiciones[MAX_EST][MAX_CAR];
};

// Automatas
struct AF decimal = {
    {{'+', '-'}, {'1', '2', '3', '4', '5', '6', '7', '8', '9'}, {'0'}}, // alfabeto
    {'A', 'B', 'E', 'F', 'G'},                                          // estados
    'A',                                                                // estado_inicial
    {'E', 'F'},                                                         // estados_finales
    {
        {'B', 'E', 'F'},
        {'G', 'E', 'F'},
        {'G', 'E', 'E'},
        {'G', 'G', 'G'},
        {'G', 'G', 'G'}} // transiciones
};

struct AF octal = {
    {{'0'}, {'1', '2', '3', '4', '5', '6', '7'}}, // alfabeto
    {'A', 'B', 'D', 'E'},                         // estados
    'A',                                          // estado_inicial
    {'D'},                                        // estados_finales
    {
        {'B', 'E'},
        {'D', 'D'},
        {'D', 'D'},
        {'E', 'E'}} // transiciones
};

struct AF hexadecimal = {
    {{'0'}, {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F'}, {'x', 'X'}}, // alfabeto
    {'G', 'H', 'I', 'J', 'K'},                                                                                                      // estados
    'G',                                                                                                                            // estado_inicial
    {'J'},                                                                                                                          // estados_finales
    {
        {'H', 'K', 'K'},
        {'K', 'K', 'I'},
        {'J', 'J', 'K'},
        {'J', 'J', 'K'},
        {'K', 'K', 'K'}} // transiciones
};

struct AF numero_entero = {
    {{'+', '-'}, {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}}, // alfabeto
    {'A', 'B', 'C', 'D'},                                             // estados
    'A',                                                              // estado_inicial
    {'C'},                                                            // estados_finales
    {
        {'B', 'C'},
        {'D', 'C'},
        {'D', 'C'},
        {'D', 'D'}} // transiciones
};

struct AF expresion_matematica = {
    {{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}, {'-'}, {'+', '*', '/'}}, // alfabeto
    {'A', 'B', 'C', 'D'},                                                         // estados
    'A',                                                                          // estado_inicial
    {'C'},                                                                        // estados_finales
    {
        {'C', 'B', 'D'},
        {'C', 'D', 'D'},
        {'C', 'A', 'A'},
        {'D', 'D', 'D'}} // transiciones
};

// CONTADORES
struct contador_AF
{
    char nombre_automata[MAX_LON_NOM];
    struct AF automata;
    int palabras_reconocidas;
};

void ejecucionPuntoUno();
void ejecucionPuntoDos();
void ejecucionPuntoTres();

// funciones string utiles:
int str_vacio(char *str);                        // indica si el string esta vacio.
int fin_str(char caracter);                      // indica si es fin de cadena '\0'.
char *str_reset(char *str);                      // resetea todo los valores a '\0'.
char *strcatchar(char *str_dest, char caracter); // concatena un caracter al string.

// funciones para el automata:
int reconoce(struct AF automata, char *palabra);                         // indica si reconoce la palabra
char transicion(struct AF automata, char estado_partida, char caracter); // devuelve el estado de trancicion
int posicion_alfabeto(struct AF automta, char caracter);                 // devuelve la posicion del primer conjunto del alfabeto que contenga el caracter
int posicion_str(char *str, char caracter);                              // devuelve la posicion de la primera ocurrencia del caracter en str

// funcion similar a atoi:
int stoi(char *str_num); // conversor de string a entero

// funciones para la lectura de los caracteres de entrada
int get_palabra(char *str_dest);                                                                              // escribe la sigiente palabra a leer y devuelve 0 si es la ultima palabra.
void sumar_reconocidos(struct contador_AF *contadores, int cant_contadores, char *palabra, int *no_reconoce); // recibe una palbra y actualiza los contadores de la lista de contadores.
void mostrar_contadores(struct contador_AF *contadores, int cant_contadores, int *no_reconoce);               // muestra en pantalla la cantidad de palabras reconocidas por cada automata

// funciones de la calculadora:
int es_expresion_matematica(char *exp_mat);
int es_operador(char caracter);
void vectorizar_expresion(int *operandos, int *c_operandos, char *operadores, char *exp_mat); // separa la expresion en vectores de operandos y operandores.
int es_complemento(char *exp_mat, int indice);                                                // devuelve verdadero si el caracter es un '-' que indica que es complemento del numero.
int calcular(int *operandos, int *c_operandos, char *operadores);                             // recibe los vectores y devuelve el resultado.
int precedencia(char operador);                                                               // recibe un operando y devuelve el nivel de precedencia.
char *eliminar_operador(char *operadores, int indice);                                        // borra el operador indicado y acomoda el string
void eliminar_operando(int *operandos, int *c_operandos, int indice);                         // borra el operando indicado y acomoda el vector
int operar(int op1, int op2, char operador);                                                  // devuelve el resultado de la operacion.
int resolver(char *exp_mat);                                                                  // recibe una expresion matematica y devuelve el resultado.

int main()
{
    int opcion;
    opcion = -1;
    printf("\n MENU DE OPCIONES: \n (0) Salir \n (1) Dada una cadena, reconocer los grupos de constantes \n (2) Convertir cadena a numero entero \n (3) Evaluar una expresion aritmetica ingresada como cadena \n");
    
    do
    {
        printf("\nIngrese una opcion: ");
        scanf("%d", &opcion);
        getchar(); // saca el '\n' para que no afecte al resto del programa
        printf("\n");

        switch (opcion)
        {
        case 0:
         printf("\nTERMINANDO EJECUCION");
            return 0; // Termina el programa

        case 1:
            // ***************************** comienzo de punto 1 ***********************************
            printf("\n OPCION SELECCIONADA: (1) Dada una cadena, reconocer los grupos de constantes \n");
            ejecucionPuntoUno();
            break;

        case 2:

            // ***************************** comienzo de punto 2 ***********************************
            printf("\n OPCION SELECCIONADA: (2) Convertir cadena a numero entero \n");
            ejecucionPuntoDos();
            break;

        case 3:

            // ***************************** comienzo de punto 3 ***********************************
            printf("\n OPCION SELECCIONADA: (3) Evaluar una expresion aritmetica ingresada como cadena\n");
            ejecucionPuntoTres();
            break;

        default:
            printf("\n ERROR! Opcion ingresada no valida\n");
            break;
        }
        
        opcion = -1;
    } while (opcion);

    return 0;
}

//--------------------------------------------------------------------------------------------------

int str_vacio(char *str)
{
    return (!strlen(str));
}

int fin_str(char caracter)
{
    return (caracter == '\0');
}

char *str_reset(char *str)
{
    for (int i = 0; !fin_str(str[i]); i++)
    {
        str[i] = '\0';
    }

    return str;
}

char *strcatchar(char *str_dest, char caracter)
{
    char str_aux[2] = {caracter, '\0'};
    return strcat(str_dest, str_aux);
}

int reconoce(struct AF automata, char *palabra)
{
    char estado_actual = automata.estado_inicial;

    for (int i = 0; !fin_str(palabra[i]); i++)
    {
        estado_actual = transicion(automata, estado_actual, palabra[i]);
    }

    return (posicion_str(automata.estados_finales, estado_actual) != -1);
}

char transicion(struct AF automata, char estado_partida, char caracter)
{
    // la posicion correspondiente en la tabla de transiciones
    int pos_estado_partida = posicion_str(automata.estados, estado_partida);
    int pos_caracter = posicion_alfabeto(automata, caracter);

    if (pos_estado_partida != -1 && pos_caracter != -1)
    {
        return automata.transiciones[pos_estado_partida][pos_caracter];
    }

    return '\0';
}

int posicion_alfabeto(struct AF automata, char caracter)
{
    for (int i = 0; !str_vacio(automata.alfabeto[i]); i++)
    {
        if (posicion_str(automata.alfabeto[i], caracter) != -1)
        {
            return i;
        }
    }

    return -1;
}

int posicion_str(char *str, char caracter)
{
    for (int i = 0; !fin_str(str[i]); i++)
    {
        if (str[i] == caracter)
        {
            return i;
        }
    }
    return -1;
}

int get_palabra(char *str_dest)
{
    int c = getchar();

    str_reset(str_dest);
    while (c != EOF && c != '\n' && c != '$')
    {
        strcatchar(str_dest, c);
        c = getchar();
    }

    if (c == EOF || c == '\n')
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int stoi(char *str_num)
{
    int signo = 1;  // variable para saber el signo de numero
    int numero = 0; // numero de salida

    if (reconoce(numero_entero, str_num))
    {
        for (int i = 0; !fin_str(str_num[i]); i++)
        {
            if (str_num[i] == '-')
            {
                signo = -1;
            }
            else if (str_num[i] != '+')
            {
                numero *= 10;
                numero += (int)str_num[i] - 48;
            }
        }

        return numero * signo;
    }

    printf("\n\"%s\" no se reconoce como numero entero.\n", str_num);
    return 0;
}

void vectorizar_expresion(int *operandos, int *c_operandos, char *operadores, char *exp_mat)
{
    int i = 0;
    char str_num[MAX_LON_NUM];
    str_reset(str_num);
    *c_operandos = 0;
    str_reset(operadores);

    while (!fin_str(exp_mat[i]))
    {
        // bucle para conseguir operandos.
        while ((!es_operador(exp_mat[i]) || es_complemento(exp_mat, i)) && !fin_str(exp_mat[i]))
        {

            strcatchar(str_num, exp_mat[i]);
            i++;
        }

        operandos[*c_operandos] = stoi(str_num);
        (*c_operandos)++;
        str_reset(str_num);

        // para conseguir operadores
        if (!fin_str(exp_mat[i]))
        {
            if (!es_complemento(exp_mat, i) && es_operador(exp_mat[i]))
            {
                strcatchar(operadores, exp_mat[i]);
            }
            i++;
        }
    }
}

int es_complemento(char *exp_mat, int indice)
{
    if (exp_mat[indice] == '-')
    {
        if (indice == 0)
        {
            return 1;
        }
        else if (es_operador(exp_mat[indice - 1]))
        {
            return 1;
        }
    }

    return 0;
}

int es_expresion_matematica(char *exp_mat)
{
    return reconoce(expresion_matematica, exp_mat);
}

int es_operador(char caracter)
{
    if (caracter == '+' ||
        caracter == '-' ||
        caracter == '*' ||
        caracter == '/')
    {
        return 1;
    }

    return 0;
}

int calcular(int *operandos, int *c_operandos, char *operadores)
{
    int prec_actual = 0; // nivel de precedencia actual.

    while (strlen(operadores))
    { // mientra haya operadores.
        for (int i = 0; !fin_str(operadores[i]); i++)
        {
            // empzando por la precedencia mas baja.
            if (prec_actual == precedencia(operadores[i]))
            {
                // reemplazamos por el resultado y eliminamos lo que ya se uso.
                operandos[i] = operar(operandos[i], operandos[i + 1], operadores[i]);
                eliminar_operador(operadores, i);
                eliminar_operando(operandos, c_operandos, i + 1);
                i--;
            }
        }
        prec_actual++;
    }

    return operandos[0];
}

int precedencia(char operador)
{
    switch (operador)
    {
    case '*':
    case '/':
        return 0;
    case '+':
    case '-':
        return 1;
    }

    return -1;
}

char *eliminar_operador(char *operadores, int indice)
{
    for (int i = indice; !fin_str(operadores[i]); i++)
    {
        operadores[i] = operadores[i + 1];
    }

    return operadores;
}

void eliminar_operando(int *operandos, int *c_operandos, int indice)
{
    for (int i = indice; i < *c_operandos; i++)
    {
        operandos[i] = operandos[i + 1];
    }
    (*c_operandos)--;
}

int operar(int op1, int op2, char operador)
{
    switch (operador)
    {
    case '+':
        return op1 + op2;
    case '-':
        return op1 - op2;
    case '*':
        return op1 * op2;
    case '/':
        return op1 / op2;
    }

    return 0;
}

int resolver(char *exp_mat)
{
    char operadores[MAX_STR_IN / 2];
    int operandos[MAX_STR_IN / 2];
    int c_operandos = 0;

    vectorizar_expresion(operandos, &c_operandos, operadores, exp_mat);

    return calcular(operandos, &c_operandos, operadores);
}

void sumar_reconocidos(struct contador_AF *contadores, int cant_contadores, char *palabra, int *no_reconoce)
{
    int reconocido = 0; // Variable para verificar si se reconoció al menos una palabra
    for (int i = 0; i < cant_contadores; i++)
    {
        if (reconoce(contadores[i].automata, palabra))
        {
            contadores[i].palabras_reconocidas++;
            reconocido = 1; // Se reconoció al menos una palabra
        }
    }
    if (!reconocido)
    {
        *no_reconoce = 1; // No se reconoció ninguna palabra, hubo un error léxico.
    }
}

void mostrar_contadores(struct contador_AF *contadores, int cant_contadores, int *no_reconoce)
{
    for (int i = 0; i < cant_contadores; i++)
    {
        printf("%s : %i\n", contadores[i].nombre_automata, contadores[i].palabras_reconocidas);
    }

    if (*no_reconoce)
    {
        printf("Hubo un error lexico en al menos una de las palabras ingresadas\n");
    }
    else
    {
        printf("No hubo error lexico\n");
    }
}

// Ejecucion punto 1
void ejecucionPuntoUno()
{
    char palabra[MAX_LON_PALABRA]; // string para cada palabra

    // lista de contadores
    const int cant_contadores = 3;
    int noReconocido = 0;
    struct contador_AF contadores[] = {
        {
            "DECIMAL", // nombre.
            decimal,   // automata
            0          // contador inicializado en 0.
        },
        {
            "OCTAL", // nombre.
            octal,   // automata
            0        // contador inicializado en 0.
        },
        {
            "HEXADECIMAL", // nombre.
            hexadecimal,   // automata
            0              // contador inicializado en 0.
        }};


    printf("\t ingrese cadena de palabras(separadas por \"$\"): ");

    // mientras siga habiendo palabras contamos los automatas que lo reconocen.
    while (get_palabra(palabra))
    {
        sumar_reconocidos(contadores, cant_contadores, palabra, &noReconocido);
    }
    sumar_reconocidos(contadores, cant_contadores, palabra, &noReconocido);

    // mostramos los conteos
    printf("\n\n\t\t***NUMEROS RECONOCIDOS***\n");
    mostrar_contadores(contadores, cant_contadores, &noReconocido);
}

// Ejecucion Punto 2
void ejecucionPuntoDos()
{
    char str_num[MAX_STR_IN];

    printf("\tingrese el numero (entero con o sin signo): ");

    if (fgets(str_num, MAX_STR_IN, stdin) != NULL)
    {
        *strchr(str_num, '\n') = '\0';
        printf("el string \"%s\" es: %i\n", str_num, stoi(str_num));
    }
    else
    {
        printf("\nERROR fin de archivo.\n");
    }
}

// Ejecucion Punto 3
void ejecucionPuntoTres()
{
    char exp_mat[MAX_STR_IN];

    printf("\tingrese la expresion: ");

    if (fgets(exp_mat, MAX_STR_IN, stdin) != NULL)
    {
        *strchr(exp_mat, '\n') = '\0';
        if (es_expresion_matematica(exp_mat))
        {
            printf("\n\tresultado: \"%s\" = %i\n", exp_mat, resolver(exp_mat));
        }
        else
        {
            printf("\nERROR de sintaxis: \"%s\" no es una entrada valida.\n", exp_mat);
        }
    }
    else
    {
        printf("\nERROR fin de archivo.\n");
    }
}
