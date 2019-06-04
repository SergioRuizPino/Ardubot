  /*Autores: Sergio Ruiz Pino y Jose Antonio Garcia velatta
 * 
 * 
 */

 
//Varible
bool dr,al,di,a0=true,a1=true,a2=true,a5=true,meta=false,moverte=false;//variables para identificar el cambio de casilla y meta
char cpc,obs,mov;//control user o automatico
const float resolucion = 0.00488;//resolucion analogico digital usado para pilas()
float voltage,lec,distancia,drec=0.0,suma=0.0,suma2=0.0,vel=0.0;//lec ,cm y suma para sharp y distancia ultrasonido por el robot
unsigned int  pasos=0;//cuenta celdas recorridas
unsigned long tb,tiempo_inicio,tiempo_ahora;//usado ultrasonido



//funciones
void funcontrol();
void mover();
void parar();
bool comprobarmeta();
bool compruebasharp1();
bool compruebasharp2();
bool compruebausonido();
void girarsharp1();
void girarsharp2();
void pilas();
void automatico();
void moveruser();
void enviardatos();
void vueltacasilla();
void aliniar();
 

  
//setup
void setup() 
  {
  Serial1.begin(9600);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(11,INPUT);
  }


//loop
void loop() 

{
  
  if(meta==false)//primero de forma autonoma sale del laberinto
  {
  delay(2000);
  automatico();
  }
  
if(Serial1.available()>0)//Luego solo espera instrucciones
 {
  cpc=Serial1.read();//lee si controla usuario o auto
  if(cpc=='1')
    {
      funcontrol();
    }
  if(cpc=='0')//recorre otro laberinto
    {
        meta=false;
        automatico();
    }
 }
 
 
}


void automatico()//algoritmo mano drch
{
  meta=comprobarmeta();
  while(!meta)
  {
    dr=compruebasharp1();
    al=compruebausonido();
    di=compruebasharp2();
    moverte=false;
    if(dr)
    { 
      girarsharp1(); 
      moverte=true;
      drec=drec+suma;
      mov='d';
      vel=suma/tiempo_ahora;
    }else{
      if(al){
        moverte=true;
        drec=drec+distancia;
        mov='b';
        vel=distancia/tiempo_ahora;
      }else{
        if(di){
        girarsharp2();
        moverte=true;
        drec=drec+suma2;
        mov='i';
        vel=suma2/tiempo_ahora;
        }}}

     vel=vel*100;
     if(moverte)
     {
      
      mover();
      }else{
        girarsharp2();
        girarsharp2();
      }
   meta=comprobarmeta(); 
   enviardatos();
   delay(2500);//despues de  parar de moverse espera para hacer la comprobacion de meta 
    }
}


void vueltacasilla()
{
  girarsharp2();
  girarsharp2();
  mover();
  bool dr,al,di;
    dr=compruebasharp1();
    al=compruebausonido();
    di=compruebasharp2();
    moverte=false;
    
    while(pasos!=0)
    {
    if(di)
    {
      girarsharp2(); 
      moverte=true;
    }else{
      if(al){
        moverte=true;
         }else{
        if(dr){
        girarsharp1();
        moverte=true;
        }}}
        
     if(moverte)
     {
      mover();
      }else{
        girarsharp2();
        girarsharp2();
      } 
   delay(500); 
   pasos=pasos-1;
   
}
}

void funcontrol()//control user
{
  bool con=true;
  char accion;
  while(con)
  {
    while(Serial1.available()<=0){}
    
    accion=Serial1.read();//igual a lo que envie serial
    switch(accion)
    {
      case '1':moveruser();break;
      case '2':girarsharp1();break;//gira drch
      case '3':girarsharp2();break;//gira izq
      case '4':con=false;break;
      case '5':vueltacasilla();break;
      default:break;
    }
  }
}


void moveruser()//mueve el usuario dutante 2 seg
{
  digitalWrite(5,HIGH); 
  digitalWrite(6,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH);
  delay(2000);
  parar();
}



void mover()//mueve robot hazta cambio casilla
{ 
  tiempo_inicio=millis();
  a1=a2=a2=a5=true;//activamos para siguiente movimiento
  digitalWrite(5,HIGH); 
  digitalWrite(6,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH); 

   while((a0 || a1 || a2 || a5 )== true)
  {
 
    if(analogRead(A5)>700){a5=false;}
    if(analogRead(A2)>700){a2=false;}
    if(analogRead(A1)>700){a1=false;}
    if(analogRead(A0)>700){a0=false;}
   }
      //tras cambio de celda parar
  delay(500);
  parar();
  tiempo_ahora=millis();
  tiempo_ahora-=tiempo_inicio;//tiempo usado para calcular velocidad
  pasos+=1;
}



void parar()//para robot
{
    digitalWrite(5,LOW); 
    digitalWrite(6,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW); 
}


bool comprobarmeta()//comprueba si ha llegado a la meta
{
    if(analogRead(A0)<700){return false;}
    if(analogRead(A1)<700){return false;}
    if(analogRead(A2)<700){return false;}
    if(analogRead(A5)<700){return false;}
    return true;
}

void girarsharp1()//girar 90 grados a la drch CONDICIONADO POR VOLTAJE PILAS
{  
  digitalWrite(5,HIGH); 
  digitalWrite(6,LOW);
  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);
  delay(525);
 parar();
}

void girarsharp2()//girar 90 grados a la izq CONDICIONADO POR VOLTAJE PILAS
{

  digitalWrite(5,LOW); 
  digitalWrite(6,HIGH);
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH);
  delay(525);
  parar();
  
}

bool compruebausonido()//comprueba si puede seguir robot de frente
{
 
  digitalWrite(8,LOW);
  delayMicroseconds(5);

  digitalWrite(8,HIGH);
  delayMicroseconds(10);
  digitalWrite(8,LOW);

  tb =pulseIn(11,HIGH);
  distancia =0.017*tb;
  if(distancia>=120)
  {//como ultrasonido falla si da valores muy alto imposibles dentro del laberinto poner valor medio
   digitalWrite(8,LOW);
   delayMicroseconds(5);

    digitalWrite(8,HIGH);
    delayMicroseconds(10);
    digitalWrite(8,LOW);

    tb =pulseIn(11,HIGH);
    distancia =0.017*tb;

  }
  if(distancia>20){
    distancia=20;
    return true;
  }else{
    return false;
}}

bool compruebasharp1()//comprueba si puede ir dir sharp1 para ser exacto toma la media de 100 medidas
{
  float cm=0;
  suma=0;
  for(int i=0;i<100;i++)
  {
    lec = analogRead(A3);
    cm = pow(3027.4 / lec, 1.2134);
    suma=suma+cm;
  }
    
  suma=suma/100;
  if(suma>20){
    suma=20;
    return true;
  }else{
    return false;
}}


bool compruebasharp2()//comprueba si puede ir dir sharp2
{
  float cm=0;
  suma2=0;
  for(int i=0;i<100;i++)
  {
  lec = analogRead(A4);
  cm = pow(3027.4 / lec, 1.2134);
  suma2=suma2+cm;
  }
  
  suma2=suma2/100;
  if(suma2>20){
    suma2=20;
    return true;
  }else{
    return false;
  } 
}

void pilas()//devuelve voltaje pilas
{
  //  Conectar al A6 la batería y mostrar por pantalla el voltaje de las pilas
  float battery = analogRead(A6);
  battery = battery * 0.0048;
  Serial1.print(battery);
}

void enviardatos()
{
  
    Serial1.println(pasos);//int
    Serial1.println(vel);//float
    Serial1.println(drec);//float
    Serial1.println(mov);
    if(!dr)
      Serial1.print('d');
     if(!al)
      Serial1.print('f');
    if(!di)
      Serial1.print('i');
    Serial1.println();
    Serial1.println(obs);//obstaculos
  
}


void isharp1()//comrpueba valor sharp 1
{
  float cm=0;
  for(int i=0;i<100;i++)
  {
    lec = analogRead(A3);//conectado sharp 1
    cm = pow(3027.4 / lec, 1.2134);
    suma=suma+cm;
  }
    
  suma=suma/100;
  Serial1.print("Distancia cm del sharp derecho ");
  Serial1.print(suma);
  Serial1.println();
  delay(1000);
  
}



void isharp2()//comprueba valor sharp 2
{

  float cm=0;
  for(int i=0;i<100;i++)
  {
    lec = analogRead(A4);//conectado sharp 2
    cm = pow(3027.4 / lec, 1.2134);
    suma=suma+cm;
  }
    
  suma=suma/100;
  Serial1.print("Distancia cm del sharp izquierdo ");
  Serial1.print(suma);
  Serial1.println();
  delay(1000);
  
  
}


void iultras()//comprueba valor ultrasonidp
{
  float sum=0;
  float dt;
  
//  for(int i=0;i<100;i++)//tomar 100 muestras de datos
 // {
     
  digitalWrite(8,LOW);
  delayMicroseconds(5);

  digitalWrite(8,HIGH);
  delayMicroseconds(10);
  digitalWrite(8,LOW);

  tb =pulseIn(11,HIGH);
  dt =0.017*tb;

  if(dt>=120)
  {
    digitalWrite(8,LOW);
    delayMicroseconds(5);

    digitalWrite(8,HIGH);
    delayMicroseconds(10);
    digitalWrite(8,LOW);

    tb =pulseIn(11,HIGH);
    dt =0.017*tb;
    
  }
  Serial1.print("Distancia cm del sensor de ultrasonido ");
  Serial1.print(dt);
  Serial1.println();
  delay(1000);
  
}







 /**
  * Rueda izquierda -> 5 y 6
  * Rueda derecha -> 9 y 10
  * CNY -> A0, A1, A2, A5
  * A0 ATRAS IZQ
  * A1 ATRAS DRCH
  * A2 ARRIBA DRCH
  * A5 ARRIBA IZQ
*/
  
