#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
/////////////////////////////////
#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
/////////////////////////////////
#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
//////////////////////////////////
const int limitx = 47;
const int limity =45;
const int limitz =32;
//////////////////////////////////
const float ext_height= 150;
const float axial_rods=220;
const float R = 135 ;
const float one_rodx = R * cos (90*0.0174532)  ;
const float one_rody =R * sin (90*0.0174532);
const float two_rodx=R * cos (210*0.0174532) ;
const float two_rody =R * sin (210*0.0174532);
const float three_rodx =R * cos (330*0.0174532);
const float three_rody =R * sin (330*0.0174532);
const float pheight =660;//printer height
const float extruder_width=35;
//////////////////////////////////
#include <SPI.h>
#include <SD.h>
#include<math.h>
/////////////////////////////////
File myFile;
int j,i =0;
int line [100];
float d1,d2,d3,xmov,ymov,zmov,xmov1,ymov1,zmov1,xout,yout,zout,Xnew,Ynew,Znew,xpast,ypast,zpast,DELAY;
/////////////////////////////////

void setup ()
{
  pinMode(limitx, INPUT);
  pinMode(limity, INPUT);
  pinMode(limitz, INPUT);
  pinMode(X_STEP_PIN  , OUTPUT);
  pinMode(X_DIR_PIN    , OUTPUT);
  pinMode(X_ENABLE_PIN    , OUTPUT);
  pinMode(Y_STEP_PIN  , OUTPUT);
  pinMode(Y_DIR_PIN    , OUTPUT);
  pinMode(Y_ENABLE_PIN    , OUTPUT);
  pinMode(Z_STEP_PIN  , OUTPUT);
  pinMode(Z_DIR_PIN    , OUTPUT); 
  pinMode(Z_ENABLE_PIN    , OUTPUT); 

  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);
  digitalWrite(Z_ENABLE_PIN, LOW);

////////////////////////////////////////////////////////
Serial.begin(9600); 
calibrate();
Serial.println("              Calibration done ....         ");
while (!Serial) {;}
Serial.println("              Initializing SD card...         ");
pinMode(53, OUTPUT); 
if (!SD.begin(53)) {
Serial.println("              initialization failed!");
return;
}
Serial.println("initialization done.");
myFile = SD.open("test.txt", FILE_READ);
while (myFile.available()) 
{ 
j=myFile.read();   
line [i]=j;
i++; 
if(j==10)
{
 if ((line[0]==71 && line[1]==49)||(line[0]==71 && line[1]==48))//g1
 {
values(line,i);/////////
i=0;
}

//delayMicroseconds  (10);
xyz2delta();

Serial.println(" ");
Serial.print(" x ");Serial.print(xout);
Serial.print(" y ");Serial.print(yout);
Serial.print(" z ");Serial.println(zout);

motorrun(xout*100, yout*100, zout*100);
i=0;
}}
myFile.close();
}

void loop() 
{}

//////////////////////////////////////////////////////////
void calibrate ()
{
  ////////////////////////////////////////////////////
   DELAY=30;
  //////////////////////////////////////////////////// 
int stopendx,stopendy,stopendz ;
stopendx= digitalRead(limitx);
stopendy=digitalRead(limity);
stopendz= digitalRead(limitz);
 while(stopendx==1 || stopendy==1 || stopendz==1)///nc-vcc  no-gnd
{
stopendx= digitalRead(limitx);
stopendy=digitalRead(limity);
stopendz= digitalRead(limitz);

if(stopendx==1)
motorx(HIGH, DELAY);
if(stopendy==1)
motory(HIGH, DELAY);
if(stopendz==1)
motorz(HIGH, DELAY);
//delayMicroseconds (100);
}
}
/////////////////////////////////////////////////////////////
void motorx (int direction_x,float DELAY)
{
digitalWrite(X_DIR_PIN,direction_x );   //direction
digitalWrite(X_STEP_PIN, LOW); //step
delayMicroseconds(DELAY);
digitalWrite(X_STEP_PIN,HIGH);  //step
//Serial.print("x");
}


////////////////////////////////////////////////////////////
void motory (int direction_y,float DELAY)
{ 
digitalWrite(Y_DIR_PIN, direction_y );    //direction
digitalWrite(Y_STEP_PIN, LOW); //step
delayMicroseconds ( DELAY);
digitalWrite(Y_STEP_PIN,HIGH);  //step
//Serial.print("y");
}
/////////////////////////////////////////////////////
void motorz (int direction_z,int DELAY)
{
digitalWrite(Z_DIR_PIN, direction_z );    //direction
digitalWrite(Z_STEP_PIN, LOW); //step
delayMicroseconds ( DELAY);
digitalWrite(Z_STEP_PIN,HIGH);  //step
//Serial.print("z");
}
//////////////////////////////////////////////////////////
void values(int line [],int i )
 {
 int X[6],Y[6],Z[6];
int z1=0;
 Xnew=0,Ynew=0,Znew=0;
 for(int k = 0; k<=i;k++)
{
if (line[k]==88)
{
 for(int l=k+1;line[l]!=32 && line[l]!=13;l++)
 {
 X[z1]=line[l]-48;
 z1++;
 }}}
 if(z1!=0)
 Xnew=modify(X,z1);
 //Serial.println(Xnew);
 int z2=0;
 for(int k = 0; k<=i;k++)
{
if (line[k]==89)
{
 for(int l=k+1;line[l]!=32 && line[l]!=13;l++)
 {
 Y[z2]=line[l]-48;
 z2++;
 }}}
 if(z2!=0)
 Ynew=modify(Y,z2);
 //Serial.println(Ynew);
  int z3=0;
 for(int k = 0; k<=i;k++)
{
if (line[k]==90)
{
 for(int l=k+1;line[l]!=32 && line[l]!=13;l++)
 {
 Z[z3]=line[l]-48;
 z3++;
 }}}
 if(z3!=0)
 Znew=modify(Z,z3);
// Serial.println(Znew);
 }
/////////////////////////////////////////////////////////////
float  modify(int x[],int z1)
{
  int point,k;
  double floatvalue=0;
  if(x[0]!=-3)
  {

  for(int i1=0;i1<z1;i1++)
  {
    if(x[i1]==-2)
    {
      point=i1;
    }
  }
  k=1;
  for(int j=0;j<z1;j++)
  {
  if(j>point)
  {
    floatvalue=floatvalue+(x[j]*(pow(0.1,k)));
    k++;
  }  
  }
 
  k=0;
   for(int j=z1;j>=0;j--)
  {
  if(j<point)
  {
    floatvalue=floatvalue+(x[j]*(pow(10,k)));
    k++;
  }  
  }
  }
  else if(x[0]==-3)
  {
  for(int i1=0;i1<z1;i1++)
  {
    if(x[i1]==-2)
    {
      point=i1;
    }
  }
  k=1;
  for(int j=0;j<z1;j++)
  {
  if(j>point)
  {
    floatvalue=floatvalue+(x[j]*(pow(0.1,k)));
    k++;
  }  
   }
   k=0;
    for(int j=z1;j>=1;j--)
  {
  if(j<point)
  {  
    floatvalue=floatvalue+(x[j]*(pow(10,k)));
    k++;
  }  

  }
    floatvalue=-floatvalue;
  }
  return(floatvalue);
}
//////////////////////////////////////////////////////
void xyz2delta()
{
 if (Znew==0&&Xnew==0&&Ynew==0)
      {xout=0;yout=0;zout=0;}
else if (Znew!=0&&Xnew==0&&Ynew==0)
   zdelta(Znew);
else 
   xydelta(Xnew,Ynew);
}
///////////////////////////////////////////////////////
  void xydelta(float Xnew,float Ynew)
  {
d1=distance(one_rodx,one_rody,Xnew,Ynew);
d2=distance(two_rodx,two_rody,Xnew,Ynew);
d3=distance(three_rodx,three_rody,Xnew,Ynew);
xmov=pow((pow(axial_rods,2)-pow(d1,2)),0.5);
ymov=pow((pow(axial_rods,2)-pow(d2,2)),0.5);
zmov=pow((pow(axial_rods,2)-pow(d3,2)),0.5);
machineconvertion(xmov,ymov,zmov);
} 
/////////////////////////////////////////////////////////
float distance (float a,float b,float c,float d)
{
return (pow(pow(a-c,2)+pow(b-d,2),0.5));
}
void zdelta(float Znew)
{
xmov=Znew;
ymov=Znew;
zmov=Znew;
machineconvertion(xmov,ymov,zmov);
}
////////////////////////////////////////////////////////////
float machineconvertion (float xmov,float ymov,float zmov)
{
  xmov1=pheight-xmov-ext_height;
  xout=xmov1-xpast;
  xpast=xmov1;
  ymov1=pheight-ymov-ext_height;
  yout=ymov1-ypast;       // convestion to meet the past values 
  ypast=ymov1;
  zmov1=pheight-zmov-ext_height;
  zout=zmov1-zpast;
  zpast=zmov1;
}
///////////////////////////////////////////////////////////////
void  motorrun( float x ,float y, float z)
{
 int direction_x=0,direction_y=0,direction_z=0;
 if(x>0)
direction_x=LOW;//clockwise
else if(x<0) 
{
x=-1*x;
direction_x=HIGH;//anti clockwise
}
 if(y>0)
direction_y=LOW;//clockwise
else if(y<0) 
{
y=-1*y;
direction_y=HIGH;//anti clockwise
}
if(z>0)
direction_z=LOW;//clockwise
else if(z<0) 
{
z=-1*z;
direction_z=HIGH;//anti clockwise
}
////////////////////////////////////////settings for motion 
/*
x=100*x;
y=100*y;//1 rotation = 3175 steps / 3.175 cm 
z=100*z;//mm to steps convertion 
*/
long maxvalue=0,x_=0,y_=0,z_=0;
int x_1=0,y_1=0,z_1=0;
maxvalue=max(max(x,y),z);
//Serial.println(x);
//Serial.print(y);
//Serial.print(z);
//Serial.print(maxvalue);
if (x==0)
x_=0;
else x_=1/(1-(x/maxvalue));
if (y==0)
y_=0;
else y_=1/(1-(y/maxvalue));
if (z==0)
z_=0;
else z_=1/(1-(z/maxvalue));
/////////////////////////////////////////////////////////////
 if ((line[0]==71 && line[1]==49))
 DELAY=100;//g1delay
 else 
  DELAY=75;//g2delay
 ////////////////////////////////////////////////////////////
for (float i=0;i<=maxvalue;i++)

{
if(i!=x_1)
{
motorx(direction_x, DELAY);
}
else {
//Serial.println("1");
x_1=x_1+x_;}
if(i!=y_1)
{
motory(direction_y, DELAY);

}
else 
{//Serial.println("2");
y_1=y_1+y_;}
if(i!=z_1)
{
motorz(direction_z, DELAY);
}
else 
{
//Serial.println("3");
z_1=z_1+z_;
}


//Serial.println(x_1);
//Serial.println(y_1);
//Serial.println(z_1);
}

}


