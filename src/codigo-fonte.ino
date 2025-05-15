#include <Arduino.h>
#include <EEPROM.h>

// =============== CONFIGURAÇÃO DOS PINOS ===============
const int sensorPins[5] = {32, 33, 34, 35, 36};
const int ENA = 25;
const int IN1 = 26;
const int IN2 = 27;
const int ENB = 14;
const int IN3 = 12;
const int IN4 = 13;

// =============== PARÂMETROS AJUSTÁVEIS ===============
const int velMAX = 160;
const int velMED = 130;
const int velMIN = 100;

// =============== VARIÁVEIS GLOBAIS ===============
int sensorValues[5];
unsigned long lastTime = 0;
int ultimoComando = 0;

// =============== FUNÇÕES DE MOVIMENTO ===============
void setMotores(int velEsq, int velDir) {
    velEsq = constrain(velEsq, -255, 255);
    velDir = constrain(velDir, -255, 255);
    
    digitalWrite(IN1, velEsq > 0 ? HIGH : LOW);
    digitalWrite(IN2, velEsq > 0 ? LOW : HIGH);
    digitalWrite(IN3, velDir > 0 ? HIGH : LOW);
    digitalWrite(IN4, velDir > 0 ? LOW : HIGH);
    
    ledcWrite(0, abs(velEsq));
    ledcWrite(1, abs(velDir));
}

void moverFrente() {
    setMotores(velMED, velMED);
    ultimoComando = 0;
}

void moverEsquerdaSuave() {
    setMotores(velMIN, velMED);
    ultimoComando = 1;
}

void moverDireitaSuave() {
    setMotores(velMED, velMIN);
    ultimoComando = 2;
}

void moverEsquerdaForte() {
    setMotores(0, velMAX);
    ultimoComando = 3;
}

void moverDireitaForte() {
    setMotores(velMAX, 0);
    ultimoComando = 4;
}

void parar() {
    setMotores(0, 0);
    ultimoComando = 5;
}

// =============== FUNÇÕES DE SENSORES ===============
void lerSensores() {
    for (int i = 0; i < 5; i++) {
        sensorValues[i] = analogRead(sensorPins[i]);
    }
}

bool estaNaLinha(int sensor) {
    return sensorValues[sensor] < 2000;  // Ajuste este valor conforme necessário
}

void printSensores() {
    if (millis() - lastTime > 250) {
        Serial.print("Sensores: ");
        for (int i = 0; i < 5; i++) {
            Serial.print(sensorValues[i]);
            Serial.print("\t");
        }
        Serial.print(" Comando: ");
        Serial.println(ultimoComando);
        lastTime = millis();
    }
}

// =============== FUNÇÃO PRINCIPAL DE CONTROLE ===============
void seguirLinha() {
    lerSensores();
    
    // Verifica cada sensor e decide a ação
    if (estaNaLinha(2)) {  // Sensor central
        if (estaNaLinha(1) && !estaNaLinha(3)) {
            // Tendência para esquerda
            moverEsquerdaSuave();
        }
        else if (!estaNaLinha(1) && estaNaLinha(3)) {
            // Tendência para direita
            moverDireitaSuave();
        }
        else {
            // Centralizado
            moverFrente();
        }
    }
    else if (estaNaLinha(1) || estaNaLinha(0)) {
        // Curva à esquerda detectada
        if (estaNaLinha(0)) {
            moverEsquerdaForte();
        } else {
            moverEsquerdaSuave();
        }
    }
    else if (estaNaLinha(3) || estaNaLinha(4)) {
        // Curva à direita detectada
        if (estaNaLinha(4)) {
            moverDireitaForte();
        } else {
            moverDireitaSuave();
        }
    }
    else {
        // Nenhum sensor na linha - tenta recuperar baseado no último comando
        switch (ultimoComando) {
            case 1:
            case 3:
                moverEsquerdaForte();
                break;
            case 2:
            case 4:
                moverDireitaForte();
                break;
            default:
                // Se estava indo reto, alterna entre procurar esquerda e direita
                static bool procurarEsquerda = true;
                if (procurarEsquerda) {
                    moverEsquerdaForte();
                } else {
                    moverDireitaForte();
                }
                procurarEsquerda = !procurarEsquerda;
                break;
        }
    }
    
    printSensores();
}

// =============== SETUP ===============
void setup() {
    Serial.begin(115200);
    
    // Configuração dos pinos
    for (int i = 0; i < 5; i++) {
        pinMode(sensorPins[i], INPUT);
    }

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    ledcSetup(0, 1000, 8);
    ledcSetup(1, 1000, 8);
    ledcAttachPin(ENA, 0);
    ledcAttachPin(ENB, 1);

    Serial.println("Sistema iniciado!");
    delay(1000);
}

// =============== LOOP PRINCIPAL ===============
void loop() {
    seguirLinha();
    delay(10);
}