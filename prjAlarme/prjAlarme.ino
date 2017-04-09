#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

int ledVermelho = 7; //Pino do led de alarme ativado
int ledVerde = 6; //Pino do led de alarme desativado
int reed1 = 2; //Pino do sensor magnético
int reed2 = 3; //Pino do sensor magnético
int reed3 = 4; //Pino do sensor magnético
int reed4 = 5; //Pino do sensor magnético
int rele = 12; //Pino do relay
int sensor = A0; //Pino do sendor de movimento
int botaoLiga = 8; //Pino do botão ligar
int botaoDesliga = 9; //Pino do botão desligar
int RF = 10; //Pino do modulo RF 433Mhz

int valorSensor; //Armazena o estado do sensor de movimento
int valorLiga; //Armazena o valor do botão de ligar o alarme
int valorDesliga; //Armazena o valor do botão de desligar o alarme
int valorReed1; //Armazena o valor so sensor magnético
int valorReed2; //Armazena o valor so sensor magnético
int valorReed3; //Armazena o valor so sensor magnético
int valorReed4; //Armazena o valor so sensor magnético


void setup() {
  Serial.begin(9600);
  mySwitch.enableTransmit(RF);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(rele, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(reed1, INPUT);
  pinMode(reed2, INPUT);
  pinMode(reed3, INPUT);
  pinMode(reed4, INPUT);
  pinMode(botaoLiga, INPUT);
  pinMode(botaoDesliga, INPUT);
  digitalWrite(rele, HIGH);

}

void loop() {
  valorLiga = digitalRead(botaoLiga); //Lendo o botão ligar
  
  if (valorLiga != 1) //Se o botão ligar não for pressionado
  {
    while(digitalRead(botaoLiga) != 1) //Esquanto o botão ligar não for pressionado
    {
      digitalWrite(ledVerde, HIGH); //Acende o led de alarme desligado
      digitalWrite(ledVermelho, LOW); //Apaga o led de alarme ligado
    }//Botão foi pressionado pelo menos 1 vez e sai do While
    ligaAlarme(); //Chama a função para ligar o alarme
  }
}

void ligaAlarme(){ 
  
  valorDesliga = digitalRead(botaoDesliga); //Lendo o botão desligar
  
  if (valorDesliga != 1) //Se o botão desligar não for pressionado
  {
    while(digitalRead(botaoDesliga) != 1) //Esquanto o botão ligar não for pressionado
    {
      digitalWrite(ledVerde, LOW); //Apaga o led de alarme desligado
      digitalWrite(ledVermelho, HIGH); //Acende o led de alarme ligado
      
      valorSensor = digitalRead(sensor); //Lendo valor do sensor de movimento
      valorReed1 = digitalRead(reed1); //Lendo valor do sensor magnético
      valorReed2 = digitalRead(reed2); //Lendo valor do sensor magnético
      valorReed3 = digitalRead(reed3); //Lendo valor do sensor magnético
      valorReed4 = digitalRead(reed4); //Lendo valor do sensor magnético

      Serial.print(valorReed1);
      Serial.print(valorReed2);
      Serial.print(valorReed3);
      Serial.print(valorReed4);
      Serial.print(valorSensor);
      
      if ((valorReed1 != 1) || (valorReed2 != 1) || (valorReed3 != 1) || (valorReed4 != 1) || (valorSensor != 0)){ //Se o sensor magnético se separar (abriu uma porta ou janela) ou houver movimento no ambiente
        mySwitch.sendTriState("0FFF0FF1FFFF"); //Liga a luz do ventilador
        digitalWrite(rele, LOW); //Aciona o relay com o buzzer
        while(digitalRead(botaoDesliga) != 1) //Esquanto o botão ligar não for pressionado
        {
          digitalWrite(ledVermelho, HIGH); //Acende o led vermelho
          delay(100); //Tempo do pisca
          digitalWrite(ledVermelho, LOW); ///Apaga o led vermelho
          delay(100); //Tempo do pisca
        }
      }
    }//Botão foi pressionado pelo menos 1 vez e sai do While
    digitalWrite(rele, HIGH); ///Desliga o relay
    return; //Volta para o Loop 
  }
}
