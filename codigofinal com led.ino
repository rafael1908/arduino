
#include <Wire.h>                       <-- biblioteca do modulo LCD
#include <LiquidCrystal_I2C.h>          <-- biblioteca LCD
#include <Servo.h>                      <-- BibliotecaServo motor
#include <Ultrasonic.h>			<-- Biblioteca sensor ultrasonico

//sensor entrada (sensor 1)
#define pt1 4      <-- pt1 nome usado pra identificar as portas (p= porta/ t=trig / 1 = numero da vaga) e 4 (onde ligará no Arduino)
#define pe1 5	   <-- pe1 nome usado pra identificar as portas (p= porta/ e=echo / 1 = numero da vaga) e 5 (onde ligará no Arduino)

//sensor saida (sensor 2)
#define pt2 2
#define pe2 3

//sensor vaga 1 (sensor 3)
#define pt3 6
#define pe3 7

//sensor vaga 2 (sensor 4)
#define pt4 8
#define pe4 9

//sensor vaga 3 (sensor 5)
#define pt5 10
#define pe5 11

//sensor vaga 4 (sensor 6)
#define pt6 12 
#define pe6 13

//servo motor
Servo entrada; <-- declarando a variavel(entrada foi o nome dado ao motor pra usar no codigo)
Servo saida;	<-- declarando a variavel(saida foi o nome dado ao motor pra usar no codigo)

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Portas A5 e A4

//sensores
Ultrasonic ultrasonic1(pt1, pe1); // entrada
Ultrasonic ultrasonic2(pt2, pe2); // saida
Ultrasonic ultrasonic3(pt3, pe3); // vaga 1
Ultrasonic ultrasonic4(pt4, pe4); // vaga 2
Ultrasonic ultrasonic5(pt5, pe5); // vaga 3
Ultrasonic ultrasonic6(pt6, pe6); // vaga 4

int controle1=0; //variavel para controle de abertura e fechamento da cancela da entrada
int controle2=0; //variavel para controle de abertura e fechamento da cancela da saida 

int totalvagas = 4; //controla a quantidade de vaga disponivel

int controlevg1=0;   //variavel para controlar as vagas disponivel
int controlevg2 =0;  //variavel para controlar as vagas disponivel
int controlevg3 = 0; //variavel para controlar as vagas disponivel
int controlevg4 = 0; //variavel para controlar as vagas disponivel

int fluxo = 0; // variavel controle de fluxo diario do estacionamento, usado no if de entrada
int ledVerde = 0 ;
int ledVermelho = A3 ;
int ctrlLed = 0;

long vaga1=0;
long vaga2=0;
long vaga3=0;
long vaga4=0;

void setup()
{
  entrada.attach(A1); //fio branco
  saida.attach(A0); // fio amarelo
  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");
  Serial.println("\nI2C Scanner");
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
}

//-------------------- Função para decremento das vagas ao sair -----------------------------
void Vagas(){
  totalvagas = 4;
     
        totalvagas = totalvagas - (controlevg1 + controlevg2 + controlevg3 + controlevg4);
     
  }

// --------------------- Função para os leds de entrada --------------------------------------
void Leds(){

  //Controle do led piscando na entrada
 if(controle1 == 0 && controle2 == 0){
    if(ctrlLed == 0){
        digitalWrite(ledVermelho, LOW);  // desligar o led vermelho
        digitalWrite(ledVerde, HIGH);  // ligar o led verde
        ctrlLed=1;
    }else{
        digitalWrite(ledVermelho, HIGH);  // ligar o led vermelho
        digitalWrite(ledVerde, LOW);  // desligar o led verde
        ctrlLed=0;
    }
 }else{
        digitalWrite(ledVermelho, HIGH);  // ligar o led vermelho
        digitalWrite(ledVerde, LOW);  // ligar o led verde
 } 
}


void loop()
{

  //Le as informacoes do sensor em cm
  float cmMsecEntrada, cmMsecSaida, vaga1, vaga2, vaga3, vaga4;
  long mcsec1 = ultrasonic1.timing();
  long mcsec2 = ultrasonic2.timing();
  long mcsec3 = ultrasonic3.timing();
  long mcsec4 = ultrasonic4.timing();
  long mcsec5 = ultrasonic5.timing();
  long mcsec6 = ultrasonic6.timing();

 
  Vagas();
  
  cmMsecEntrada = ultrasonic1.convert(mcsec1, Ultrasonic::CM);
  cmMsecSaida = ultrasonic2.convert(mcsec2, Ultrasonic::CM);
  vaga1 = ultrasonic3.convert(mcsec3, Ultrasonic::CM);
  vaga2 = ultrasonic4.convert(mcsec4, Ultrasonic::CM);
  vaga3 = ultrasonic5.convert(mcsec5, Ultrasonic::CM);
  vaga4 = ultrasonic6.convert(mcsec6, Ultrasonic::CM);


  Serial.print("entrada: ");
  Serial.println(cmMsecEntrada);

  Serial.print("Saida: ");
  Serial.println(cmMsecSaida);

   
   Serial.print("vaga 1: ");
  Serial.println(vaga1);
   
   Serial.print("vaga 2: ");
  Serial.println(vaga2);
  
    Serial.print("vaga 3: ");
  Serial.println(vaga3);
  
    Serial.print("vaga 4: ");
  Serial.println(vaga4);

  delay(2000);

   lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispon.");
  lcd.setCursor(8, 0);
  lcd.print(totalvagas);
  lcd.setCursor(10, 0);
  lcd.print("vagas");
    
  lcd.setCursor(0, 1);
  lcd.print("Fluxo diario");
  lcd.setCursor(13, 1);
  lcd.print(fluxo);

  Leds(); //função controle dos leds de entrada VERDE e VERMELHO
  
  // ---------------- Condição de entrada ABRIR --------------------------------------------
  if (cmMsecEntrada < 7.0 && cmMsecEntrada > 0.9 && controle1 == 0 && totalvagas <= 4) {

      fluxo++; // contagem de veiculos diario
    
     if(totalvagas <= 4 && totalvagas > 0){ 
        Vagas();
        Serial.print("vagas disponiveis ");
        Serial.println(totalvagas);
        
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Bem vindo!");

         for(int i=90; i <= 170 ; i++){
            entrada.write(i); //regular angulo na maket montada
            delay(20);
         }
        
        controle1 = 1 ;
       
     }else{
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Estacionamento");
        lcd.setCursor(2, 1);
        lcd.print("** Lotado **");
        delay(3000);
        
     }
  }

  //------------------- Condição de entrada FECHAR ---------------------------------------------------
   if (cmMsecEntrada > 12.0 && controle1 == 1) {
    
     //delay(300); //regular tempo de fechamento 
     
     for(int i2=170; i2 >= 90; i2--){
            entrada.write(i2); //regular angulo na maket montada
            delay(20);
     }
     
     //digitalWrite(ledVermelho, LOW);  // desligar o led vermelho
     //digitalWrite(ledVerde, HIGH);  // ligar o led verde
     controle1=0;
  }


  // ------------------- Condição de saida ABRIR -----------------------------------------------------
  if(cmMsecSaida < 12.0 && cmMsecSaida > 0.0 && controle2 == 0){

     lcd.clear();
     lcd.setCursor(7, 1);
     lcd.print("Volte sempre");
     delay(100); // mudar para 5000
     lcd.clear();
     for(int i=100; i < 231; i++){
     saida.write(i); //regular angulo na maket montada
     delay(20);
     }
     controle2 = 1 ;
    }

  // -------------------- Condição de saida FECHAR ---------------------------------------------------
   if (cmMsecSaida > 12.0 && controle2 == 1) {
     delay(500); //mudar para 5000, delay tempo de fechamento cancela de saida
     
     for(int i=230; i > 90; i--){
     saida.write(i); //regular angulo na maket montada
     delay(20);
     }
     controle2=0;
     
     
  }


  //----------------- VAGA 1 ----------------------------------------------------
  if(vaga1 < 10.0 && vaga1 > 0.6 && controlevg1 == 0){
     controlevg1 = 1;
     
  }
  if(vaga1 > 10.0 && controlevg1 == 1){
    Vagas();
    controlevg1 =0;
  }  

  // --------------- VAGA 2 -------------------------------------------------------
  if(vaga2 < 10.0 && vaga2 > 0.6 && controlevg2 == 0){
     controlevg2 = 1;
  }
  if(vaga2 > 10.0 && controlevg2 == 1){
    Vagas();
    controlevg2 = 0;
  }


  // ---------------- VAGA 3 ----------------------------------------------------- 
  if(vaga3 < 10.0 && vaga3 > 0.6 && controlevg3 == 0){
     controlevg3 = 1;
  }
  if(vaga3 > 10.0 && controlevg3 == 1){
    Vagas();
    controlevg3 = 0;
  }


  // ----------------- VAGA 4 ----------------------------------------------------
  if(vaga4 < 10.0 && vaga4 > 0.6 && controlevg4 == 0){
     controlevg4 = 1;
  }
  if(vaga4 > 10.0 && controlevg4 == 1){
    Vagas();
    controlevg4 = 0;
  } 

  
}


