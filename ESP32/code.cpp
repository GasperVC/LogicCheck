const int pins[] = {
  25, 26, 27, 22, 21, 19, 18, 5, 17, 16, 4, 2, 15, 13
};
const int numTests = 12; // Número de pruebas (AND, OR, etc.)
int currentTest = 0;    // Índice del test actual (0 para AND, 1 para OR, etc.)
const char* testNames[numTests] = {"74HC08", "74HC32", "74HC00", "74HC86", "74HC02", "74HC04", "74HC10", "74HC11", "74HC27", "74HC20", "74HC21", "74LS75"};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 14; i++) {
    pinMode(pins[i], INPUT);
  }
  // configurarPines(currentTest); // Reconfigurar los pines según el test
}

//-----------------------------------
// APARTIR DE ACA DE AGREGAN LAS DEMAS COMPUERTAS
//-----------------------------------

bool andLogic2Bits(int estadoA, int estadoB) {  // Operación lógica AND 2 entradas
  return estadoA && estadoB;
}
bool orLogic2Bits(int estadoA, int estadoB) { // Operación lógica OR 2 entradas
  return estadoA || estadoB;
}
bool nandLogic2Bits(int estadoA, int estadoB) { // Operación lógica NAND 2 entradas
  return ! (estadoA && estadoB);
}
bool xorLogic2Bits(int estadoA, int estadoB) {  // Operación lógica XOR 2 entradas
  return estadoA ^ estadoB;
}
bool norLogic2Bits(int estadoA, int estadoB) {  // Operación lógica NOR 2 entradas
  return ! (estadoA || estadoB);
}
bool notLogic1Bit(int estadoA) {  // Operación lógica NOT
  return !estadoA;
}
bool andLogic3Bits(int estadoA, int estadoB, int estadoC) { // Operación lógica AND 3 entradas
  return estadoA && estadoB && estadoC;
}
bool nandLogic3Bits(int estadoA, int estadoB, int estadoC) {  // Operación lógica NAND 3 entradas
  return ! (estadoA && estadoB && estadoC);
}
bool norLogic3Bits(int estadoA, int estadoB, int estadoC) { // Operación lógica NOR 3 entradas
  return ! (estadoA || estadoB || estadoC);
}
bool andLogic4Bits(int estadoA, int estadoB, int estadoC, int estadoD) {  // Operación lógica AND 4 entradas
  return estadoA && estadoB && estadoC && estadoD;
}
bool nandLogic4Bits(int estadoA, int estadoB, int estadoC, int estadoD) { // Operación lógica NAND 4 entradas
  return ! (estadoA && estadoB && estadoC && estadoD);
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

// Función para configurar los pines según el test
void configurarPines(int test) {
  switch (test) {
    case 0: // AND cuadruple
    case 1: // OR cuadruple
    case 2: // NAND cuadruple
    case 3: // XOR cuadruple
      // Configuración de pines de la puerta 1
      pinMode(pins[0], OUTPUT); pinMode(pins[1], OUTPUT); pinMode(pins[2], INPUT);
      pinMode(pins[3], OUTPUT); pinMode(pins[4], OUTPUT); pinMode(pins[5], INPUT);
      pinMode(pins[7], OUTPUT); pinMode(pins[8], OUTPUT); pinMode(pins[6], INPUT);
      pinMode(pins[10], OUTPUT); pinMode(pins[11], OUTPUT); pinMode(pins[9], INPUT);
      break;
    case 4: // NOR cuadruple
      // Configuración de pines de la puerta 1
      pinMode(pins[2], OUTPUT); pinMode(pins[1], OUTPUT); pinMode(pins[0], INPUT);
      pinMode(pins[5], OUTPUT); pinMode(pins[4], OUTPUT); pinMode(pins[3], INPUT);
      pinMode(pins[6], OUTPUT); pinMode(pins[7], OUTPUT); pinMode(pins[8], INPUT);
      pinMode(pins[9], OUTPUT); pinMode(pins[10], OUTPUT); pinMode(pins[11], INPUT);
      break;
    case 5: // NOT sixtuple
      // Configuración de pines de la puerta 1
      pinMode(pins[0], OUTPUT); pinMode(pins[1], INPUT);
      pinMode(pins[2], OUTPUT); pinMode(pins[3], INPUT);
      pinMode(pins[4], OUTPUT); pinMode(pins[5], INPUT);

      // Configuración de pines de la puerta 3
      pinMode(pins[6], INPUT); pinMode(pins[7], OUTPUT);
      pinMode(pins[8], INPUT); pinMode(pins[9], OUTPUT);
      pinMode(pins[10], INPUT); pinMode(pins[11], OUTPUT);
      break;
    case 6: // NAND triple
    case 7: // AND triple
    case 8: // NOR triple
      // A, B, C, Salida
      // Configuración de pines de la puerta 1
      pinMode(pins[0], OUTPUT); pinMode(pins[1], OUTPUT); pinMode(pins[11], OUTPUT); pinMode(pins[10], INPUT);
      pinMode(pins[2], OUTPUT); pinMode(pins[3], OUTPUT); pinMode(pins[4], OUTPUT); pinMode(pins[5], INPUT);
      pinMode(pins[7], OUTPUT); pinMode(pins[8], OUTPUT); pinMode(pins[9], OUTPUT); pinMode(pins[6], INPUT);
      break;
    case 9: // NAND doble
    case 10: // AND doble
      // A, B, C, D, Salida
      // Configuración de pines de la puerta 1
      pinMode(pins[0], OUTPUT); pinMode(pins[1], OUTPUT); pinMode(pins[3], OUTPUT); pinMode(pins[4], OUTPUT); pinMode(pins[5], INPUT);
      pinMode(pins[7], OUTPUT); pinMode(pins[8], OUTPUT); pinMode(pins[10], OUTPUT); pinMode(pins[11], OUTPUT); pinMode(pins[6], INPUT);
      break;
    case 11: // Quad bistable transparent latch
      // 1D, 1Qn, 1Q
      // Configuración de pines de la puerta 1
      pinMode(pins[0], OUTPUT); pinMode(pins[13], INPUT); pinMode(pins[12], INPUT);
      pinMode(pins[1], OUTPUT); pinMode(pins[10], INPUT); pinMode(pins[11], INPUT);
      pinMode(pins[3], OUTPUT); pinMode(pins[8], INPUT); pinMode(pins[7], INPUT);
      pinMode(pins[4], OUTPUT); pinMode(pins[5], INPUT); pinMode(pins[6], INPUT);

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

void printResult(const char* message, bool resultado) {
  Serial.print(message);
  if (resultado) {
    Serial.println("\tOK");
  } else {
    Serial.println("\tERROR");
  }
}

// Ejecuta pruebas con una función lógica dada para 4 entradas, NO SE MODIFICA
bool checkLogicGate4Bits(int pinA, int pinB, int pinC, int pinD, int pinSalida, bool (*logicOperation)(int, int, int, int)) {
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
    digitalWrite(pinA, statesA4bits[i]);
    digitalWrite(pinB, statesB4bits[i]);
    digitalWrite(pinC, statesC4bits[i]);
    digitalWrite(pinD, statesD4bits[i]);
    delay(100); // Pequeña pausa para estabilizar

    int val = digitalRead(pinSalida);
    resultado &= logicOperation(statesA4bits[i], statesB4bits[i], statesC4bits[i], statesD4bits[i]);
    delay(20); // Espera 0.02 segundos
    printResult(messages4bits[i], resultado);
  }
  return resultado;
}

// Ejecuta pruebas con una función lógica dada para 3 entradas, NO SE MODIFICA
bool checkLogicGate3Bits(int pinA, int pinB, int pinC, int pinSalida, bool (*logicOperation)(int, int, int)) {
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
    digitalWrite(pinA, statesA3bits[i]);
    digitalWrite(pinB, statesB3bits[i]);
    digitalWrite(pinC, statesC3bits[i]);
    delay(100); // Pequeña pausa para estabilizar

    int val = digitalRead(pinSalida);
    resultado &= logicOperation(statesA3bits[i], statesB3bits[i], statesC3bits[i]) == val;
    delay(20); // Espera 0.02 segundos
    printResult(messages3bits[i], resultado);
  }

  return resultado;
}

// Ejecuta pruebas con una función lógica dada para 2 entradas, NO SE MODIFICA
bool checkLogicGate2Bits(int pinA, int pinB, int pinSalida, bool (*logicOperation)(int, int)) {
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
    digitalWrite(pinA, statesA2bits[i]);
    digitalWrite(pinB, statesB2bits[i]);
    delay(100); // Pequeña pausa para estabilizar

    int val = digitalRead(pinSalida);
    resultado &= logicOperation(statesA2bits[i], statesB2bits[i]) == val;
    delay(20); // Espera 0.02 segundos
    printResult(messages2bits[i], resultado);
  }

  return resultado;
}

// Ejecuta pruebas con una función lógica dada para 1 entrada, NO SE MODIFICA
bool checkLogicGate1Bit(int pinA, int pinSalida, bool (*logicOperation)(int)) {
	const char* messages1bit[] = {
  	"A = 1",
  	"A = 0"
	};

	int statesA1bit[] = {HIGH, LOW};

  bool resultado = true;

  for (int i = 0; i < 2; i++) {
    digitalWrite(pinA, statesA1bit[i]);
    delay(100); // Pequeña pausa para estabilizar

    int val = digitalRead(pinSalida);
    resultado &= logicOperation(statesA1bit[i]) == val;
    delay(20); // Espera 0.02 segundos
    printResult(messages1bit[i], resultado);
  }

  return resultado;
}

bool pruebasDoble(int pinA1, int pinB1, int pinC1, int pinD1, int pinSalida1,
                  int pinA2, int pinB2, int pinC2, int pinD2, int pinSalida2,
                  bool (*testFunc)(int, int, int, int)) {
    return checkLogicGate4Bits(pinA1, pinB1, pinC1, pinD1, pinSalida1, testFunc) &&
           checkLogicGate4Bits(pinA2, pinB2, pinC2, pinD2, pinSalida2, testFunc);
}
bool pruebasTriple(int pinA1, int pinB1, int pinC1, int pinSalida1,
                   int pinA2, int pinB2, int pinC2, int pinSalida2,
                   int pinA3, int pinB3, int pinC3, int pinSalida3,
                   bool (*testFunc)(int, int, int)) {
    return checkLogicGate3Bits(pinA1, pinB1, pinC1, pinSalida1, testFunc) &&
           checkLogicGate3Bits(pinA2, pinB2, pinC2, pinSalida2, testFunc) &&
           checkLogicGate3Bits(pinA3, pinB3, pinC3, pinSalida3, testFunc);
}
bool pruebasCuadriple(int pinA1, int pinB1, int pinSalida1,
                      int pinA2, int pinB2, int pinSalida2,
                      int pinA3, int pinB3, int pinSalida3,
                      int pinA4, int pinB4, int pinSalida4,
                      bool (*testFunc)(int, int)) {
    return checkLogicGate2Bits(pinA1, pinB1, pinSalida1, testFunc) &&
           checkLogicGate2Bits(pinA2, pinB2, pinSalida2, testFunc) &&
           checkLogicGate2Bits(pinA3, pinB3, pinSalida3, testFunc) &&
           checkLogicGate2Bits(pinA4, pinB4, pinSalida4, testFunc);
}
bool pruebasSixtuple(int pinA1, int pinSalida1,
                     int pinA2, int pinSalida2,
                     int pinA3, int pinSalida3,
                     int pinA4, int pinSalida4,
                     int pinA5, int pinSalida5,
                     int pinA6, int pinSalida6,
                     bool (*testFunc)(int)) {
    return checkLogicGate1Bit(pinA1, pinSalida1, testFunc) &&
           checkLogicGate1Bit(pinA2, pinSalida2, testFunc) &&
           checkLogicGate1Bit(pinA3, pinSalida3, testFunc) &&
           checkLogicGate1Bit(pinA4, pinSalida4, testFunc) &&
           checkLogicGate1Bit(pinA5, pinSalida5, testFunc) &&
           checkLogicGate1Bit(pinA6, pinSalida6, testFunc);
}

// Función para realizar el test actual
void realizarTest(int test) {
  switch (test) {
    case 0: // AND
      if (pruebasCuadriple(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[6], pins[10], pins[11], pins[9], andLogic2Bits)) {
        Serial.print("AND 4: OK    ");
      } else {
        Serial.print("AND 4: ERROR ");
      }
      break;
    case 1: // OR
      if (pruebasCuadriple(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[6], pins[10], pins[11], pins[9], orLogic2Bits)) {
        Serial.print("OR 4: OK     ");
      } else {
        Serial.print("OR 4: ERROR  ");
      }
      break;
    case 2: // NAND
      if (pruebasCuadriple(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[6], pins[10], pins[11], pins[9], nandLogic2Bits)) {
        Serial.print("NAND 4: OK     ");
      } else {
        Serial.print("NAND 4: ERROR  ");
      }
      break;
    case 3: // XOR
      if (pruebasCuadriple(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[6], pins[10], pins[11], pins[9], xorLogic2Bits)) {
        Serial.print("XOR 4: OK     ");
      } else {
        Serial.print("XOR 4: ERROR  ");
      }
      break;
    case 4: // NOR
      if (pruebasCuadriple(pins[2], pins[1], pins[0], pins[5], pins[4], pins[3], pins[6], pins[7], pins[8], pins[9], pins[10], pins[11], norLogic2Bits)) {
        Serial.print("NOR 4: OK     ");
      } else {
        Serial.print("NOR 4: ERROR  ");
      }
      break;
    case 5: // NOT sixtuple
      if (pruebasSixtuple(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5], pins[7], pins[6], pins[9], pins[8], pins[11], pins[10], notLogic1Bit)) {
        Serial.print("NOT 6: OK     ");
      } else {
        Serial.print("NOT 6: ERROR  ");
      }
      break;
    case 6: // NAND triple
      if (pruebasTriple(pins[0], pins[1], pins[11], pins[10], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[9], pins[6], nandLogic3Bits)) {
        Serial.print("NAND 3: OK     ");
      } else {
        Serial.print("NAND 3: ERROR  ");
      }
      break;
		case 7: // AND triple
      if (pruebasTriple(pins[0], pins[1], pins[11], pins[10], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[9], pins[6], andLogic3Bits)) {
        Serial.print("AND 3: OK     ");
      } else {
        Serial.print("AND 3: ERROR  ");
      }
      break;
		case 8: // NOR triple
      if (pruebasTriple(pins[0], pins[1], pins[11], pins[10], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[9], pins[6], norLogic3Bits)){
        Serial.print("NOR 3: OK     ");
      } else {
        Serial.print("NOR 3: ERROR  ");
      }
      break;
    case 9: // NAND Doble
      if (pruebasDoble(pins[0], pins[1], pins[3], pins[4], pins[5], pins[7], pins[8], pins[10], pins[11], pins[6], nandLogic4Bits)){
        Serial.print("NAND 2: OK     ");
      } else {
        Serial.print("NAND 2: ERROR  ");
      }
      break;
		case 10: // AND Doble
      if (pruebasDoble(pins[0], pins[1], pins[3], pins[4], pins[5], pins[7], pins[8], pins[10], pins[11], pins[6], andLogic4Bits)){
        Serial.print("AND 2: OK     ");
      } else {
        Serial.print("AND 2: ERROR  ");
      }
      break;
    case 11: // Quad bistable transparent latch
      if (pruebasBiTrLatch(pins[9], pins[0], pins[12], pins[13]) &&
          pruebasBiTrLatch(pins[9], pins[1], pins[11], pins[10]) &&
          pruebasBiTrLatch(pins[2], pins[3], pins[7], pins[8]) &&
          pruebasBiTrLatch(pins[2], pins[4], pins[6], pins[5])) {
        Serial.print("LS75 4: OK    ");
      } else {
        Serial.print("LS75 4: ERROR ");
      }
      break;
  }
}

void loop() {
  for (int i = 0; i < 16; i++) {
    currentTest=i;
    configurarPines(currentTest); // Reconfigurar los pines según el test
    realizarTest(currentTest);
    delay(1500);
  }
  Serial.println("FINISH");
}