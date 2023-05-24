#include<Servo.h>

//Define os pinos do sensor de cores
#define s0 2 //Pino S0 do sensor no pino 2 do arduino
#define s1 3 //Pino S1 do sensor no pino 3 do arduino
#define s2 4 //Pino S2 do sensor no pino 4 do arduino
#define s3 5 //Pino S3 do sensor no pino 5 do arduino
#define sensorOut 6 //Pino output do sensor no pino 6 do arduino


//Variável para salvar os dados vindos do sensorOut
int frequency = 0;

// Variável para salvar cor
byte identifiedColor = 0;

//Declarando servos

Servo feedingServo;
Servo decisionServo;

//Declarando delays gerais
byte smallDelay_ms = 5;
byte mediumDelay_ms = 30;
byte bigDelay_ms = 1000;
byte veryBigDelay_ms = 2000;


//Declarando posição do feedservo
byte pos1FeederServo = 165;
byte pos2FeederServo = 92;
byte pos3FeederServo = 12;

//Declarando posição do decisionservo
byte decisionServo_RED = 14;
byte decisionServo_ORANGE = 53;
byte decisionServo_GREEN = 85;
byte decisionServo_YELLOW = 115;
byte decisionServo_PURPLE = 165;

void setup() {

  //Define os pinos do sensor de cores
  pinMode (s0, OUTPUT);
  pinMode (s1, OUTPUT);
  pinMode (s2, OUTPUT);
  pinMode (s3, OUTPUT);
  pinMode (sensorOut, INPUT);

  /* 
      L - L = Sem energia
      L - H = 2%
      H - L = 20%
      H - H = 100%
  */
  //Usando 100%
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);

  //juntando os servos
  feedingServo.attach (9);
  decisionServo.attach(11);

  //Define a comunicação serial em bytes por segundo
  Serial.begin(9600);

}//Encerra o setup

void loop() {

  //Posição sensor
  for (int i = pos1FeederServo; i > pos2FeederServo ; i--) {

    feedingServo.write (i);
    delay (mediumDelay_ms);

  }

  
  delay(bigDelay_ms);


  identifiedColor = getTheColor();


  switch (identifiedColor) {

    //No caso da cor vermelha
    case 1:
      decisionServo.write (decisionServo_RED);
      break;

    //No caso da cor laranja
    case 2:
      decisionServo.write (decisionServo_ORANGE);
      break;

    //No caso da cor verde
    case 3:
      decisionServo.write (decisionServo_GREEN);
      break;

    //No caso da cor amarela
    case 4:
      decisionServo.write (decisionServo_YELLOW);
      break;

    //No caso da cor roxa
    case 5:
      decisionServo.write (decisionServo_PURPLE);
      break;

  }// Encerra o switch

  //Delay posição de saída
  delay(bigDelay_ms);


  //move o servo de alimentação para posição de saída
  for (int i = pos2FeederServo; i > pos3FeederServo; i--) {

    feedingServo.write (i);
    delay (mediumDelay_ms);

  }//Encerra o loop

  //Delay para retornar posiçaõ servo
  delay(bigDelay_ms);


  //Retorna o alimentador para posição original
  for (int i = pos3FeederServo; i < pos1FeederServo ; i++) {

    feedingServo.write (i);
    delay (smallDelay_ms);
  }//Encerra o loop - Servo na posição original


  //Retorna o valor para 0
  identifiedColor = 0;

  //Delay para começãr novamente
  delay (veryBigDelay_ms);

}//encerra loop


int getTheColor() {


  //---Lê os valores vermelhos... S2 LOW - S3 LOW---
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  frequency = pulseIn(sensorOut, LOW);

  int redValue = frequency;

  Serial.print("R=");
  Serial.print(redValue);
  Serial.print("  ");

  delay(50);

  //---Lê os valores verdes ... S2 LOW - S3 HIGH---
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  frequency = pulseIn(sensorOut, LOW);

  int greenValue = frequency;

  Serial.print("G=");
  Serial.print(greenValue);
  Serial.print("  ");

  delay(50);


  //---Lê os valores azuis ... S2 HIGH - S3 LOW---
  digitalWrite(s2, HIGH);
  digitalWrite(s3, LOW);
  frequency = pulseIn(sensorOut, LOW);

  int blueValue = frequency ;

  Serial.print("B=");
  Serial.print(blueValue);
  Serial.print("  ");

  delay(50);


    //---Lê os valores limpos ... S2 HIGH - S3 HIGH---
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  frequency = pulseIn(sensorOut, LOW);

  int clearValue = frequency ;

  Serial.print("C=");
  Serial.print(clearValue);
  Serial.println("  ");

  delay(50);


  //////////Parametros vermelho//////////
  if (redValue<38 & redValue>14 & clearValue<58 & clearValue>=45 ) {
    identifiedColor = 1; // vermelho
  }

  //////////Parametros laranja//////////
  if (redValue<27 & redValue>10  & clearValue<45 & clearValue>=35) {
    identifiedColor = 2; // laranja
  }


  //////////Parametros verde//////////
  if (blueValue<15 & blueValue>4 & clearValue<35 & clearValue>20 ) {
    identifiedColor = 3; // verde
  }

  //////////Parametros amarelo//////////
  if (redValue<20 & redValue>8 & clearValue<24 & clearValue>10 ) {
    identifiedColor = 4; // amarelo

  //////////Parametros roxo//////////
  if ( redValue<49 & redValue>39  & clearValue<65 & clearValue>50) {
    identifiedColor = 5; // roxo
  }


  //--- Retorna para valor encontrado---