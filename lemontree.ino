const int VAL_PROBE = 7; // Analog pin 08
const int FAN_PIN  = 3;
const int TEMP_PIN = A6;
const int PUMP_PIN = 2;

const int GOOD_MOISTURE = 70;
const int LOW_SOIL_MOISTURE = 35;
const int FAN_SPEED_MAX = 255;

String data = "";
boolean isLive = false;

int moisture = 0;
double remTemp = 0l; 
double roomTemp = 0l;
int remLux = 0;       // Sanfransisco light value
double windSpeed = 0;
int remSoilMoist = 0;
int isRaining = 0;
int fanSpeed=0;
float localTemp = 0;
int sfTime = 0; // sanfransisco local time in 24hrs
int sunsetTime = 0;

long  startTime =0;
boolean timeStart = false;
int showerSeconds = 5;
int waitSeconds = 20;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(FAN_PIN,OUTPUT);
  pinMode(VAL_PROBE,INPUT);
  //sampledata="test<RT>remote temp12.12</RT><LT>localtemp10.10</LT><RL>remoteLux123</RL>,<WS>windSpeed</WS>,<RSM>remote soil moist</RSM>,<LSM>local soilMoist</LSM><RR> remote rain0</RR><LR>local riN</LR><RMST>1</RMST><LMST></LMST>";
}

void loop() {
  // put your main code here, to run repeatedly:
    String incoming = ""; 
    moisture = analogRead(VAL_PROBE);
    Serial.print("Sensor:LocalSoilMoisture");Serial.print(moisture);Serial.println("------------------");
    int tempSensor = analogRead(TEMP_PIN);
    localTemp = readTemp(tempSensor);
    Serial.print("Sensor:LocalTemprature");Serial.print(tempSensor);Serial.println("------------------");

    /// read all local sensor values before this line 
    delay(2000);
    if(Serial.available() > 0 ){
      data = Serial.readString();
      readData(data);
      windSpeed = constrain(windSpeed,0,50);
      moisture = map(moisture,550,10,0,100);

  if(moisture < LOW_SOIL_MOISTURE ){
    if(shouldStartPump()){
     Serial.print(" plant says : water me !! ");
     digitalWrite(PUMP_PIN,HIGH); Serial.println("Started pump .. ");
    }
    //trigger relay to start pump 
    
     
  }
  else if(moisture >= GOOD_MOISTURE){
      // tigger relay to stop 
     digitalWrite(PUMP_PIN,LOW);
      Serial.print("Plant says : Stop Drowning me plss  !!!  ");
  }
//map(value, fromLow, fromHigh, toLow, toHigh) --- map(val, 0, 1023, 0, 255);
    
    fanSpeed = map(windSpeed,0,50,0,FAN_SPEED_MAX);
    Serial.print("Fan Speed  ");Serial.print(fanSpeed);   Serial.println("  ");
 
    analogWrite(FAN_PIN,fanSpeed);


    startLights();
   
}
}
boolean shouldStartPump(){
  boolean retStatus = false;
  // if raining 
  if(isRaining == 1){ // if its raining 
        if( !timeStart ){ // check if our shower duration counter has started
             startTime = millis();// start the timer 
             retStatus= true; 
          }
          else{    // if timer is already started 
                long endTime = millis(); // get end time to see how much time passed 
                long duration = endTime - startTime;
                long endSecs = duration/1000;
                if(endSecs >= showerSeconds){ // if current end time shows > than set shower secs 
                    retStatus= false;        // stop the pump we elapsed allowed shower secs
                }
                else if(endSecs >= waitSeconds ){// now if local shower is stopped but its still raining there we still wait till elapsed time to save home from flooding
                      startTime = millis(); // rset our base counter to current time  
                      timeStart = false; // reset falg to default 
                      retStatus =  true;
               }
            }
         return retStatus;   
    }

  
    
  }

void readData(String incoming){
//sampledata="test<RT>remote temp12.12</RT><LT>localtemp10.10</LT><RL>remoteLux123</RL>,<WS>windSpeed</WS>,<RSM>remote soil moist</RSM>,<LSM>local soilMoist</LSM><RR> remote rain0</RR><LR>local riN</LR><RMST>1</RMST><LMST></LMST><TM>14</TM>";

   if(incoming.startsWith("test")){
    int startPos = 0;
    int endPos = 0;
     String val = "";
    commonData( incoming);
      String str = "<LT>";
      startPos = (incoming.indexOf(str))+ str.length();
      endPos = incoming.indexOf("</LT>");
      val = incoming.substring(startPos,endPos);
      Serial.print("Override:LocalTemprature:");Serial.print(val);Serial.println("------------------");
      roomTemp = val.toDouble();

      str = "LSM";
      startPos = incoming.indexOf(str)+ str.length();
      endPos = incoming.indexOf("</LSM>");
      val = incoming.substring(startPos,endPos);   
      Serial.print("Override:LocalSoilMoisture:");Serial.print(val);Serial.println("------------------");
      moisture = val.toDouble();
      
    
                
      }
      else if(incoming.startsWith("live")){
        commonData( incoming);
      }
}

void commonData(String incoming){

     int startPos = 0;
    int endPos = 0;
    String str = "<RT>";
    String val = "";
       startPos = incoming.indexOf(str)+ str.length();
       endPos = incoming.indexOf("</RT>");
      val = incoming.substring(startPos,endPos);   
      Serial.print("RemoteTemp:");Serial.print(val);Serial.println("------------------");
      remTemp = val.toDouble();
      
      str = "<RL>";
      startPos = incoming.indexOf(str)+ str.length();
      endPos = incoming.indexOf("</RL>");
      val = incoming.substring(startPos,endPos);
      Serial.print("RemoteLightValue:");Serial.print(val);Serial.println("------------------");
      remLux = val.toInt();
      
      str = "<WS>";
      startPos = incoming.indexOf(str)+ str.length();
      endPos = incoming.indexOf("</WS>");
      val = incoming.substring(startPos,endPos);
      Serial.print("Windspeed:");Serial.print(val);Serial.println("------------------");
      windSpeed = val.toDouble();


       str = "<RSM>";
       startPos = incoming.indexOf(str)+ str.length();
      endPos = incoming.indexOf("</RSM>");
      val = incoming.substring(startPos,endPos);
      Serial.print("RemoteSoilMoisture:");Serial.print(val);Serial.println("------------------");
      remSoilMoist = val.toInt();

      str = "<RR>";
       startPos = incoming.indexOf(str)+ str.length();
      endPos = incoming.indexOf("</RR>");
      val = incoming.substring(startPos,endPos);
      Serial.print("RemoteRainStatus:");Serial.print(val);Serial.println("------------------");
      isRaining = val.toInt();

      str = "<TM>";
       startPos = incoming.indexOf(str)+ str.length();
      endPos = incoming.indexOf("</TM>");
      val = incoming.substring(startPos,endPos);
      Serial.print("Timein24Hrs:");Serial.print(val);Serial.println("------------------");
      sfTime = val.toInt();

      str = "<SS>";
      startPos = incoming.indexOf(str)+ str.length();
      endPos = incoming.indexOf("</SS>");
      val = incoming.substring(startPos,endPos);
      Serial.print("SunSetTimein24Hrs:");Serial.print(val);Serial.println("------------------");
      sunsetTime = val.toInt();
      
  }

void setFanSpeed(int fSpeed){
    
  }
float readTemp(int temp){
  float r1 = 1000;
  float logr2,r2,t;
  float c1 = 0.001129148, c2 = 0.000234125,c3 = 0.0000000876741;

  r2 = r1 * (1023.0/(float)temp - 1.0);
  logr2 = log(r2);
  t = (1.0/(c1+c2*logr2+c3*logr2*logr2*logr2));
  t = t - 273.15;
  //t = (t*9.0)/5.0 + 32.0;
  localTemp = t;
  
  }

 void  startLights(){
      if(sfTime < 12){
         //blue lights on
         Serial.print("Blue Lights on ...");Serial.println("------------------");
        }
      else if(sfTime >12){
          // whitelights on
          Serial.print("White Lights on ...");Serial.println("------------------");
          }
      else if(sfTime > sunsetTime){
          Serial.print("Red Lights on ...");Serial.println("------------------");
            }
    }
