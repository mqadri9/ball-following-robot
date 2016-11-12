int ultraSoundSignalPins[] = {8,9}; // Front Left,Front, Front Right, Rear Ultrasound signal pins
char *pingString[] = {"Front Left ", "Front Right "}; // just something to print to indicate direction

#define LA 7
#define LB 5
#define LPWM 6

#define RA 2
#define RB 4
#define RPWM 3


void setup() {
  Serial.begin(9600);
  pinMode(LA, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(RA, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(RPWM, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

}
unsigned long ping(int index)
{
 unsigned long echo;

 pinMode(ultraSoundSignalPins[index], OUTPUT); // Switch signalpin to output
 digitalWrite(ultraSoundSignalPins[index], LOW); // Send low pulse
 delayMicroseconds(2); // Wait for 2 microseconds
 digitalWrite(ultraSoundSignalPins[index], HIGH); // Send high pulse
 delayMicroseconds(5); // Wait for 5 microseconds
 digitalWrite(ultraSoundSignalPins[index], LOW); // Holdoff
 pinMode(ultraSoundSignalPins[index], INPUT); // Switch signalpin to input
 digitalWrite(ultraSoundSignalPins[index], HIGH); // Turn on pullup resistor
 echo = pulseIn(ultraSoundSignalPins[index], HIGH); //Listen for echo
 return (echo / 58.138) * .39; //convert to CM then to inches
}

void loop()
{
 unsigned long leftMeas=0;
 unsigned long rightMeas=0;
 unsigned long ultrasoundValue;
 int pwmValLeft;
 int pwmValRight;
 char stro[5];
 char BaseSpeed[2];
 char offset[2];
 String inData;
 byte inByte;
 for(int i=0; i < 2; i++){
 ultrasoundValue = ping(i);
   if(i==0){
   leftMeas=ultrasoundValue;
   }
   else if(i==1) {
   rightMeas=ultrasoundValue;
   }
   //Serial.print(pingString[i]);
   //Serial.print(ultrasoundValue);
   //Serial.print("cm, ");    
   delay(50);
 }
   //Serial.println();
   while (!Serial.available()) {
    digitalWrite(LED_BUILTIN, HIGH);
    }
   if(Serial.available()>0) {
     char temp[2];
        for(int k=0; k<5;k++){
          char data=Serial.read();
          stro[k]=data;
          if(k==5){
            stro[5]='\0';
          }
        }
       Serial.println(stro);
        offset[0]=stro[1];
        offset[1]=stro[2];
        BaseSpeed[0]=stro[3];
        BaseSpeed[1]=stro[4];
        if(stro[0]=='L'){
               pwmValLeft=atoi(offset)+atoi(BaseSpeed);
               pwmValRight=atoi(BaseSpeed)-atoi(offset);
               if (pwmValLeft <0 or pwmValLeft > 100 or pwmValRight<0 or pwmValRight > 100){
                  pwmValLeft=50;
                  pwmValRight=50;
               }
               digitalWrite(LA,HIGH) ;
               digitalWrite(LB,LOW) ;
               analogWrite(LPWM,pwmValLeft-5) ;
               digitalWrite(RA,HIGH) ;
               digitalWrite(RB,LOW) ;
               analogWrite(RPWM,pwmValRight+10) ;

          }
          else if (stro[0]=='R'){
               pwmValRight=atoi(offset)+atoi(BaseSpeed);
               pwmValLeft=atoi(BaseSpeed)-atoi(offset);
               if (pwmValLeft <0 or pwmValLeft > 100 or pwmValRight<0 or pwmValRight > 100){
                  pwmValLeft=50;
                  pwmValRight=50;
               }
               digitalWrite(LA,HIGH) ;
               digitalWrite(LB,LOW) ;
               analogWrite(LPWM,pwmValLeft-5) ;
               digitalWrite(RA,HIGH) ;
               digitalWrite(RB,LOW) ;
               analogWrite(RPWM,pwmValRight) ;  
          }
          else if (stro[0]=='W'){
               digitalWrite(LA,LOW) ;
               digitalWrite(LB,HIGH) ;
               analogWrite(LPWM,0) ;
               digitalWrite(RA,HIGH) ;
               digitalWrite(RB,LOW) ;
               analogWrite(RPWM,0) ;  
          }
          else if (stro[0]=='S'){
               pwmValRight=atoi(BaseSpeed);
               pwmValLeft=atoi(BaseSpeed);
               if (pwmValLeft <0 or pwmValLeft > 100 or pwmValRight<0 or pwmValRight > 100) {
                  pwmValLeft=50;
                  pwmValRight=50;
               }
               digitalWrite(LA,HIGH) ;
               digitalWrite(LB,LOW) ;
               analogWrite(LPWM,pwmValLeft) ;
               digitalWrite(RA,HIGH) ;
               digitalWrite(RB,LOW) ;
               analogWrite(RPWM,pwmValRight+10) ;  
          }
          else if (stro[0]=='F'){
                if (leftMeas<20 || rightMeas<20)  {
                digitalWrite(LA,HIGH) ;
                  digitalWrite(LB,LOW) ;
                  analogWrite(LPWM,50) ;
                  digitalWrite(RA,LOW) ;
                  digitalWrite(RB,HIGH) ;
                  analogWrite(RPWM,50) ;
                 }
                 else {
                  digitalWrite(LA, HIGH) ;
                  digitalWrite(LB,LOW) ;
                  analogWrite(LPWM,60) ;
                  digitalWrite(RA,HIGH) ;
                  digitalWrite(RB,LOW) ;
                  analogWrite(RPWM,50) ;
                 }
        }
        else{
                  digitalWrite(LA,HIGH) ;
                  digitalWrite(LB,LOW) ;
                  analogWrite(LPWM,0) ;
                  digitalWrite(RA,LOW) ;
                  digitalWrite(RB,HIGH) ;
                  analogWrite(RPWM,0) ;
        }
}
}