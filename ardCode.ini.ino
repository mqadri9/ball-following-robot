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
 char str[2];
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
   if(Serial.available() >0) {
        digitalWrite(LED_BUILTIN, LOW);
        char data=Serial.read();
        str[0]=data;
        str[1]='\0';
        Serial.print(str);
        if(str[0]=='R'){
               //Serial.print("L");
               //Previously turn on axis
               digitalWrite(LA,HIGH) ;
                digitalWrite(LB,LOW) ;
                analogWrite(LPWM,50) ;
                 digitalWrite(RA,LOW) ;
               digitalWrite(RB,HIGH) ;
               analogWrite(RPWM,50) ;
               /*digitalWrite(LA,HIGH) ;
                digitalWrite(LB,LOW) ;
                analogWrite(LPWM,50) ;
                digitalWrite(RA,HIGH) ;
               digitalWrite(RB,LOW) ;
               analogWrite(RPWM,30);*/
          }
          else if (str[0]=='L'){
                  //  Serial.print("R");
               digitalWrite(LA,LOW) ;
                digitalWrite(LB,HIGH) ;
                analogWrite(LPWM,50) ;
                 digitalWrite(RA,HIGH) ;
               digitalWrite(RB,LOW) ;
               analogWrite(RPWM,50) ;  
          }
          else if (str[0]=='W'){
//                    Serial.print("W");

               digitalWrite(LA,LOW) ;
                digitalWrite(LB,HIGH) ;
                analogWrite(LPWM,0) ;
                 digitalWrite(RA,HIGH) ;
               digitalWrite(RB,LOW) ;
               analogWrite(RPWM,0) ;  
          }
          else if (str[0]=='S'){
              //      Serial.print("S");

               digitalWrite(LA,HIGH) ;
                digitalWrite(LB,LOW) ;
                analogWrite(LPWM,64) ;
                 digitalWrite(RA,HIGH) ;
               digitalWrite(RB,LOW) ;
               analogWrite(RPWM,64) ;  
          }
          else if (str[0]=='F'){
              //      Serial.print("F");

                if (leftMeas<20 || rightMeas<20)  {
                digitalWrite(LA,HIGH) ;
                  digitalWrite(LB,LOW) ;
                  analogWrite(LPWM,64) ;
                  digitalWrite(RA,LOW) ;
                  digitalWrite(RB,HIGH) ;
                  analogWrite(RPWM,64) ;
                 }
                 else {
                  digitalWrite(LA, HIGH) ;
                  digitalWrite(LB,LOW) ;
                  analogWrite(LPWM,64) ;
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


