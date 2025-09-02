#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Configuración del teclado
const byte FILAS = 4;
const byte COLUMNAS = 4;

char teclas[FILAS][COLUMNAS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pinesFilas[FILAS] = {13,12,11,10};   
byte pinesColumnas[COLUMNAS] = {9,8,7,6};

Keypad teclado = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, FILAS, COLUMNAS);

// Pines de control motores
int enable0 = 0;  
int enable1 = 1;  
int pinMotorA1 = 2;
int pinMotorA2 = 3;
int pinMotorB1 = 4;
int pinMotorB2 = 5;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando sistema...");

  lcd.init();         // si no funciona, cambiar por lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Iniciando...");

  // Configuración pines de motores
  pinMode(enable0, OUTPUT);
  pinMode(enable1, OUTPUT);
  pinMode(pinMotorA1, OUTPUT);
  pinMode(pinMotorA2, OUTPUT);
  pinMode(pinMotorB1, OUTPUT);
  pinMode(pinMotorB2, OUTPUT);

  detenerMotor('A');
  detenerMotor('B');

  delay(1000);
  lcd.clear();
  Serial.println("Sistema listo.");
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("1:on 2:off");
  lcd.setCursor(0,1);
  lcd.print("3:right 4:left");

  char tecla = teclado.getKey();

  if(tecla){
    Serial.print("Tecla presionada: ");
    Serial.println(tecla);

    lcd.clear();
    switch(tecla){
      case '1':
        lcd.print("Motor A/B?");
        Serial.println("Selecciona motor a encender.");
        esperaMotor(true);
        break;

      case '2':
        lcd.print("Motor A/B?");
        Serial.println("Selecciona motor a apagar.");
        esperaMotor(false);
        break;

      case '3':
        lcd.print("Avanzar A/B?");
        Serial.println("Selecciona motor para avanzar.");
        esperaGiro(true);
        break;

      case '4':
        lcd.print("Retroceder A/B?");
        Serial.println("Selecciona motor para retroceder.");
        esperaGiro(false);
        break;

      default:
        Serial.println("Tecla no asignada.");
        break;
    }
  }
}

void encenderMotor(char motor){
  Serial.print("Encendiendo motor ");
  Serial.println(motor);
  if(motor=='A'){
    digitalWrite(enable0, HIGH);
  }else if(motor=='B'){
    digitalWrite(enable1, HIGH);
  }
}

void apagarMotor(char motor){
  Serial.print("Apagando motor ");
  Serial.println(motor);
  if(motor=='A'){
    digitalWrite(enable0, LOW);
    detenerMotor('A');
  }else if(motor=='B'){
    digitalWrite(enable1, LOW);
    detenerMotor('B');
  }
}

void avanzarMotor(char motor){
  Serial.print("Avanzar motor ");
  Serial.println(motor);
  if(motor=='A'){ //pin 1 HIGH, 2 low
    digitalWrite(pinMotorA1, HIGH);
    digitalWrite(pinMotorA2, LOW);
    encenderMotor('A');
  }else if(motor=='B'){
    digitalWrite(pinMotorB1, HIGH);
    digitalWrite(pinMotorB2, LOW);
    encenderMotor('B');
  }
}

void retroMotor(char motor){
  Serial.print("Retroceder motor ");
  Serial.println(motor);
  if(motor=='A'){
    digitalWrite(pinMotorA1, LOW);
    digitalWrite(pinMotorA2, HIGH);
    encenderMotor('A');
  }else if(motor=='B'){
    digitalWrite(pinMotorB1, LOW);
    digitalWrite(pinMotorB2, HIGH);
    encenderMotor('B');
  }
}

void detenerMotor(char motor){
  Serial.print("Detener motor ");
  Serial.println(motor);
  if(motor=='A'){
    digitalWrite(pinMotorA1, LOW);
    digitalWrite(pinMotorA2, LOW);
  }else if(motor=='B'){
    digitalWrite(pinMotorB1, LOW);
    digitalWrite(pinMotorB2, LOW);
  }
}

void esperaMotor(bool encender){ 
  char tecla=0;
  while(!tecla){
    tecla = teclado.getKey();
    if(tecla=='A'){
      if(encender) encenderMotor('A');
      else apagarMotor('A');
      lcd.clear();
      lcd.print("Motor A listo");
      Serial.println("Accion ejecutada en Motor A.");
      delay(1000);
    }
    else if(tecla=='B'){
      if(encender) encenderMotor('B');
      else apagarMotor('B');
      lcd.clear();
      lcd.print("Motor B listo");
      Serial.println("Accion ejecutada en Motor B.");
      delay(1000);
    }
  }
}

void esperaGiro(bool horario){
  char tecla=0;
  while(!tecla){
    tecla = teclado.getKey();
    if(tecla=='A'){
      if(horario) avanzarMotor('A');
      else retroMotor('A');
      lcd.clear();
      lcd.print("Motor A girando");
      Serial.println("Motor A en movimiento.");
      delay(1000);
    }
    else if(tecla=='B'){
      if(horario) avanzarMotor('B');
      else retroMotor('B');
      lcd.clear();
      lcd.print("Motor B girando");
      Serial.println("Motor B en movimiento.");
      delay(1000);
    }
  }
}
