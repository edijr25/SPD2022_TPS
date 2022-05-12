//--- DEFINES ---//
#define BOTON_START 2            //? Pin del boton start
#define BOTON_RESET 3    		 //? Pin del boton reset
#define PRIMER_LED 4             //? Primer pin de los leds
#define ULTIMO_LED 13             //? Ultimo pin de los leds
#define BASE_MILI_SEGUNDOS 1000 //? Segundos expresados en milisegundos
#define MAX_SEGUNDOS 1023        //! Maxima cantidad de segundos a contar
//--- FIN DEFINES--//

//--- DECLARACIONES DE FUNCIONES ---//
/// void pasarABinario
/// Convierte números enteros a numeros binarios.
/// @param numero - numero a convertir a binario.
/// @param numeroBinario - arreglo donde se almacenará el número binario.
void pasarABinario(int numero, int numeroBinario[]);

/// bool pasaronLosSegundos
/// Determina si paso el tiempo necesario, expresado en milisegundos.
/// @param tiempoAPasar - milisegundos que representan el tiempo que debe transcurrir.
/// return
///   true - si transcurrieron los milisegundos necesarios.
///   false - si aún no transcurrieron los milisegundos necesarios.
bool pasaronLosSegundos(unsigned int tiempoAPasar);

/// void controlarLeds
/// Controla el encendido y el apagado de los leds, en base al número binario a representar.
/// @param numeroBinario - numero binario a representar.
void controlarLeds(int numeroBinario[]);

/// void reiniciarNumeroBinario
/// Reinicia el arreglo que representa el número binario, dejando todos sus campos (bits) a cero.
/// @param numeroBinario - numero binario al que se le va a reiniciar su valor.
void reiniciarNumeroBinario(int numeroBinario[]);

/// void mostrarNumero
/// Muestra a través del monitor en serie el segundo que se está contando en ese momento, junto con su representación en binario.
/// @param segundos - segundo que está transcurriendo en ese momento.
/// @param numeroBinario - representación binaria del segundo en cuestión.
void mostrarNumero(int segundos, int numeroBinario[]);
//--- FIN DECLARACIONES DE FUNCIONES ---//

void setup(){
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
  Serial.begin(9600);
}

int numeroBinario[10] = {0,0,0,0,0,0,0,0,0,0};
int contadorSegundos = 0;
int botonStartAntes = 0;
int botonRestartAntes = 0;
int diferenciaEnPausa = 0;
unsigned long millisAntes;
bool estaPausado = true;

void loop(){
  int botonStartAhora = digitalRead(BOTON_START);
  int botonRestartAhora = digitalRead(BOTON_RESET);

  if (botonStartAhora == 1 && botonStartAntes == 0) {
  	estaPausado = !estaPausado;
    if (estaPausado) {
      diferenciaEnPausa = millis() - millisAntes;
    } else {
      millisAntes = millis() - diferenciaEnPausa;
    }
  }
  
  if (botonRestartAhora == 1 && botonRestartAntes == 0) {
    contadorSegundos = 0;
    reiniciarNumeroBinario(numeroBinario);
    controlarLeds(numeroBinario);
    millisAntes = millis();
  }
   
  if (!estaPausado && pasaronLosSegundos(BASE_MILI_SEGUNDOS)) {
    contadorSegundos++; 
    if (contadorSegundos < MAX_SEGUNDOS + 1) {
      pasarABinario(contadorSegundos, numeroBinario);
      controlarLeds(numeroBinario);
      mostrarNumero(contadorSegundos, numeroBinario); 
    } else if (contadorSegundos == MAX_SEGUNDOS + 1) {
      reiniciarNumeroBinario(numeroBinario);
      controlarLeds(numeroBinario); 
    }  
  }
  
  botonStartAntes = botonStartAhora;
  botonRestartAntes = botonRestartAhora;
  delay(5); //Necesario para el buen funcionamiento de Tinkercad
}

//--- DEFINICIONES DE FUNCIONES ---//
void pasarABinario(int numero, int numeroBinario[]) { 
  for (int i = 9; i >= 0; i--) {
    if (numero < 1) {
      break;
    }
    numeroBinario[i] = numero % 2;
    numero = numero / 2;
  }
}

bool pasaronLosSegundos(unsigned int tiempoAPasar) {
  unsigned long millisAhora = millis();
  
  if (millisAhora - millisAntes >= tiempoAPasar) {
    millisAntes = millisAhora;
  	return true;
  }
  return false;
}

void controlarLeds(int numeroBinario[]) {
  int pos = 0;
  for (int i = ULTIMO_LED; i >= PRIMER_LED; i--) {
    int estado = numeroBinario[pos];
    digitalWrite(i, estado);
    pos++;
  }
}

void mostrarNumero(int segundos, int numeroBinario[]) {
  Serial.print("Seconds: ");
  Serial.print(segundos);
  Serial.print(" | ");
  Serial.print("Binary: ");
  for (int i = 0; i < 10; i++) {
    Serial.print(numeroBinario[i]);
  }
  Serial.println("");
}

void reiniciarNumeroBinario(int numeroBinario[]) {
  for (int i = 0; i < 10; i++) {
    numeroBinario[i] = 0;
  }
}
//--- FIN DEFINICIONES DE FUNCIONES ---//