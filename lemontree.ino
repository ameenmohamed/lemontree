const int VAL_PROBE = 7; // Analog pin 08
const int FAN_PIN  = 3;
const int TEMP_PIN = A6;
const int PUMP_PIN = 2;

const int GOOD_MOISTURE = 70;
const int LOW_SOIL_MOISTURE = 35;
const int FAN_SPEED_MAX = 255;
const String NO_VALUE = "NOVAL";

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
  //  Serial.print("Sensor:LocalSoilMoisture");Serial.print(moisture);Serial.println("------------------");
    int tempSensor = analogRead(TEMP_PIN);
    localTemp = readTemp(tempSensor);
 //   Serial.print("Sensor:LocalTemprature");Serial.print(tempSensor);Serial.println("------------------");

    /// read all local sensor values before this line 
    delay(2000);
    if(Serial.available() > 0 ){
      data = Serial.readString();
      readData();
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

void readData(){
//sampledata="test<RT>remote temp12.12</RT><LT>localtemp10.10</LT><RL>remoteLux123</RL>,<WS>windSpeed</WS>,<RSM>remote soil moist</RSM>,<LSM>local soilMoist</LSM><RR> remote rain0</RR><LR>local riN</LR><RMST>1</RMST><LMST></LMST><TM>14</TM>";

   if(data.startsWith("test")){
    int startPos = 0;
    int endPos = 0;
     String val = "";
      parseCommonData();

       val = parseValue("<LT>","</LT>"); 
       if(!val.equalsIgnoreCase(NO_VALUE)){
           roomTemp = val.toDouble();
       }
       Serial.print("Override:LocalTemprature:");Serial.print(val);Serial.print("-----CurrentVal:");Serial.print(roomTemp);Serial.println("");
 

      val = parseValue("<LSM>","</LSM>"); 
       if(!val.equalsIgnoreCase(NO_VALUE)){
        moisture = val.toDouble();
       }
       Serial.print("Override:LocalSoilMoisture:");Serial.print(val);Serial.print("-----CurrentVal:");Serial.print(moisture);Serial.println("");
    }
     else if(data.startsWith("live")){
        parseCommonData();
      }
}

void parseCommonData(){

    String val = "";
    Serial.println(data);

       val = parseValue("<RT>","</RT>");      
      if(!val.equalsIgnoreCase(NO_VALUE)){
           remTemp = val.toDouble();
      }
      Serial.print("RemoteTemp:");Serial.print(val);Serial.print("-----CurrentVal:");Serial.print(remTemp);Serial.println("");

       val = parseValue("<RL>","</RL>");
       if(!val.equalsIgnoreCase(NO_VALUE)){
            remLux = val.toInt();
       }
       Serial.print("RemoteLightValue:");Serial.print(val);Serial.print("-----CurrentVal:");Serial.print(remLux);Serial.println("");
      
      val = parseValue("<WS>","</WS>");
       if(!val.equalsIgnoreCase(NO_VALUE)){
          windSpeed = val.toDouble();
       }
       Serial.print("Windspeed:");Serial.print(val);Serial.print("-----CurrentVal:");Serial.print(windSpeed);Serial.println("");


      val = parseValue("<RSM>","</RSM>");
       if(!val.equalsIgnoreCase(NO_VALUE)){
           remSoilMoist = val.toInt();
       }
       Serial.print("RemoteSoilMoisture:");Serial.print(val);Serial.print("-----CurrentVal:");Serial.print(remSoilMoist);Serial.println("");


      val = parseValue("<RR>","</RR>");
       if(!val.equalsIgnoreCase(NO_VALUE)){
            isRaining = val.toInt();
       }
       Serial.print("RemoteRainStatus:");Serial.print(val);Serial.print("-----CurrentVal:");Serial.print(isRaining);Serial.println("");

      
      val = parseValue("<TM>","</TM>");
      if(!val.equalsIgnoreCase(NO_VALUE)){
        sfTime = val.toInt();
      }
      Serial.print("Timein24Hrs:");Serial.print(val);Serial.print("-----CurrentVal:");Serial.print(sfTime);Serial.println("");

       val = parseValue("<SS>","</SS>");  
       if(!val.equalsIgnoreCase(NO_VALUE)){
           sunsetTime = val.toInt();
       }
       Serial.print("SunSetTimein24Hrs:");Serial.print(val);Serial.print("-----CurrentVal:");Serial.print(sunsetTime);Serial.println("");
      
  }

String parseValue(String startStr,String endStr){
  
   int startPos = 0;
    int endPos = 0;
    String retVal = "";
    int baseChk = data.indexOf(startStr);
    int endChk = data.indexOf(endStr);
    if(baseChk > 0 && endChk > 0){
        startPos = baseChk + startStr.length();
        endPos = endChk;
        retVal = data.substring(startPos,endPos);
    }
    else {
        retVal = NO_VALUE;
      }
    return retVal;
  
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
         Serial.print("Blue Lights on ...");
        }
      else if(sfTime >12){
          // whitelights on
          Serial.print("White Lights on ...");
          }
      else if(sfTime > sunsetTime){
          Serial.print("Red Lights on ...");
            }
    }
