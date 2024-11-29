int button = 13;
int button_2 = 4;

int ledRed_1 = 12;
int ledRed_2 = 11;
int ledYellow_1 = 10;
int ledYellow_2 = 9;
int ledGreen_1 = 8;
int ledGreen_2 = 7;

bool bandera = false;      // Controla el ciclo del semáforo
bool lastButtonState = HIGH;
bool lastButtonState_2 = HIGH;

void setup() {
  Serial.begin(9600);

  pinMode(button, INPUT_PULLUP);  // Usar resistencias internas pull-up
  pinMode(button_2, INPUT_PULLUP);

  pinMode(ledRed_1, OUTPUT);
  pinMode(ledRed_2, OUTPUT);
  pinMode(ledYellow_1, OUTPUT);
  pinMode(ledYellow_2, OUTPUT);
  pinMode(ledGreen_1, OUTPUT);
  pinMode(ledGreen_2, OUTPUT);

  // Apagar todos los LEDs inicialmente
  apagarSemaforo();
}

void loop() {
  // Leer el estado del botón 1 (encender ciclo)
  bool state = digitalRead(button);
  if (state == LOW && lastButtonState == HIGH) {
    bandera = true;  // Activar el semáforo
    Serial.println("Semáforo encendido");
    delay(200); // Antirrebote
  }
  lastButtonState = state;

  // Leer el estado del botón 2 (apagar ciclo)
  bool state_2 = digitalRead(button_2);
  if (state_2 == LOW && lastButtonState_2 == HIGH) {
    bandera = false;  // Desactivar el semáforo
    apagarSemaforo();
    Serial.println("Semáforo apagado");
    delay(200); // Antirrebote
  }
  lastButtonState_2 = state_2;

  // Si el ciclo está activo, ejecutar el semáforo
  if (bandera) {
    ejecutarSemaforo();
  }
}

// Función para apagar todos los LEDs
void apagarSemaforo() {
  digitalWrite(ledRed_1, LOW);
  digitalWrite(ledRed_2, LOW);
  digitalWrite(ledYellow_1, LOW);
  digitalWrite(ledYellow_2, LOW);
  digitalWrite(ledGreen_1, LOW);
  digitalWrite(ledGreen_2, LOW);
}

// Función para ejecutar el ciclo del semáforo
void ejecutarSemaforo() {
  // Semáforo 1 en verde, semáforo 2 en rojo
  digitalWrite(ledGreen_1, HIGH);
  digitalWrite(ledRed_2, HIGH);
  delayInterruptible(4000); // Mantener esta configuración 4 segundos

  // Semáforo 1 amarillo
  digitalWrite(ledGreen_1, LOW);
  digitalWrite(ledYellow_1, HIGH);
  delayInterruptible(1000);

  // Semáforo 1 rojo, semáforo 2 verde
  digitalWrite(ledYellow_1, LOW);
  digitalWrite(ledRed_1, HIGH);
  digitalWrite(ledRed_2, LOW);
  digitalWrite(ledGreen_2, HIGH);
  delayInterruptible(4000);

  // Semáforo 2 amarillo
  digitalWrite(ledGreen_2, LOW);
  digitalWrite(ledYellow_2, HIGH);
  delayInterruptible(1000);

  // Semáforo 2 rojo, semáforo 1 en espera
  digitalWrite(ledYellow_2, LOW);
  digitalWrite(ledRed_2, HIGH);
  digitalWrite(ledRed_1, LOW);
}

// Función personalizada para delay que verifica el botón de apagado
void delayInterruptible(unsigned long tiempo) {
  unsigned long inicio = millis();
  while (millis() - inicio < tiempo) {
    bool state_2 = digitalRead(button_2);
    if (state_2 == LOW) {
      bandera = false;
      apagarSemaforo();
      Serial.println("Semáforo interrumpido");
      return;
    }
  }
}
