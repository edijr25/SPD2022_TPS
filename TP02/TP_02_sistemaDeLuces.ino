#include<LiquidCrystal.h>

//--- DEFINES ---//
#define NO_BOTON 0
#define BOTON_1  1
#define BOTON_2  2
#define BOTON_3  3
#define BOTON_4  4
#define BOTON_5  5
#define BOTON_6  6
//--- FIN DEFINES ---//

//--- DECLARACIONES DE FUNCIONES ---//
/// bool pasaronLosSegundos
/// Determina si pasó el tiempo necesario, expresado en milisegundos.
/// @param tiempoAPasar - milisegundos que representan el tiempo que debe transcurrir.
/// return
///   true - si transcurrieron los milisegundos necesarios.
///   false - si aún no transcurrieron los milisegundos necesarios.
bool pasaronLosSegundos(unsigned int tiempoAPasar);

/// void controlarAcciones
/// Controla a las acciones que ocurren de manera continua luego de accionar un pulsador:
///		- Guiño de la izquierda.
///		- Guiño de la derecha.
///		- Balizas.
///		- Luces de posición (si se activan mediante el pulsador y no mediante el fotoresistor).
/// Se encarga no solo de activar los leds y todo lo necesario, sino también de desactivarlo una vez que se vuelve a pulsar el botón.
/// @param botonAhora - representa el botón pulsado para activar una acción.
/// @param intensidadLuz - representa la luz captada por el fotoresistor.
void controlarAcciones(int botonAhora, int intensidadLuz);

/// void prenderApagarGuinios
/// Se encarga de prender y apagar los guiños izquierda y derecha del sistema de luces.
/// @param bool esIzquierda - true si se debe encender/apagar el led de la izquierda; false si se debe encender/apagar el led de la derecha.
void prenderApagarGuinios(bool esIzquierda);

/// void limpiarLedsYPantalla
/// Limpia la pantalla al finalizar una determinada acción y apaga los leds correspondientes de la misma. También coloca el estado del led
/// las balizas y guiños en cero.
/// @param int accion - representa la acción a limpiar.
void limpiarLedsYPantalla(int accion);

/// bool esUnaAccionActiva
/// Determina si una acción está activa, es decir, si está ocurriendo en ese momento, y también la borra en caso de que se indique.
/// @param int accion - representa a la acción que se busca saber si está activa.
/// @param bool borrar - true si se debe borrar la acción en cuestión; false si no debe ser borrada.
bool esUnaAccionActiva(int accion, bool borrar);

/// void setAccionActiva
/// Coloca una acción en estado activo.
/// @param int accion - representa la acción que se busca activar.
void setAccionActiva(int accion);

/// void comprobarSimultainedadLeds
/// Verifica que las acciones de los guiños y las balizas no estén activadas al mismo tiempo. en caso de estarlo,
/// quita la acción anterior y la reemplaza por la nueva para que solo ocurra una a la vez.
/// @param int accion - acción que se busca activar.
void comprobarSimultainedadLeds (int accion);

/// void encenderPantalla
/// Indica el estado de las luces al momento de encender la pantalla. 
void encenderPantalla();

/// void encenderLedsDePosicion
/// Enciende los leds utilizadas como luces de posición.
void encenderLedsDePosicion();

/// void encenderApagarFrenos
/// Enciende los frenos al momento de mantener pulsado alguno de los botones de frenado, y
/// los apaga al momento de soltarlo.
/// @param bool encender - indica si el led de freno debe encenderse o apagarse.
void encenderApagarFrenos(bool encender);

/// void realizarAcciones
/// Realiza las acciones activas, prendiendo y apagando los leds necesarios y mostrando por
/// pantalla lo que corresponda.
/// @param botonAhora - representa el botón pulsado para realizar una acción.
/// @param intensidadLuz - representa la luz captada por el fotoresistor.
void realizarAcciones(int botonAhora, int intensidadLuz);
//--- FIN DECLARACIONES DE FUNCIONES ---//

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

void setup()
{  
  pinMode(A0, INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  lcd.begin(16, 2);
  encenderPantalla();
}

int botonAntes;
unsigned long int millisAntes;
int accionesActivas[] = {0, 0};
int estadoLed = 0;

void loop()
{
  int intensidadLuz = map(analogRead(A1), 1023, 931, 0, 100);
  int botonAhora = leerBoton();
  
  controlarAcciones(botonAhora, intensidadLuz);
  realizarAcciones(botonAhora, intensidadLuz);  
  
  botonAntes = botonAhora;
  delay(15);
}

//--- FUNCIONES ---//
int leerBoton(){
  int valorA0 = analogRead(A0);
  
  if(valorA0>502 && valorA0<522)
    return BOTON_1;
  if(valorA0>672 && valorA0<692)
    return BOTON_2;
  if(valorA0>757 && valorA0<777)
    return BOTON_3;
  if(valorA0>808 && valorA0<828)
    return BOTON_4; 
  if(valorA0>842 && valorA0<862)
    return BOTON_5; 
  if(valorA0>867 && valorA0<887)
    return BOTON_6;
  
  return NO_BOTON;
}

bool pasaronLosSegundos(unsigned int tiempoAPasar) {
  unsigned long millisAhora = millis();
  
  if (millisAhora - millisAntes >= tiempoAPasar) {
    millisAntes = millisAhora;
    return true;
  }
  return false;
}

void encenderPantalla() {
  lcd.setCursor(0, 1);
  lcd.print("LUCES: OFF");
}

void controlarAcciones(int botonAhora, int intensidadLuz) {
  if (botonAhora != NO_BOTON && botonAhora != botonAntes) {
    switch(botonAhora){
      case 1:
      case 2:
      case 6:
        if (esUnaAccionActiva(botonAhora, true)) {
          limpiarLedsYPantalla(botonAhora);
        } else {
          comprobarSimultainedadLeds(botonAhora);
          setAccionActiva(botonAhora);
        }
      break;
      case 3:
        if (intensidadLuz > 50) {
          if (esUnaAccionActiva(botonAhora, true)) {
            limpiarLedsYPantalla(botonAhora);
          } else {
            setAccionActiva(botonAhora);
	    encenderLedsDePosicion();
          }
        }
      break;
    }  
  }
}

void realizarAcciones(int botonAhora, int intensidadLuz) {
  if(pasaronLosSegundos(1000)) {
    if (esUnaAccionActiva(1, false)) {
      prenderApagarGuinios(true);
    } else if (esUnaAccionActiva(6, false)) {
      prenderApagarGuinios(false);
    } else if (esUnaAccionActiva(2, false)) {
      prenderApagarBalizas();
    }  
  }

  if (!esUnaAccionActiva(3, false)) {
    if ((intensidadLuz < 50)) {
      encenderLedsDePosicion();     
    } else if (intensidadLuz > 50) {
      limpiarLedsYPantalla(3);
    }
  }
  
  if (botonAhora == NO_BOTON && (botonAntes == BOTON_4 || botonAntes == BOTON_5)) {
    encenderApagarFrenos(false);
  } else if (botonAhora == BOTON_4 || botonAhora == BOTON_5) {
    encenderApagarFrenos(true);
  }
}

bool esUnaAccionActiva(int accion, bool borrar) {
  for (int i = 0; i < 2; i++) {
    if (accionesActivas[i] == accion) {
      if (borrar) 
        accionesActivas[i] = 0;      
      return true;
    }
  }
  return false;
}

void setAccionActiva(int accion) {
  for (int i = 0; i < 2; i++) {
    if (accionesActivas[i] == 0) {
      accionesActivas[i] = accion;
      break;
    }
  }
}

void prenderApagarGuinios(bool esIzquierda) {
  int pinLed;
  
  if (esIzquierda) {
    pinLed = 10;
    lcd.setCursor(0,0);
    if (!estadoLed) {
      lcd.print("<");
    } else {
      lcd.print(" ");
    }
  } else {
    pinLed = 9;
    lcd.setCursor(15,0);
    if (!estadoLed) {
      lcd.print(">");
    } else {
      lcd.print(" ");
    }
  }
  estadoLed = !estadoLed;
  digitalWrite(pinLed, estadoLed);
}

void prenderApagarBalizas() {
  if (estadoLed) {
    lcd.setCursor(0, 0);
    lcd.print(" ");
    lcd.setCursor(15, 0);
    lcd.print(" ");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("<");
    lcd.setCursor(15, 0);
    lcd.print(">");
  }
  estadoLed = !estadoLed;
  digitalWrite(9, estadoLed);
  digitalWrite(10, estadoLed); 
}

void encenderLedsDePosicion() {
  digitalWrite(8, 1);
  digitalWrite(11, 1);
  lcd.setCursor(7, 1);
  lcd.print("ON ");
}

void encenderApagarFrenos(bool encender) {
  if (encender) {
    lcd.setCursor(7,0);
    lcd.print("!!");
    digitalWrite(12, HIGH); 
  } else {
    lcd.setCursor(7,0);
    lcd.print("  ");
    digitalWrite(12, LOW);   
  }
}

void limpiarLedsYPantalla(int accion) {
  int pinLed;
  String texto;
  
  switch(accion) {
    case 1:
      texto = " ";
      pinLed = 10;
      lcd.setCursor(0,0);
    break;
    case 2:
      texto = " ";
      digitalWrite(10, 0);
      pinLed = 9;
      lcd.setCursor(0, 0);
      lcd.print(texto);
      lcd.setCursor(15, 0);
    break;
    case 3:
      texto = "OFF";
      digitalWrite(8, 0);
      lcd.setCursor(7, 1);
      pinLed = 11;
    break;
    case 6:
      texto = " ";
      pinLed = 9;
      lcd.setCursor(15,0);
    break;
  }
  
  if (accion == 1 || accion == 2 || accion == 6) estadoLed = 0;
  lcd.print(texto);
  digitalWrite(pinLed, 0);
}

void comprobarSimultainedadLeds (int accion) {
  int accionSimultanea1;
  int accionSimultanea2;
  
  switch(accion) {
  	case 1:
      accionSimultanea1 = 2;
      accionSimultanea2 = 6;
    break;
    case 6:
      accionSimultanea1 = 1;
      accionSimultanea2 = 2;
    break;
    case 2:
      accionSimultanea1 = 1;
      accionSimultanea2 = 6;
    break;
  }
  
  for (int i = 0; i < 2; i++) {
    if (accionesActivas[i] == accionSimultanea1 || accionesActivas[i] == accionSimultanea2) {
      limpiarLedsYPantalla(accionesActivas[i]);
      accionesActivas[i] = 0;
    }
  }
}
//--- FIN FUNCIONES ---//
