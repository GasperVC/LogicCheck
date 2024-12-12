#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
                  // 0,  1,  2,  3,  4,  5,  6,  7, 8,  9,  10,11,12, 13  // posicion de pins
const int pins[] = { 25, 26, 27, 22, 21, 19, 18, 5, 17, 16, 4, 2, 15, 13 };
                  // 1,  2,  3,  4,  5,  6,  8,  9, 10, 11, 12,13         // posicion de 14 pines
                  // 2,  3,  4,  6,  7,  8,  9,  10,11, 13, 14,15,16, 1   // posicion de 16 pines 74LS75
                  // 2,  3,  4,  5,  6,  7,  9,  10,11, 12, 13,14,15, 1   // posicion de 16 pines 74LS48
const int numTests = 15; // Número de pruebas (AND, OR, etc.)
// int currentTest = 0;    // Índice del test actual (0 para AND, 1 para OR, etc.)
const char* testNames[numTests] = {"74HC08", "74HC32", "74HC00", "74HC86", "74HC02", "74HC04", "74HC10", "74HC11", "74HC27", "74HC20", "74HC21", "74LS75", "74HC4075", "74HC4072", "74LS48"};

// Configura el nombre y la contraseña de tu red WiFi
const char* ssid = "clifford";
const char* password = "cliff4yyyy";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

//-----------------------------------
// APARTIR DE ACA DE AGREGAN LAS DEMAS COMPUERTAS
//-----------------------------------

bool andLogic2Bits(int estadoA, int estadoB) {  // Operación lógica AND 2 entradas
  return estadoA && estadoB; }
bool orLogic2Bits(int estadoA, int estadoB) { // Operación lógica OR 2 entradas
  return estadoA || estadoB; }
bool nandLogic2Bits(int estadoA, int estadoB) { // Operación lógica NAND 2 entradas
  return ! (estadoA && estadoB); }
bool xorLogic2Bits(int estadoA, int estadoB) {  // Operación lógica XOR 2 entradas
  return estadoA ^ estadoB; }
bool norLogic2Bits(int estadoA, int estadoB) {  // Operación lógica NOR 2 entradas
  return ! (estadoA || estadoB); }

bool notLogic1Bit(int estadoA) {  // Operación lógica NOT
  return !estadoA; }

bool andLogic3Bits(int estadoA, int estadoB, int estadoC) { // Operación lógica AND 3 entradas
  return estadoA && estadoB && estadoC; }
bool orLogic3Bits(int estadoA, int estadoB, int estadoC) {  // Operación lógica OR 3 entradas
  return estadoA || estadoB || estadoC; }
bool nandLogic3Bits(int estadoA, int estadoB, int estadoC) {  // Operación lógica NAND 3 entradas
  return ! (estadoA && estadoB && estadoC); }
bool norLogic3Bits(int estadoA, int estadoB, int estadoC) { // Operación lógica NOR 3 entradas
  return ! (estadoA || estadoB || estadoC); }

bool andLogic4Bits(int estadoA, int estadoB, int estadoC, int estadoD) {  // Operación lógica AND 4 entradas
  return estadoA && estadoB && estadoC && estadoD; }
bool orLogic4Bits(int estadoA, int estadoB, int estadoC, int estadoD) { // Operación lógica OR 4 entradas
  return estadoA || estadoB || estadoC || estadoD; }
bool nandLogic4Bits(int estadoA, int estadoB, int estadoC, int estadoD) { // Operación lógica NAND 4 entradas
  return ! (estadoA && estadoB && estadoC && estadoD); }

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

bool pruebasDeco7Seg(int pinLT, int pinRBI, int pinA0, int pinA1, int pinA2, int pinA3, int pinBIRBO, int pin_a, int pin_b, int pin_c, int pin_d, int pin_e, int pin_f, int pin_g) {
  const char* inputs4bits[] = {
    "D = 0, C = 0, B = 0, A = 0",
    "D = 0, C = 0, B = 0, A = 1",
    "D = 0, C = 0, B = 1, A = 0",
    "D = 0, C = 0, B = 1, A = 1",
    "D = 0, C = 1, B = 0, A = 0",
    "D = 0, C = 1, B = 0, A = 1",
    "D = 0, C = 1, B = 1, A = 0",
    "D = 0, C = 1, B = 1, A = 1",
    "D = 1, C = 0, B = 0, A = 0",
    "D = 1, C = 0, B = 0, A = 1",
    "D = 1, C = 0, B = 1, A = 0",
    "D = 1, C = 0, B = 1, A = 1",
    "D = 1, C = 1, B = 0, A = 0",
    "D = 1, C = 1, B = 0, A = 1",
    "D = 1, C = 1, B = 1, A = 0",
    "D = 1, C = 1, B = 1, A = 1"
  };

  int statesRBI[] = {HIGH, LOW};
  int statesA0[] = {LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH};
  int statesA1[] = {LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH};
  int statesA2[] = {LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH};
  int statesA3[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

  const char* outputs4bits[] = {
    "0: BI/RBO=H, a=H, b=H, c=H, d=H, e=H, f=H, g=L",
    "1: BI/RBO=H, a=L, b=H, c=H, d=L, e=L, f=L, g=L",
    "2: BI/RBO=H, a=H, b=H, c=L, d=H, e=H, f=L, g=H",
    "3: BI/RBO=H, a=H, b=H, c=H, d=H, e=L, f=L, g=H",
    "4: BI/RBO=H, a=L, b=H, c=H, d=L, e=L, f=H, g=H",
    "5: BI/RBO=H, a=H, b=L, c=H, d=H, e=L, f=H, g=H",
    "6: BI/RBO=H, a=L, b=L, c=H, d=H, e=H, f=H, g=H",
    "7: BI/RBO=H, a=H, b=H, c=H, d=L, e=L, f=L, g=L",
    "8: BI/RBO=H, a=H, b=H, c=H, d=H, e=H, f=H, g=H",
    "9: BI/RBO=H, a=H, b=H, c=H, d=L, e=L, f=H, g=H",
    "10: BI/RBO=H, a=L, b=L, c=L, d=H, e=H, f=L, g=H",
    "11: BI/RBO=H, a=L, b=L, c=H, d=H, e=L, f=L, g=H",
    "12: BI/RBO=H, a=L, b=H, c=L, d=L, e=L, f=H, g=H",
    "13: BI/RBO=H, a=H, b=L, c=L, d=H, e=L, f=H, g=H",
    "14: BI/RBO=H, a=L, b=L, c=L, d=H, e=H, f=H, g=H",
    "15: BI/RBO=H, a=L, b=L, c=L, d=L, e=L, f=L, g=L"
  };

  int resul_birbo = HIGH;
  int resul_a[] = {HIGH,LOW,HIGH,HIGH,LOW,HIGH,LOW,HIGH,HIGH,HIGH,LOW,LOW,LOW,HIGH,LOW,LOW};
  //int resul_b[] = {LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH};
  //int resul_c[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
  //int resul_d[] = {LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH};
  //int resul_e[] = {LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH};
  //int resul_f[] = {LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH};
  //int resul_g[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

  bool resultado = true;

  digitalWrite(pinLT, HIGH);

  for (int i = 0; i < 16; i++) {
    if (i==0) { digitalWrite(pinRBI, statesRBI[0]); }
    else { digitalWrite(pinRBI, statesRBI[1]); }
    digitalWrite(pinA0, statesA0[i]);
    digitalWrite(pinA1, statesA1[i]);
    digitalWrite(pinA2, statesA2[i]);
    digitalWrite(pinA3, statesA3[i]);
    delay(1000); // Pequeña pausa para estabilizar

    int val_birbo = digitalRead(pinBIRBO);
    int val_a = digitalRead(pin_a);
    int val_b = digitalRead(pin_b);
    int val_c = digitalRead(pin_c);
    int val_d = digitalRead(pin_d);
    int val_e = digitalRead(pin_e);
    int val_f = digitalRead(pin_f);
    int val_g = digitalRead(pin_g);
    //resultado &= logicOperation(statesA4bits[i], statesB4bits[i], statesC4bits[i], statesD4bits[i]);
    delay(200); // Espera 0.02 segundos
    printResult(outputs4bits[i], resultado);
  }
  return resultado;
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
      pinMode(pins[10], OUTPUT); pinMode(pins[11], OUTPUT); pinMode(pins[9], INPUT); pinMode(pins[12], OUTPUT);
      break;
    case 4: // NOR cuadruple
      // Configuración de pines de la puerta 1
      pinMode(pins[2], OUTPUT); pinMode(pins[1], OUTPUT); pinMode(pins[0], INPUT);
      pinMode(pins[5], OUTPUT); pinMode(pins[4], OUTPUT); pinMode(pins[3], INPUT);
      pinMode(pins[6], OUTPUT); pinMode(pins[7], OUTPUT); pinMode(pins[8], INPUT);
      pinMode(pins[9], OUTPUT); pinMode(pins[10], OUTPUT); pinMode(pins[11], INPUT); pinMode(pins[12], OUTPUT);
      break;
    case 5: // NOT sixtuple
      // Configuración de pines de la puerta 1
      pinMode(pins[0], OUTPUT); pinMode(pins[1], INPUT);
      pinMode(pins[2], OUTPUT); pinMode(pins[3], INPUT);
      pinMode(pins[4], OUTPUT); pinMode(pins[5], INPUT);

      // Configuración de pines de la puerta 3
      pinMode(pins[6], INPUT); pinMode(pins[7], OUTPUT);
      pinMode(pins[8], INPUT); pinMode(pins[9], OUTPUT);
      pinMode(pins[10], INPUT); pinMode(pins[11], OUTPUT); pinMode(pins[12], OUTPUT);
      break;
    case 6: // NAND triple
    case 7: // AND triple
    case 8: // NOR triple
      // A, B, C, Salida
      // Configuración de pines de la puerta 1
      pinMode(pins[0], OUTPUT); pinMode(pins[1], OUTPUT); pinMode(pins[11], OUTPUT); pinMode(pins[10], INPUT);
      pinMode(pins[2], OUTPUT); pinMode(pins[3], OUTPUT); pinMode(pins[4], OUTPUT); pinMode(pins[5], INPUT);
      pinMode(pins[7], OUTPUT); pinMode(pins[8], OUTPUT); pinMode(pins[9], OUTPUT); pinMode(pins[6], INPUT); pinMode(pins[12], OUTPUT);
      break;
    case 9: // NAND doble
    case 10: // AND doble
      // A, B, C, D, Salida
      // Configuración de pines de la puerta 1
      pinMode(pins[0], OUTPUT); pinMode(pins[1], OUTPUT); pinMode(pins[3], OUTPUT); pinMode(pins[4], OUTPUT); pinMode(pins[5], INPUT);
      pinMode(pins[7], OUTPUT); pinMode(pins[8], OUTPUT); pinMode(pins[10], OUTPUT); pinMode(pins[11], OUTPUT); pinMode(pins[6], INPUT); pinMode(pins[12], OUTPUT);
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
    case 12: // OR triple
      // A, B, C, Salida
      // Configuración de pines de la puerta 1
      pinMode(pins[2], OUTPUT); pinMode(pins[3], OUTPUT); pinMode(pins[4], OUTPUT); pinMode(pins[5], INPUT); // 1
      pinMode(pins[0], OUTPUT); pinMode(pins[1], OUTPUT); pinMode(pins[6], OUTPUT); pinMode(pins[7], INPUT); // 2
      pinMode(pins[9], OUTPUT); pinMode(pins[10], OUTPUT); pinMode(pins[11], OUTPUT); pinMode(pins[8], INPUT); // 3
      pinMode(pins[12], OUTPUT);
      break;
    case 13: // OR doble
      // A, B, C, D, Salida
      // Configuración de pines de la puerta 1
      pinMode(pins[1], OUTPUT); pinMode(pins[2], OUTPUT); pinMode(pins[3], OUTPUT); pinMode(pins[4], OUTPUT); pinMode(pins[0], INPUT); 
      pinMode(pins[7], OUTPUT); pinMode(pins[8], OUTPUT); pinMode(pins[9], OUTPUT); pinMode(pins[10], OUTPUT); pinMode(pins[11], INPUT);  pinMode(pins[12], OUTPUT);
      break;
    case 14: // DECO 7 segmentos
      // LT, RBI A0, A1, A2, A3
      pinMode(pins[1], OUTPUT); pinMode(pins[3], OUTPUT); pinMode(pins[5], OUTPUT); pinMode(pins[13], OUTPUT); pinMode(pins[0], OUTPUT); pinMode(pins[4], OUTPUT);
      // BI/RBO, a, b, c, d, e, f, g
      pinMode(pins[2], INPUT); pinMode(pins[10], INPUT); pinMode(pins[9], INPUT); pinMode(pins[8], INPUT); pinMode(pins[7], INPUT); pinMode(pins[6], INPUT); pinMode(pins[12], INPUT); pinMode(pins[11], INPUT);
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
    ws.textAll(String(message) + "   OK");
  } else {
    Serial.println("\tERROR");
    ws.textAll(String(message) + "   ERROR");
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
    resultado &= logicOperation(statesA4bits[i], statesB4bits[i], statesC4bits[i], statesD4bits[i]) == val;
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
      digitalWrite(pins[12], HIGH);
      if (pruebasCuadriple(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[6], pins[10], pins[11], pins[9], andLogic2Bits)) {
        Serial.print("AND 4: OK"); ws.textAll("AND 4: OK");
      } else {
        Serial.print("AND 4: ERROR"); ws.textAll("AND 4: ERROR");
      }
      break;
    case 1: // OR
      digitalWrite(pins[12], HIGH);
      if (pruebasCuadriple(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[6], pins[10], pins[11], pins[9], orLogic2Bits)) {
        Serial.print("OR 4: OK"); ws.textAll("OR 4: OK");
      } else {
        Serial.print("OR 4: ERROR"); ws.textAll("OR 4: ERROR");
      }
      break;
    case 2: // NAND
      digitalWrite(pins[12], HIGH);
      if (pruebasCuadriple(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[6], pins[10], pins[11], pins[9], nandLogic2Bits)) {
        Serial.print("NAND 4: OK"); ws.textAll("NAND 4: OK");
      } else {
        Serial.print("NAND 4: ERROR"); ws.textAll("NAND 4: ERROR");
      }
      break;
    case 3: // XOR
      digitalWrite(pins[12], HIGH);
      if (pruebasCuadriple(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[6], pins[10], pins[11], pins[9], xorLogic2Bits)) {
        Serial.print("XOR 4: OK"); ws.textAll("XOR 4: OK");
      } else {
        Serial.print("XOR 4: ERROR"); ws.textAll("XOR 4: ERROR");
      }
      break;
    case 4: // NOR
      digitalWrite(pins[12], HIGH);
      if (pruebasCuadriple(pins[2], pins[1], pins[0], pins[5], pins[4], pins[3], pins[6], pins[7], pins[8], pins[9], pins[10], pins[11], norLogic2Bits)) {
        Serial.print("NOR 4: OK"); ws.textAll("NOR 4: OK");
      } else {
        Serial.print("NOR 4: ERROR"); ws.textAll("NOR 4: ERROR");
      }
      break;
    case 5: // NOT sixtuple
      digitalWrite(pins[12], HIGH);
      if (pruebasSixtuple(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5], pins[7], pins[6], pins[9], pins[8], pins[11], pins[10], notLogic1Bit)) {
        Serial.print("NOT 6: OK"); ws.textAll("NOT 6: OK");
      } else {
        Serial.print("NOT 6: ERROR"); ws.textAll("NOT 6: ERROR");
      }
      break;
    case 6: // NAND triple
      digitalWrite(pins[12], HIGH);
      if (pruebasTriple(pins[0], pins[1], pins[11], pins[10], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[9], pins[6], nandLogic3Bits)) {
        Serial.print("NAND 3: OK"); ws.textAll("NAND 3: OK");
      } else {
        Serial.print("NAND 3: ERROR"); ws.textAll("NAND 3: ERROR");
      }
      break;
    case 7: // AND triple
      digitalWrite(pins[12], HIGH);
      if (pruebasTriple(pins[0], pins[1], pins[11], pins[10], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[9], pins[6], andLogic3Bits)) {
        Serial.print("AND 3: OK"); ws.textAll("AND 3: OK");
      } else {
        Serial.print("AND 3: ERROR"); ws.textAll("AND 3: ERROR");
      }
      break;
    case 8: // NOR triple
      digitalWrite(pins[12], HIGH);
      if (pruebasTriple(pins[0], pins[1], pins[11], pins[10], pins[2], pins[3], pins[4], pins[5], pins[7], pins[8], pins[9], pins[6], norLogic3Bits)) {
        Serial.print("NOR 3: OK"); ws.textAll("NOR 2: OK");
      } else {
        Serial.print("NOR 3: ERROR"); ws.textAll("NOR 2: ERROR");
      }
      break;
    case 9: // NAND Doble
      digitalWrite(pins[12], HIGH);
      if (pruebasDoble(pins[0], pins[1], pins[3], pins[4], pins[5], pins[7], pins[8], pins[10], pins[11], pins[6], nandLogic4Bits)) {
        Serial.print("NAND 2: OK"); ws.textAll("NAND 2: OK");
      } else {
        Serial.print("NAND 2: ERROR"); ws.textAll("NAND 2: ERROR");
      }
      break;
    case 10: // AND Doble
      digitalWrite(pins[12], HIGH);
      if (pruebasDoble(pins[0], pins[1], pins[3], pins[4], pins[5], pins[7], pins[8], pins[10], pins[11], pins[6], andLogic4Bits)) {
        Serial.print("AND 2: OK"); ws.textAll("AND 2: OK");
      } else {
        Serial.print("AND 2: ERROR"); ws.textAll("AND 2: ERROR");
      }
      break;
    case 11: // Quad bistable transparent latch
      if (pruebasBiTrLatch(pins[9], pins[0], pins[12], pins[13]) &&
          pruebasBiTrLatch(pins[9], pins[1], pins[11], pins[10]) &&
          pruebasBiTrLatch(pins[2], pins[3], pins[7], pins[8]) &&
          pruebasBiTrLatch(pins[2], pins[4], pins[6], pins[5])) {
        Serial.print("74LS75 4: OK"); ws.textAll("74LS75 4: OK");
      } else {
        Serial.print("74LS75 4: ERROR"); ws.textAll("74LS75 4: ERROR");
      }
      break;
    case 12: // OR Triple
      digitalWrite(pins[12], HIGH);
      if (pruebasTriple(pins[2], pins[3], pins[4], pins[5], pins[0], pins[1], pins[6], pins[7], pins[9], pins[10], pins[11], pins[8], orLogic3Bits)) {
        Serial.print("OR 3: OK"); ws.textAll("OR 3: OK");
      } else {
        Serial.print("OR 3: ERROR"); ws.textAll("OR 3: ERROR");
      }
      break;
    case 13: // OR Doble
      digitalWrite(pins[12], HIGH);
      if (pruebasDoble(pins[1], pins[2], pins[3], pins[4], pins[0], pins[7], pins[8], pins[9], pins[10], pins[11], orLogic4Bits)) {
        Serial.print("OR 2: OK"); ws.textAll("OR 2: OK");
      } else {
        Serial.print("OR 2: ERROR"); ws.textAll("OR 2: ERROR");
      }
      break;
    case 14: // DECO 7 segmentos
      if (pruebasDeco7Seg(pins[1], pins[3], pins[5], pins[13], pins[0], pins[4], pins[2], pins[10], pins[9], pins[8], pins[7], pins[6], pins[12], pins[11])) {
        Serial.print("DECO 7 SEG: OK"); ws.textAll("DECO 7 SEG: OK");
      } else {
        Serial.print("DECO 7 SEG: ERROR"); ws.textAll("DECO 7 SEG: ERROR");
      }
      break;
  }
}

//-----------------------------------
// APARTIR DE ACA ES EL WEB SERVER CON EL EL WEB SOCKET
//-----------------------------------

// Función que realiza una serie de acciones y va enviando resultados al navegador
void ejecutarAcciones(int currentTest) {
  configurarPines(currentTest - 1); // Reconfigurar los pines según el test
  realizarTest(currentTest - 1);
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 14; i++) {
    pinMode(pins[i], INPUT);
  }

  // Conecta el ESP32 a la red WiFi
  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();
  const unsigned long wifiTimeout = 10000; // 10 segundos de espera máxima
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifiTimeout) {
    delay(2000);
    Serial.println("Conectando a WiFi...");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Conectado a WiFi.");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("No se pudo conectar a WiFi. Reiniciando...");
    ESP.restart();
  }

  // Configura el manejador de WebSocket
  ws.onEvent([](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
      Serial.println("Cliente conectado.");
    } else if (type == WS_EVT_DISCONNECT) {
      Serial.println("Cliente desconectado.");
    }
  });
  server.addHandler(&ws);

  // Página web con un botón y un div para mostrar resultados
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    String html = R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>WebSocket ESP32</title>
        <style>
        h1 {text-align: center;}
        body {
            font-family: Arial, sans-serif;
            margin: 15px;
        }
        .container {
            display: flex;
            align-items: center;
            justify-content: center;
            margin-bottom: 20px;
        }
        .scroll-bar {
            display: flex;
            align-items: center;
            width: 80%;
            overflow: hidden;
            border: 1px solid #ccc;
            border-radius: 8px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        }
        .scroll-content {
            display: flex;
            transition: transform 0.3s ease-in-out;
        }
        .test-card {
            flex: 0 0 200px;
            margin: 10px;
            padding: 10px;
            text-align: center;
            border: 1px solid #ddd;
            border-radius: 8px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
            transition: transform 0.2s;
            cursor: pointer;
        }
        .test-card:hover {transform: scale(1.05);}
        button {
            background-color: #007BFF;
            color: white;
            border: none;
            padding: 10px 20px;
            margin: 0 10px;
            cursor: pointer;
            border-radius: 4px;
        }
        button:hover {background-color: #0056b3;}
        .test-details {
            border: 1px solid #ddd;
            border-radius: 8px;
            padding: 20px;
            display: none;
        }
        .test-header {
            font-size: 1.5em;
            margin-bottom: 10px;
        }
        .test-description {margin-bottom: 20px;}
        .test-actions {
            display: flex;
            justify-content: space-between;
            gap: 20px;
        }
        .status-section, .monitor-section {
            flex: 1;
            border: 1px solid #ccc;
            border-radius: 8px;
            padding: 10px;
            height: 200px;
            overflow-y: auto;
        }
        .status-section {text-align: center;}
        .status-indicator {
            display: inline-block;
            width: 80px;
            height: 40px;
            line-height: 40px;
            margin: 10px;
            border-radius: 4px;
            color: white;
        }
        .ok {background-color: green;}
        .error {background-color: red;}
        .monitor-section {
            font-family: monospace;
            background-color: #f8f9fa;
            margin-bottom: 0px;
        }
        .monitor-section p {margin: 0px;}
        .execute-button {
            margin-top: 10px;
            background-color: #28a745;
            color: white;
            padding: 10px 20px;
            border: none;
            cursor: pointer;
            border-radius: 4px;
        }
        .execute-button:hover {background-color: #218838;}
        </style>
      </head>
      <body>
        <h1>Pruebas IoT</h1>
        <div class="container">
          <button onclick="scrollIzquierda()">⬅</button>
          <div class="scroll-bar">
            <div id="scroll-content" class="scroll-content">
              <div class="test-card" onclick="selectTest(1)">
                <h2>74HC08</h2>
              </div>
              <div class="test-card" onclick="selectTest(2)">
                <h2>74HC32</h2>
              </div>
              <div class="test-card" onclick="selectTest(3)">
                <h2>74HC00</h2>
              </div>
              <div class="test-card" onclick="selectTest(4)">
                <h2>74HC86</h2>
              </div>
              <div class="test-card" onclick="selectTest(5)">
                <h2>74HC02</h2>
              </div>
              <div class="test-card" onclick="selectTest(6)">
                <h2>74HC04</h2>
              </div>
              <div class="test-card" onclick="selectTest(7)">
                <h2>74HC10</h2>
              </div>
              <div class="test-card" onclick="selectTest(8)">
                <h2>74HC11</h2>
              </div>
              <div class="test-card" onclick="selectTest(9)">
                <h2>74HC27</h2>
              </div>
              <div class="test-card" onclick="selectTest(10)">
                <h2>74HC20</h2>
              </div>
              <div class="test-card" onclick="selectTest(11)">
                <h2>74HC21</h2>
              </div>
              <div class="test-card" onclick="selectTest(12)">
                <h2>74LS75</h2>
              </div>
              <div class="test-card" onclick="selectTest(13)">
                <h2>74HC4075</h2>
              </div>
              <div class="test-card" onclick="selectTest(14)">
                <h2>74HC4072</h2>
              </div>
              <div class="test-card" onclick="selectTest(15)">
                <h2>74LS48</h2>
              </div>
              <div class="test-card" onclick="selectTest(0)">
                <h2>FALTA...</h2>
              </div>
            </div>
          </div>
          <button onclick="scrollRight()">➡</button>
        </div>

        <div id="test-details" class="test-details">
            <div class="test-header" id="test-header">Prueba 1</div>
            <div class="test-description" id="test-description">
                Descripción detallada de la prueba seleccionada.
            </div>
            <div class="test-actions">
                <div class="status-section">
                    <div id="status-ok" class="status-indicator ok">OK</div>
                    <div id="status-error" class="status-indicator error">ERROR</div>
                    <button class="execute-button" onclick="executeTest()">Ejecutar</button>
                </div>
                <div class="monitor-section" id="monitor-section">
                    <p>Monitor serial de pruebas:</p>
                </div>
            </div>
        </div>

        <script>
          const scrollContent = document.getElementById('scroll-content');
          let scrollPosition = 0;
          const testDetails = document.getElementById('test-details');
          const testHeader = document.getElementById('test-header');
          const testDescription = document.getElementById('test-description');
          const monitorSection = document.getElementById('monitor-section');
          let selectedTestNumber = null;

          function scrollIzquierda() {
            const cardWidth = 220;
            scrollPosition = Math.max(scrollPosition - cardWidth, 0);
            scrollContent.style.transform = `translateX(-${scrollPosition}px)`;
          }

          function scrollRight() {
            const cardWidth = 220;
            const maxScroll = scrollContent.scrollWidth - scrollContent.parentElement.clientWidth;
            scrollPosition = Math.min(scrollPosition + cardWidth, maxScroll);
            scrollContent.style.transform = `translateX(-${scrollPosition}px)`;
          }

          function selectTest(testNumber) {
            selectedTestNumber = testNumber;
            testDetails.style.display = 'block';
            testHeader.textContent = `Prueba ${testNumber}`;
            testDescription.textContent = `Descripción detallada de la prueba ${testNumber}.`;
            monitorSection.innerHTML = '<p>Monitor serial de pruebas:</p>';
          }
          function executeTest() {
            if (selectedTestNumber === null) {
              alert("Por favor, selecciona una prueba primero.");
              return;
            }
            fetch(`/ejecutar?parametro=${encodeURIComponent(selectedTestNumber)}`)
              .then(response => {
                if (!response.ok) {
                  throw new Error(`Error: ${response.status}`);
                }
                return response.text();
              })
              .then(data => {
                console.log(data);
                monitorSection.innerHTML += `<p>${data}</p>`;
              })
              .catch(error => {
                console.error('Error al realizar la solicitud:', error);
                monitorSection.innerHTML += `<p style="color:red;">Error: ${error.message}</p>`;
              });
          }
          
          const websocket = new WebSocket(`ws://${window.location.host}/ws`);
          websocket.onmessage = function(event) {
            // Añadir el mensaje recibido al monitor
            monitorSection.innerHTML += '<p>' + event.data + '</p>';
            monitorSection.scrollTop = monitorSection.scrollHeight; // Mantener el scroll al último mensaje
        
            // Analizar el mensaje para verificar si contiene "ERROR"
            if (event.data.includes('ERROR')) {
                // Mostrar el mensaje de error y cambiar los colores de estado
                //monitorSection.innerHTML += `<p>Prueba fallida.</p>`;
                document.getElementById('status-ok').style.backgroundColor = '#ccc';
                document.getElementById('status-error').style.backgroundColor = 'red';
            } else {
              // Mostrar el mensaje de éxito y cambiar los colores de estado
              //monitorSection.innerHTML += `<p>Prueba completada con éxito.</p>`;
              document.getElementById('status-ok').style.backgroundColor = 'green';
              document.getElementById('status-error').style.backgroundColor = '#ccc';
            }
          };
        </script>

      </body>
      </html>
    )rawliteral";
    request->send(200, "text/html", html);
  });

  // Ruta que ejecuta las acciones
  server.on("/ejecutar", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->hasParam("parametro")) {
      String parametroStr = request->getParam("parametro")->value();
      int parametroInt = parametroStr.toInt();
      if (parametroInt >= 0) { // Validar que el parámetro es válido
        request->send(200, "text/plain", "Acciones iniciadas con el parámetro: " + String(parametroInt));
        ejecutarAcciones(parametroInt);
      } else {
        request->send(400, "text/plain", "Parámetro inválido.");
      }
    } else {
      request->send(400, "text/plain", "Falta el parámetro.");
    }
  });

  // Inicia el servidor
  server.begin();
}

void loop() {
  // No es necesario código aquí cuando se usa ESPAsyncWebServer
}
