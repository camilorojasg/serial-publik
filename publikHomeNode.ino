int clk = 2;
int dato = 3;
int f1 = 6;
int f2 = 7;
int f3 = 8;
int f4 = 9;
int f5 = 10;
int f6 = 11;
int f7 = 12;
String texto="";
int tamano=0;
byte textoArr[740];
int desplazamiento = 0;

boolean recMsj = false;
boolean continuar = false;
boolean recTam = false;
boolean recRep = false;

char tamStr[4];
char repMsj[4];

String cadena = "";
int n = 0;
int m = -1;
int r = 0;
int repeticiones = 0;

byte alfabeto[][6] = {
  {0,0,0,0,0,0},
  {62,127,77,89,127,62},
  {17,49,127,127,1,1},
  {35,103,77,73,121,49},
  {34,99,73,73,127,54},
  {12,28,52,127,127,4},
  {114,115,81,81,95,78},
  {30,63,105,73,79,6},
  {64,71,79,88,112,96},
  {54,127,73,73,127,54},
  {48,121,73,75,126,60},
  {63,127,68,68,127,63},
  {127,127,73,73,127,54},
  {62,127,65,65,99,34},
  {127,127,65,99,62,28},
  {127,127,73,73,73,65},
  {127,127,72,72,72,64},
  {62,127,73,73,111,47},
  {127,127,8,8,127,127},
  {0,65,127,127,65,0},
  {6,7,65,127,126,64},
  {127,127,28,54,99,65},
  {127,127,1,1,1,1},
  {127,48,28,28,48,127},
  {127,127,24,12,127,127},
  {62,127,65,65,127,62},
  {127,127,72,72,120,48},
  {60,126,70,70,127,61},
  {127,127,76,78,123,49},
  {50,123,73,73,111,38},
  {64,64,127,127,64,64},
  {126,127,1,1,127,126},
  {124,126,3,3,126,124},
  {127,6,28,28,6,127},
  {99,119,28,28,119,99},
  {112,120,15,15,120,112},
  {67,71,77,89,113,97},
  {0,0,3,3,0,0}, //Punto
  {0,1,7,6,0,0}, //Coma
  {62,65,125,101,61,2} //Arroba
};

void setup(){
  Serial.begin(9600);
  cadena.reserve(7);
  texto.reserve(100);
  pinMode(clk,OUTPUT);
  pinMode(dato,OUTPUT);
  pinMode(f1,OUTPUT);
  pinMode(f2,OUTPUT);
  pinMode(f3,OUTPUT);
  pinMode(f4,OUTPUT);
  pinMode(f5,OUTPUT);
  pinMode(f6,OUTPUT);
  pinMode(f7,OUTPUT);
}

byte textoComp(){
  int i,j,ult;
  for(i = 0; i<20; i++){
    textoArr[i] = 0;
  }
  for(i = 0; i<tamano; i++){
    if(texto[i]>47&&texto[i]<58){
      for(j = 0; j<6; j++){
        textoArr[i*7+j+20] = alfabeto[texto[i]-47][j];
      }
    }else if(texto[i]>64&&texto[i]<91){
      for(j = 0; j<6; j++){
        textoArr[i*7+j+20] = alfabeto[texto[i]-54][j];
      }
    }else if(texto[i]==46){
      for(j = 0; j<6; j++){
        textoArr[i*7+j+20] = alfabeto[37][j];
      }
    }else if(texto[i]==64){
      for(j = 0; j<6; j++){
        textoArr[i*7+j+20] = alfabeto[39][j];
      }
    }else if(texto[i]==44){
      for(j = 0; j<6; j++){
        textoArr[i*7+j+20] = alfabeto[38][j];
      }
    }else{
      for(j = 0; j<6; j++){
        textoArr[i*7+j+20] = alfabeto[0][j];
      }
    }
    textoArr[i*7+26] = 0;
  }
  ult = (i-1)*7+26;
  for(i = ult; i<ult+20; i++){
    textoArr[i] = 0;
  }
}

void escribirLinea(int valor, int num){
  digitalWrite(clk,LOW);
  digitalWrite(dato,valor);
  digitalWrite(clk,HIGH);
  digitalWrite(clk,LOW);
  digitalWrite(f1,1&num);
  digitalWrite(f2,2&num);
  digitalWrite(f3,4&num);
  digitalWrite(f4,8&num);
  digitalWrite(f5,16&num);
  digitalWrite(f6,32&num);
  digitalWrite(f7,64&num);
  delayMicroseconds(700);
}

void escribirDisplay(){
  int registroDesp = 0;
  int i;
  int j = 0;
  while(j<50){
  for(i = 0; i<20; i++){
    escribirLinea(registroDesp,textoArr[i+desplazamiento]);
    registroDesp = 1;
    j++;
  }
  registroDesp = 0;
  }
}

void publik(int desp){
  int i = 0;
  while(i < desp){
    escribirDisplay();
    if(desplazamiento > tamano*7+20){
      desplazamiento = 0;
      i++;
    }else{
      desplazamiento+=1;
    }
  }
}

void loop(){
    //publik(10); //Numero de desplazamientos.
}

void serialEvent(){
  while(Serial.available()) {
    char nChar = (char)Serial.read();
    
    if(continuar) cadena += nChar;
    
    if(nChar==36){
      cadena = "";
      cadena += nChar;
      continuar = true;
    }
    
    if(recRep){
      if(r<=2){
        repMsj[r] = nChar;
        r += 1;
      }else{
        repMsj[3] = 0;
        repeticiones = atoi(repMsj);
        r = 0;
        n = 0;
        m = -1;
        recMsj = false;
        continuar = false;
        recTam = false;
        recRep = false;
        cadena = "";
        textoComp();
        publik(repeticiones);
      }
    }
    
    if(recMsj){
      if(m == tamano){
        //Serial.println("Mensaje recibido.");
        recMsj = false;
        recRep = true;
        m = -1;
      }else{
        if(m>=0) texto += nChar;
        m += 1;
      }
    }

    if(recTam){
      if(n<=2){
        tamStr[n] = nChar;
      }
      if(n==2){
        recTam = false;
        recMsj = true;
        tamStr[3] = 0;
        tamano = atoi(tamStr);
        n = 0;
      //Serial.println("Esperando mensaje de ");
      //Serial.print(tamano);
      }
      n += 1;
    }
    
    if(cadena == "$PBK,1,"){
      texto = "";
      recTam = true;
      cadena = "";
      continuar = false;
      //Serial.println("Esperando tamaño.");
    }
  }
}
