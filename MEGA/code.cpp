#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Crear el objeto lcd dirección 0x20 y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definición de pines para las cuatro puertas AND del 74HC08
int pins[] = {43, 41, 39, 37, 35, 33, 42, 40, 38, 36, 34, 32, 47, 46};
int pinSelector = 22; // Pin para seleccionar el tipo de prueba (AND u OR)
int pinExecute = 24;  // Pin para ejecutar las pruebas

const int numTests = 12; // Número de pruebas (AND, OR, etc.)
int currentTest = 0;    // Índice del test actual (0 para AND, 1 para OR, etc.)
const char* testNames[numTests] = {"74HC08", "74HC32", "74HC00", "74HC86", "74HC02", "74HC04", "74HC10", "74HC11", "74HC27", "74HC20", "74HC21", "74LS75"};

void setup() {
  lcd.init();
  lcd.clear();         
  lcd.backlight(); // Enciende la luz de fondo del LCD
  
  Serial.begin(9600);
  
  lcd.print("Test: ");
  lcd.print(testNames[currentTest]);

  pinMode(pinSelector, INPUT_PULLUP); // Configurar pin de selección con pull-up
  pinMode(pinExecute, INPUT_PULLUP);  // Configurar pin de ejecución con pull-up
}

void loop() {
  // Leer el estado actual del botón de selección
  if (digitalRead(pinSelector) == HIGH) {
    delay(5); // Espera breve para evitar lecturas erráticas
    if (digitalRead(pinSelector) == HIGH) {
      currentTest = (currentTest + 1) % numTests; // Cambia entre opciones

      // Actualizar la pantalla LCD con el tipo de prueba actual
      lcd.setCursor(0, 0);
      lcd.print("Test: ");
      lcd.print(testNames[currentTest]);
      while (digitalRead(pinSelector) == HIGH); // Espera a que se suelte el botón
    }
  }

  // Leer el estado actual del botón de ejecución
  if (digitalRead(pinExecute) == HIGH) {
    delay(5); // Espera breve para evitar lecturas erráticas
    if (digitalRead(pinExecute) == HIGH) {
      // Actualizar la pantalla LCD con el tipo de prueba actual
      lcd.setCursor(0, 0);
      lcd.print("Test: ");
      lcd.print(testNames[currentTest]);
      
      lcd.setCursor(0, 1);
      lcd.print("Running...    ");
      
      configurarPines(currentTest); // Reconfigurar los pines según el test
      
      // Ejecutar el test actual
      realizarTest(currentTest);
      while (digitalRead(pinSelector) == HIGH); // Espera a que se suelte el botón
    }
  }
}

// Función para configurar los pines según el test
void configurarPines(int test) {
  // Configuración específica para cada tipo de prueba
  switch (test) {
    case 0: // AND cuadruple
    case 1: // OR cuadruple
    case 2: // NAND cuadruple
    case 3: // XOR cuadruple
      // Configuración de pines de la puerta 1
      pinMode(pins[0], OUTPUT);
      pinMode(pins[1], OUTPUT);
      pinMode(pins[2], INPUT);

      // Configuración de pines de la puerta 2
      pinMode(pins[3], OUTPUT);
      pinMode(pins[4], OUTPUT);
      pinMode(pins[5], INPUT);

      // Configuración de pines de la puerta 3
      pinMode(pins[7], OUTPUT);
      pinMode(pins[8], OUTPUT);
      pinMode(pins[6], INPUT);

      // Configuración de pines de la puerta 4
      pinMode(pins[10], OUTPUT);
      pinMode(pins[11], OUTPUT);
      pinMode(pins[9], INPUT);
      break;
    case 4: // NOR cuadruple
      // Configuración de pines de la puerta 1
      pinMode(pins[2], OUTPUT);
      pinMode(pins[1], OUTPUT);
      pinMode(pins[0], INPUT);

      // Configuración de pines de la puerta 2
      pinMode(pins[5], OUTPUT);
      pinMode(pins[4], OUTPUT);
      pinMode(pins[3], INPUT);

      // Configuración de pines de la puerta 3
      pinMode(pins[6], OUTPUT);
      pinMode(pins[7], OUTPUT);
      pinMode(pins[8], INPUT);

      // Configuración de pines de la puerta 4
      pinMode(pins[9], OUTPUT);
      pinMode(pins[10], OUTPUT);
      pinMode(pins[11], INPUT);
      break;
    case 5: // NOT sixtuple
      // Configuración de pines de la puerta 1
      pinMode(pins[0], OUTPUT);
      pinMode(pins[1], INPUT);
      pinMode(pins[2], OUTPUT);
      pinMode(pins[3], INPUT);
      pinMode(pins[4], OUTPUT);
      pinMode(pins[5], INPUT);

      // Configuración de pines de la puerta 3
      pinMode(pins[6], INPUT);
      pinMode(pins[7], OUTPUT);
      pinMode(pins[8], INPUT);
      pinMode(pins[9], OUTPUT);
      pinMode(pins[10], INPUT);
      pinMode(pins[11], OUTPUT);
      break;
    case 6: // NAND triple
    case 7: // AND triple
    case 8: // NOR triple
      // Configuración de pines de la puerta 1
      pinMode(pins[0], OUTPUT);
      pinMode(pins[1], OUTPUT);
      pinMode(pins[11], OUTPUT);
      pinMode(pins[10], INPUT);
      
      // Configuración de pines de la puerta 2
      pinMode(pins[2], OUTPUT);
      pinMode(pins[3], OUTPUT);
      pinMode(pins[4], OUTPUT);
      pinMode(pins[5], INPUT);

      // Configuración de pines de la puerta 3
      pinMode(pins[7], OUTPUT); // A
      pinMode(pins[8], OUTPUT); // B
      pinMode(pins[9], OUTPUT); // C
	    pinMode(pins[6], INPUT); // Salida
      break;
    case 9: // NAND doble
    case 10: // AND doble
      // Configuración de pines de la puerta 1
      pinMode(pins[0], OUTPUT); // A
      pinMode(pins[1], OUTPUT); // B
      pinMode(pins[3], OUTPUT); // C
      pinMode(pins[4], OUTPUT); // D
      pinMode(pins[5], INPUT); // Salida
      
      // Configuración de pines de la puerta 2
      pinMode(pins[7], OUTPUT);
      pinMode(pins[8], OUTPUT);
      pinMode(pins[10], OUTPUT);
      pinMode(pins[11], OUTPUT);
      pinMode(pins[6], INPUT);
      break;
    case 11: // Quad bistable transparent latch
      // Configuración de pines de la puerta 1
      pinMode(pins[0], OUTPUT); // 1D
      pinMode(pins[13], INPUT); // 1Qn
      pinMode(pins[12], INPUT); // 1Q

      // Configuración de pines de la puerta 2
      pinMode(pins[1], OUTPUT); // 2D
      pinMode(pins[10], INPUT); // 2Qn
      pinMode(pins[11], INPUT); // 2Q

      // Configuración de pines de la puerta 3
      pinMode(pins[3], OUTPUT); // 3D
      pinMode(pins[8], INPUT); // 3Qn
      pinMode(pins[7], INPUT); // 3Q

      // Configuración de pines de la puerta 4
      pinMode(pins[4], OUTPUT); // 4D
      pinMode(pins[5], INPUT); // 4Qn
      pinMode(pins[6], INPUT); // 4Q

      // Configuración de pines Enable
      pinMode(pins[9], OUTPUT); // LE12
      pinMode(pins[2], OUTPUT); // LE34
      break;
    default:
      // Configuración por defecto: todos los pines como INPUT
      for (int i = 0; i < 14; i++) {
        pinMode(pins[i], INPUT);
      }
      break;
  }
}

// Función para realizar el test actual
void realizarTest(int test) {
  switch (test) {
    case 0: // AND
      if (pruebasANDCuadriple(pins[0], pins[1], pins[2]) &&
          pruebasANDCuadriple(pins[3], pins[4], pins[5]) &&
          pruebasANDCuadriple(pins[7], pins[8], pins[6]) &&
          pruebasANDCuadriple(pins[10], pins[11], pins[9])) {
        lcd.setCursor(0, 1);
        lcd.print("AND 4: OK    ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("AND 4: ERROR ");
      }
      break;
    case 1: // OR
      if (pruebasORCuadriple(pins[0], pins[1], pins[2]) &&
          pruebasORCuadriple(pins[3], pins[4], pins[5]) &&
          pruebasORCuadriple(pins[7], pins[8], pins[6]) &&
          pruebasORCuadriple(pins[10], pins[11], pins[9])) {
        lcd.setCursor(0, 1);
        lcd.print("OR 4: OK     ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("OR 4: ERROR  ");
      }
      break;
    case 2: // NAND
      if (pruebasNANDCuadriple(pins[0], pins[1], pins[2]) &&
          pruebasNANDCuadriple(pins[3], pins[4], pins[5]) &&
          pruebasNANDCuadriple(pins[7], pins[8], pins[6]) &&
          pruebasNANDCuadriple(pins[10], pins[11], pins[9])) {
        lcd.setCursor(0, 1);
        lcd.print("NAND 4: OK     ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("NAND 4: ERROR  ");
      }
      break;
    case 3: // XOR
      if (pruebasXORCuadriple(pins[0], pins[1], pins[2]) &&
          pruebasXORCuadriple(pins[3], pins[4], pins[5]) &&
          pruebasXORCuadriple(pins[7], pins[8], pins[6]) &&
          pruebasXORCuadriple(pins[10], pins[11], pins[9])) {
        lcd.setCursor(0, 1);
        lcd.print("XOR 4: OK     ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("XOR 4: ERROR  ");
      }
      break;
    case 4: // NOR
      if (pruebasNORCuadriple(pins[2], pins[1], pins[0]) &&
          pruebasNORCuadriple(pins[5], pins[4], pins[3]) &&
          pruebasNORCuadriple(pins[6], pins[7], pins[8]) &&
          pruebasNORCuadriple(pins[9], pins[10], pins[11])) {
        lcd.setCursor(0, 1);
        lcd.print("NOR 4: OK     ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("NOR 4: ERROR  ");
      }
      break;
    case 5: // NOT sixtuple
      if (pruebasNOTSixtuple(pins[0], pins[1]) &&
          pruebasNOTSixtuple(pins[2], pins[3]) &&
          pruebasNOTSixtuple(pins[4], pins[5]) &&
          pruebasNOTSixtuple(pins[7], pins[6]) &&
          pruebasNOTSixtuple(pins[9], pins[8]) &&
          pruebasNOTSixtuple(pins[11], pins[10])) {
        lcd.setCursor(0, 1);
        lcd.print("NOT 6: OK     ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("NOT 6: ERROR  ");
      }
      break;
    case 6: // NAND triple
      if (pruebasNANDTriple(pins[0], pins[1], pins[11], pins[10]) &&
          pruebasNANDTriple(pins[2], pins[3], pins[4], pins[5]) &&
          pruebasNANDTriple(pins[7], pins[8], pins[9], pins[6])) {
        lcd.setCursor(0, 1);
        lcd.print("NAND 3: OK     ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("NAND 3: ERROR  ");
      }
      break;
		case 7: // AND triple
      if (pruebasANDTriple(pins[0], pins[1], pins[11], pins[10]) &&
          pruebasANDTriple(pins[2], pins[3], pins[4], pins[5]) &&
          pruebasANDTriple(pins[7], pins[8], pins[9], pins[6])) {
        lcd.setCursor(0, 1);
        lcd.print("AND 3: OK     ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("AND 3: ERROR  ");
      }
      break;
		case 8: // NOR triple
      if (pruebasNORTriple(pins[0], pins[1], pins[11], pins[10]) &&
          pruebasNORTriple(pins[2], pins[3], pins[4], pins[5]) &&
          pruebasNORTriple(pins[7], pins[8], pins[9], pins[6])) {
        lcd.setCursor(0, 1);
        lcd.print("NOR 3: OK     ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("NOR 3: ERROR  ");
      }
      break;
    case 9: // NAND Doble
      if (pruebasNANDDoble(pins[0], pins[1], pins[3], pins[4], pins[5]) &&
          pruebasNANDDoble(pins[7], pins[8], pins[10], pins[11], pins[6])) {
        lcd.setCursor(0, 1);
        lcd.print("NAND 2: OK     ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("NAND 2: ERROR  ");
      }
      break;
		case 10: // AND Doble
      if (pruebasANDDoble(pins[0], pins[1], pins[3], pins[4], pins[5]) &&
          pruebasANDDoble(pins[7], pins[8], pins[10], pins[11], pins[6])) {
        lcd.setCursor(0, 1);
        lcd.print("AND 2: OK     ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("AND 2: ERROR  ");
      }
      break;
    case 11: // Quad bistable transparent latch
      if (pruebasBiTrLatch(pins[9], pins[0], pins[12], pins[13]) &&
          pruebasBiTrLatch(pins[9], pins[1], pins[11], pins[10]) &&
          pruebasBiTrLatch(pins[2], pins[3], pins[7], pins[8]) &&
          pruebasBiTrLatch(pins[2], pins[4], pins[6], pins[5])) {
        lcd.setCursor(0, 1);
        lcd.print("LS75 4: OK    ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("LS75 4: ERROR ");
      }
      break;
  }
}

// Ejecuta pruebas con una función lógica dada para 4 entradas, NO SE MODIFICA
bool checkLogicGate4Bits(int pinA, int pinB, int pinC, int pinD, int pinSalida, bool (*logicGateFunction)(int, int, int, int, int, int, int, int, int, const char*)) {
	const char* messages4bits[] = {
		"A = 1, B = 1, C = 1, D = 1",
		"A = 0, B = 1, C = 1, D = 1",
		"A = 1, B = 0, C = 1, D = 1",
		"A = 0, B = 0, C = 1, D = 1",
		"A = 1, B = 1, C = 0, D = 1",
		"A = 0, B = 1, C = 0, D = 1",
		"A = 1, B = 0, C = 0, D = 1",
		"A = 0, B = 0, C = 0, D = 1",
    "A = 1, B = 1, C = 1, D = 0",
    "A = 0, B = 1, C = 1, D = 0",
    "A = 1, B = 0, C = 1, D = 0",
    "A = 0, B = 0, C = 1, D = 0",
    "A = 1, B = 1, C = 0, D = 0",
    "A = 0, B = 1, C = 0, D = 0",
    "A = 1, B = 0, C = 0, D = 0",
    "A = 0, B = 0, C = 0, D = 0"
	};

	int statesA4bits[] = {HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW};
	int statesB4bits[] = {HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW};
	int statesC4bits[] = {HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW};
  int statesD4bits[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

  bool resultado = true;

  for (int i = 0; i < 16; i++) {
    resultado &= logicGateFunction(pinA, pinB, pinC, pinD, pinSalida, statesA4bits[i], statesB4bits[i], statesC4bits[i], statesD4bits[i], messages4bits[i]);
    delay(100); // Espera 0.1 segundos
  }
  return resultado;
}

// Ejecuta pruebas con una función lógica dada para 3 entradas, NO SE MODIFICA
bool checkLogicGate3Bits(int pinA, int pinB, int pinC, int pinSalida, bool (*logicGateFunction)(int, int, int, int, int, int, int, const char*)) {
	const char* messages3bits[] = {
		"A = 1, B = 1, C = 1",
		"A = 0, B = 1, C = 1",
		"A = 1, B = 0, C = 1",
		"A = 0, B = 0, C = 1",
		"A = 1, B = 1, C = 0",
		"A = 0, B = 1, C = 0",
		"A = 1, B = 0, C = 0",
		"A = 0, B = 0, C = 0"
	};

	int statesA3bits[] = {HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW};
	int statesB3bits[] = {HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW};
	int statesC3bits[] = {HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW};

  bool resultado = true;

  for (int i = 0; i < 8; i++) {
    resultado &= logicGateFunction(pinA, pinB, pinC, pinSalida, statesA3bits[i], statesB3bits[i], statesC3bits[i], messages3bits[i]);
    delay(100); // Espera 0.1 segundos
  }

  return resultado;
}

// Ejecuta pruebas con una función lógica dada para 2 entradas, NO SE MODIFICA
bool checkLogicGate2Bits(int pinA, int pinB, int pinSalida, bool (*logicGateFunction)(int, int, int, int, int, const char*)) {
	const char* messages2bits[] = {
  	"A = 1, B = 1",
  	"A = 0, B = 1",
  	"A = 1, B = 0",
  	"A = 0, B = 0"
	};

	int statesA2bits[] = {HIGH, LOW, HIGH, LOW};
	int statesB2bits[] = {HIGH, HIGH, LOW, LOW};

  bool resultado = true;

  for (int i = 0; i < 4; i++) {
    resultado &= logicGateFunction(pinA, pinB, pinSalida, statesA2bits[i], statesB2bits[i], messages2bits[i]);
    delay(100); // Espera 0.1 segundos
  }

  return resultado;
}

// Ejecuta pruebas con una función lógica dada para 1 entrada, NO SE MODIFICA
bool checkLogicGate1Bit(int pinA, int pinSalida, bool (*logicGateFunction)(int, int, int, const char*)) {
	const char* messages1bit[] = {
  	"A = 1",
  	"A = 0"
	};

	int statesA1bit[] = {HIGH, LOW};

  bool resultado = true;

  for (int i = 0; i < 2; i++) {
    resultado &= logicGateFunction(pinA, pinSalida, statesA1bit[i], messages1bit[i]);
    delay(100); // Espera 0.1 segundos
  }

  return resultado;
}

// Realiza la prueba lógica y compara el resultado para 4 entradas, NO SE MODIFICA
bool testLogicGate4Bits(int pinA, int pinB, int pinC, int pinD, int pinSalida, int estadoA, int estadoB, int estadoC, int estadoD, bool (*logicOperation)(int, int, int, int), const char* message) {
  digitalWrite(pinA, estadoA);
  digitalWrite(pinB, estadoB);
	digitalWrite(pinC, estadoC);
  digitalWrite(pinD, estadoD);
  delay(100); // Pequeña pausa para estabilizar

  int val = digitalRead(pinSalida);
  bool resultado = logicOperation(estadoA, estadoB, estadoC, estadoD) == val;

  printResult(message, resultado);

  return resultado;
}

// Realiza la prueba lógica y compara el resultado para 3 entradas, NO SE MODIFICA
bool testLogicGate3Bits(int pinA, int pinB, int pinC, int pinSalida, int estadoA, int estadoB, int estadoC, bool (*logicOperation)(int, int, int), const char* message) {
  digitalWrite(pinA, estadoA);
  digitalWrite(pinB, estadoB);
	digitalWrite(pinC, estadoC);
  delay(100); // Pequeña pausa para estabilizar

  int val = digitalRead(pinSalida);
  bool resultado = logicOperation(estadoA, estadoB, estadoC) == val;

  printResult(message, resultado);

  return resultado;
}

// Realiza la prueba lógica y compara el resultado para 2 entradas, NO SE MODIFICA
bool testLogicGate2Bits(int pinA, int pinB, int pinSalida, int estadoA, int estadoB, bool (*logicOperation)(int, int), const char* message) {
  digitalWrite(pinA, estadoA);
  digitalWrite(pinB, estadoB);
  delay(100); // Pequeña pausa para estabilizar

  int val = digitalRead(pinSalida);
  bool resultado = logicOperation(estadoA, estadoB) == val;

  printResult(message, resultado);

  return resultado;
}

// Realiza la prueba lógica y compara el resultado para 1 entrada, NO SE MODIFICA
bool testLogicGate1Bit(int pinA, int pinSalida, int estadoA, bool (*logicOperation)(int), const char* message) {
  digitalWrite(pinA, estadoA);
  delay(100); // Pequeña pausa para estabilizar

  int val = digitalRead(pinSalida);
  bool resultado = logicOperation(estadoA) == val;

  printResult(message, resultado);

  return resultado;
}

// Imprime el resultado en el monitor serial, NO SE MODIFICA
void printResult(const char* message, bool resultado) {
  Serial.print(message);
  if (resultado) {
    Serial.println("\tOK");
  } else {
    Serial.println("\tERROR");
  }
}

// APARTIR DE ACA DE AGREGAN LAS DEMAS COMPUERTAS

// Pruebas específicas para la puerta AND
bool pruebasANDCuadriple(int pinA, int pinB, int pinSalida) {
  return checkLogicGate2Bits(pinA, pinB, pinSalida, testAND2Bits);
}

// Pruebas específicas para la puerta OR
bool pruebasORCuadriple(int pinA, int pinB, int pinSalida) {
  return checkLogicGate2Bits(pinA, pinB, pinSalida, testOR2Bits);
}

// Pruebas específicas para la puerta NAND
bool pruebasNANDCuadriple(int pinA, int pinB, int pinSalida) {
  return checkLogicGate2Bits(pinA, pinB, pinSalida, testNAND2Bits);
}

// Pruebas específicas para la puerta XOR
bool pruebasXORCuadriple(int pinA, int pinB, int pinSalida) {
  return checkLogicGate2Bits(pinA, pinB, pinSalida, testXOR2Bits);
}

// Pruebas específicas para la puerta NOR
bool pruebasNORCuadriple(int pinA, int pinB, int pinSalida) {
  return checkLogicGate2Bits(pinA, pinB, pinSalida, testNOR2Bits);
}

// Pruebas específicas para la puerta NOT
bool pruebasNOTSixtuple(int pinA, int pinSalida) {
  return checkLogicGate1Bit(pinA, pinSalida, testNOT1Bit);
}

// Pruebas específicas para la puerta NAND
bool pruebasNANDTriple(int pinA, int pinB, int pinC, int pinSalida) {
  return checkLogicGate3Bits(pinA, pinB, pinC, pinSalida, testNAND3Bits);
}

// Pruebas específicas para la puerta NAND
bool pruebasANDTriple(int pinA, int pinB, int pinC, int pinSalida) {
  return checkLogicGate3Bits(pinA, pinB, pinC, pinSalida, testAND3Bits);
}

// Pruebas específicas para la puerta NAND
bool pruebasNORTriple(int pinA, int pinB, int pinC, int pinSalida) {
  return checkLogicGate3Bits(pinA, pinB, pinC, pinSalida, testNOR3Bits);
}

// Pruebas específicas para la puerta NAND
bool pruebasNANDDoble(int pinA, int pinB, int pinC, int pinD, int pinSalida) {
  return checkLogicGate4Bits(pinA, pinB, pinC, pinD, pinSalida, testNAND4Bits);
}

// Pruebas específicas para la puerta NAND
bool pruebasANDDoble(int pinA, int pinB, int pinC, int pinD, int pinSalida) {
  return checkLogicGate4Bits(pinA, pinB, pinC, pinD, pinSalida, testAND4Bits);
}

// Pruebas específicas para la puerta Biestable Transparent Latch
bool pruebasBiTrLatch(int pinEn, int pinD, int pinQ, int pinQn) {
  int statesD[] = {HIGH, LOW, HIGH, LOW, LOW, HIGH, HIGH, HIGH, LOW, LOW, LOW};
  int statesEn[] = {HIGH, HIGH, LOW, LOW, HIGH, LOW, HIGH, LOW, LOW, HIGH, LOW};

  const int numTests = 11;

  bool lastQ = HIGH;
  bool lastQn = LOW;
  bool lastD = HIGH;
  bool lastEn = HIGH;

  bool resultado = true;

  for (int i = 0; i < numTests; i++) {
    digitalWrite(pinEn, statesEn[i]);
    digitalWrite(pinD, statesD[i]);

    bool actualQ = digitalRead(pinQ);
    bool actualQn = digitalRead(pinQn);

    bool expectedQ, expectedQn;
    biTriLatchLogic(statesD[i], statesEn[i], expectedQ, expectedQn, lastQ, lastQn, lastD, lastEn);

    resultado &= ((actualQ == expectedQ) && (actualQn == expectedQn));

    lastD = statesD[i];
    lastEn = statesEn[i];
    
    lastQ = expectedQ;
    lastQn = expectedQn;

    delay(100); // Pequeña pausa para estabilizar
  }
  return resultado;
}

// Función para el latch
void biTriLatchLogic(bool D, bool actualEn, bool &Q, bool &Qn, bool lastQ, bool lastQn, bool lastD, bool lastEn) {
   if (actualEn) {
    if (D != lastD) {
      Q = !lastQ;
      Qn = !lastQn;
    } 
    else if ((D == HIGH) && (lastD == HIGH) && (lastQ == LOW) && (lastQn == HIGH) && (actualEn != lastEn)) {
      Q = !lastQ;
      Qn = !lastQn;
    }
    else if ((D == LOW) && (lastD == LOW) && (lastQ == HIGH) && (lastQn == LOW) && (actualEn != lastEn)) {
      Q = !lastQ;
      Qn = !lastQn;
    } else {
      Q = lastQ;
      Qn = lastQn;
    }
  } else {
    Q = lastQ;
    Qn = lastQn;
  }
}

//-----------------------------------

// Operación lógica AND 2 entradas
bool andLogic2Bits(int estadoA, int estadoB) {
  return estadoA && estadoB;
}

// Operación lógica OR 2 entradas
bool orLogic2Bits(int estadoA, int estadoB) {
  return estadoA || estadoB;
}

// Operación lógica NAND 2 entradas
bool nandLogic2Bits(int estadoA, int estadoB) {
  return ! (estadoA && estadoB);
}

// Operación lógica XOR 2 entradas
bool xorLogic2Bits(int estadoA, int estadoB) {
  return estadoA ^ estadoB;
}

// Operación lógica NOR 2 entradas
bool norLogic2Bits(int estadoA, int estadoB) {
  return ! (estadoA || estadoB);
}

// Operación lógica NOT
bool notLogic1Bit(int estadoA) {
  return !estadoA;
}

// Operación lógica AND 3 entradas
bool andLogic3Bits(int estadoA, int estadoB, int estadoC) {
  return estadoA && estadoB && estadoC;
}

// Operación lógica OR 3 entradas, No existe al parecer
bool orLogic3Bits(int estadoA, int estadoB, int estadoC) {
  return estadoA || estadoB || estadoC;
}

// Operación lógica NAND 3 entradas
bool nandLogic3Bits(int estadoA, int estadoB, int estadoC) {
  return ! (estadoA && estadoB && estadoC);
}

// Operación lógica XOR 3 entradas, No existe al parecer
bool xorLogic3Bits(int estadoA, int estadoB, int estadoC) {
  return estadoA ^ estadoB ^ estadoC;
}

// Operación lógica NOR 3 entradas
bool norLogic3Bits(int estadoA, int estadoB, int estadoC) {
  return ! (estadoA || estadoB || estadoC);
}

// Operación lógica AND 4 entradas
bool andLogic4Bits(int estadoA, int estadoB, int estadoC, int estadoD) {
  return estadoA && estadoB && estadoC && estadoD;
}

// Operación lógica OR 4 entradas, No existe al parecer
bool orLogic4Bits(int estadoA, int estadoB, int estadoC, int estadoD) {
  return estadoA || estadoB || estadoC || estadoD;
}

// Operación lógica NAND 4 entradas
bool nandLogic4Bits(int estadoA, int estadoB, int estadoC, int estadoD) {
  return ! (estadoA && estadoB && estadoC && estadoD);
}

// Operación lógica XOR 4 entradas, No existe al parecer
bool xorLogic4Bits(int estadoA, int estadoB, int estadoC, int estadoD) {
  return estadoA ^ estadoB ^ estadoC ^ estadoD;
}

// Operación lógica NOR 4 entradas, No existe al parecer
bool norLogic4Bits(int estadoA, int estadoB, int estadoC, int estadoD) {
  return ! (estadoA || estadoB || estadoC || estadoD);
}

//-----------------------------------

// Función específica para la puerta lógica AND
bool testAND2Bits(int pinA, int pinB, int pinSalida, int estadoA, int estadoB, const char* message) {
  return testLogicGate2Bits(pinA, pinB, pinSalida, estadoA, estadoB, andLogic2Bits, message);
}

// Función específica para la puerta lógica OR
bool testOR2Bits(int pinA, int pinB, int pinSalida, int estadoA, int estadoB, const char* message) {
  return testLogicGate2Bits(pinA, pinB, pinSalida, estadoA, estadoB, orLogic2Bits, message);
}

// Función específica para la puerta lógica NAND
bool testNAND2Bits(int pinA, int pinB, int pinSalida, int estadoA, int estadoB, const char* message) {
  return testLogicGate2Bits(pinA, pinB, pinSalida, estadoA, estadoB, nandLogic2Bits, message);
}

// Función específica para la puerta lógica XOR
bool testXOR2Bits(int pinA, int pinB, int pinSalida, int estadoA, int estadoB, const char* message) {
  return testLogicGate2Bits(pinA, pinB, pinSalida, estadoA, estadoB, xorLogic2Bits, message);
}

// Función específica para la puerta lógica NOR
bool testNOR2Bits(int pinA, int pinB, int pinSalida, int estadoA, int estadoB, const char* message) {
  return testLogicGate2Bits(pinA, pinB, pinSalida, estadoA, estadoB, norLogic2Bits, message);
}

// Función específica para la puerta lógica NOT
bool testNOT1Bit(int pinA, int pinSalida, int estadoA, const char* message) {
  return testLogicGate1Bit(pinA, pinSalida, estadoA, notLogic1Bit, message);
}

// Función específica para la puerta lógica NAND 
bool testNAND3Bits(int pinA, int pinB, int pinC, int pinSalida, int estadoA, int estadoB, int estadoC, const char* message) {
  return testLogicGate3Bits(pinA, pinB, pinC, pinSalida, estadoA, estadoB, estadoC, nandLogic3Bits, message);
}

// Función específica para la puerta lógica NAND 
bool testAND3Bits(int pinA, int pinB, int pinC, int pinSalida, int estadoA, int estadoB, int estadoC, const char* message) {
  return testLogicGate3Bits(pinA, pinB, pinC, pinSalida, estadoA, estadoB, estadoC, andLogic3Bits, message);
}

// Función específica para la puerta lógica NAND 
bool testNOR3Bits(int pinA, int pinB, int pinC, int pinSalida, int estadoA, int estadoB, int estadoC, const char* message) {
  return testLogicGate3Bits(pinA, pinB, pinC, pinSalida, estadoA, estadoB, estadoC, norLogic3Bits, message);
}

// Función específica para la puerta lógica NAND 
bool testNAND4Bits(int pinA, int pinB, int pinC, int pinD, int pinSalida, int estadoA, int estadoB, int estadoC, int estadoD, const char* message) {
  return testLogicGate4Bits(pinA, pinB, pinC, pinD, pinSalida, estadoA, estadoB, estadoC, estadoD, nandLogic4Bits, message);
}

// Función específica para la puerta lógica NAND 
bool testAND4Bits(int pinA, int pinB, int pinC, int pinD, int pinSalida, int estadoA, int estadoB, int estadoC, int estadoD, const char* message) {
  return testLogicGate4Bits(pinA, pinB, pinC, pinD, pinSalida, estadoA, estadoB, estadoC, estadoD, andLogic4Bits, message);
}