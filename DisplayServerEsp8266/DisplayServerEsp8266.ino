/*
Abhishek Tiwari
Display server run on ESP8266 device. Wifi Access Point connect via Mobile or Web Browser
Just a little test message.  Go to http://192.168.4.1 in a web browser
*/
#include <ESP8266WiFi.h> //wifi base library
#include <WiFiClient.h>  //wifi base library
#include <ESP8266WebServer.h> //wifi base library
#include "lcd.h" //#include "image.h" //Display library

//-----------Defines and Macros----------------//
//-------gpio5: display vcc gpio4: backlight vcc------//
#define gpio5 5 //power
#define gpio4 4 //backlight
#define LCD_BACKLIGHT_ON()      {GPOS = ((uint32_t)1 << gpio4); }//digitalWrite(gpio5,HIGH); }
#define LCD_BACKLIGHT_OFF()     {GPOC = ((uint32_t)1 << gpio4); }//digitalWrite(gpio5,LOW);  }
#define LCD_POWER_ON()          {GPOS = ((uint32_t)1 << gpio5); }//digitalWrite(gpio4,HIGH); }
#define LCD_POWER_OFF()         {GPOC = ((uint32_t)1 << gpio5); }//digitalWrite(gpio4,LOW);  }

//-----------------Dsiplay Gpio-----------------------//
const int _CS         = 15;          
const int _RESET      = 16;       
const int SCLK        = 14;  
const int SID         = 13; //Mosi
//------------------Wifi Details----------------------//
#define APSSID "AstkDisplay" //ssid wifi name show on scanning result
#define APPSK  "itsabhishek" //password of wifi
#define secretMessage "I Too Love You ji" //Secret Message to Shown on Display and WEB

//--------Display class and Wifi server class----------//
Nokia105 display( SID,  SCLK, _RESET, _CS);
ESP8266WebServer server(80);

//-----Varaibles and flags-----------------------------//
const char *ssid                            = APSSID; //ssid
const char *password                        = APPSK;  //password
unsigned char flagDatareceivedFromtheMobile = 0;      //0: no data, 1: yes data, print all
char bufferTemp[800]; //temp buffer to display 
unsigned int loopindex                      = 0;
unsigned long previousMillis                = 0;     // previous time
const long sleepIntervalNoActivity          = 30000; // after this much millisecond device will sleep!
const long sleepIntervalSecretMessage       = 5000; // after this much millisecond device will sleep!
char flagPutDeviceSleepNoActivity           = 0;     //0: mean current start time, 1 means restart time, flag to start timer to put device in sleep! after disconnection!
char flagPutDeviceSleepInstant              = 2;     //0: mean nosleep, 1 means instant sleep, 2: THIRD state to not to go instant sleep
char flagRunOnce                            = 0;     //run once flag
//-------copy paste html page that you want to show on webserver---------------//
const char *html_page = R"(<!DOCTYPE HTML><html>
<head>
  <title>ESP-IDF Programmable Power Supply Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #4B1D3F; color: white; font-size: 1.7rem; }
    .content { padding: 70px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading {  font-size: 2.8rem;}
    .head   {  font-family: "Inconsolata"; font-size:1.9rem;}
    .card.voltage { color: #0e7c7b; }
    .card.raw_adc { color: #d62246; }
    .card.current { color: #3fca6b; }
    .card.update_me { color: #d62246; }
    .input{width: 200px;margin:auto;height:20px;display:block;}
    .submitbtn{margin-top:10px;width:100px;height:25px;background-color:green;color:#fff;border-radius:7px;border:none;}
  </style>
</head>
<body>
  <div class="topnav">
    <h3><i class="fas fa-solid fa-meteor"></i>Display Web Server</h3>
  </div>
  <div class="content">
    <div class="cards">
         <div class="card voltage">
         <h4><i class = "head"> Write Any Text </i><i class="fas fa-solid fa-mask"></i></h4> <form action="/"> <input type="text" name="userInput" placeholder="Display...">
         <p> <input type="submit" value="Submit"  class = "submitbtn" </p>
          </form><br>
      </div>
    </div>
  </div>
    <div class="content">
    <div class="cards">
     <div class="card raw_adc">
        <h4><i class=" fas fa-sharp fa-pen"></i> Powered By Abhishek Tiwari</h4><p></p>
      </div>
    </div>
  </div>
</body>
</html>
)";

//----secret page--------------------------//
const char *htmlSecretpage = R"(<!DOCTYPE html>
<html>
<head>
<style>
h1 {
 color:#40B3DF;
 letter-spacing:12px;
 font-size:50px;
 position:relative;
 left:25px;
 top:30px;
}
p  {
  color: red;
  font-family: courier;
  font-size: 50px;
  background-color:#B4009E;
  color:#ffffff;
}
</style>
</head>
<body>
<h1 style="text-align:center;" > Welcome To Secret Page!</h1> 
<p  style="text-align:center;"> I too <span style="font-size:500%;color:red;">&hearts;</span> you</p>
</body>
</html>
)";

//-----define function Prototype------------//
void callbackWebHandle();
void sleepHandle(unsigned long setInterval);
void sleepInstantHandle();

//------------Setup run once all init happned here--------------------//
void setup() {
pinMode(gpio4, OUTPUT);                                      //BackLight control via gpio
WiFi.softAP(ssid, password);                                 //You can remove the password parameter if you want the AP to be open.
IPAddress myIP = WiFi.softAPIP();                            //Serial.print("AP IP address: "); //Serial.println(myIP);
server.on("/", HTTP_GET, callbackWebHandle);                 //attact callback handler her for webpage
server.on("/", HTTP_POST, callbackWebHandle);
server.begin();                                               //Serial.println("HTTP server started");
//------Init thhe Display Module------------//
LCD_BACKLIGHT_ON();                                          //backlight on the display //Serial.begin(115200); //Serial.println(); //Serial.print("Configuring access point...");
display.initDisplay();
display.setDrawPosition(128,160);                             //ROW(space)--COLUMN(height)
display.printString((char *)"Find me?",20,10,PURPLE,YELLOW);  //charactare, horizontal position
display.printString((char *) APSSID,20,30,PURPLE,YELLOW);     //charactare, horizontal position
display.printString((char *)"Pass:",20,60,PURPLE,YELLOW);     //charactare, horizontal position
display.printString((char *)APPSK,20,80,PURPLE,YELLOW);      //charactare, horizontal position //display.backgroundColor(BLUE); //display.displayClear();
display.printString((char *)"http://",20,120,RED,BLACK);      //charactare, horizontal position //display.backgroundColor(BLUE); //display.displayClear();
display.printString((char *)"192.168.4.1",20,140,RED,BLACK);      //charactare, horizontal position //display.backgroundColor(BLUE); //display.displayClear();
previousMillis = millis(); //time start now! for deep seep when no activity
}
//--------------------loop like while(1)-----------------------------------------//
void loop() {
  server.handleClient();                              //set continous webpage handler to active listen
  if (flagDatareceivedFromtheMobile == 1 ) {          //if yes: data received from web server
    flagDatareceivedFromtheMobile = 0;
    display.displayClear();                             //reset display buffer
    display.printString(bufferTemp,0,2,BLACK,YELLOW); //charactare, horizontal position
    memset(bufferTemp,0x00,800);                        //clear display buffer
  } else if (flagDatareceivedFromtheMobile == 2 ) {     //if secret message recceived from webserver!
    flagDatareceivedFromtheMobile = 0;                  //reset flag
    flagPutDeviceSleepInstant = 1;                      //instant sleep after print message!
    display.displayClear();                             //reset display buffer
    display.printString( (char*)secretMessage,0,2,RED,YELLOW);  //charactare, horizontal position
    memset(bufferTemp,0x00,800);                        //reset display buffer
  }
  //-----run once when anything happend then start time to read!-----//
  if ( flagPutDeviceSleepNoActivity == 1 || flagPutDeviceSleepInstant == 1) { //1 means device done with display
    flagPutDeviceSleepNoActivity = 0;
    if ( flagPutDeviceSleepInstant == 1) {
      flagPutDeviceSleepNoActivity = 2; //set to third state, no state condition
    }
    flagPutDeviceSleepInstant    = 0;
    previousMillis = millis(); //time start now!
  }
  //-------check flAG of sleep intrval---------------------------------//
  if ( flagPutDeviceSleepNoActivity == 0 ) {    //no Activity flag is activated
    sleepHandle(sleepIntervalNoActivity);       //sleep handler!
  } else if( flagPutDeviceSleepInstant == 0 ) { //sleep when secret message is done
    sleepHandle(sleepIntervalSecretMessage);    //sleep handler!  
  }
  
}//end of loop

//------------------this function called when webserver activated!-------------------------//
void callbackWebHandle() {
if (server.hasArg("userInput")) {             //webpage recieved user input has someting?
  String userInput = server.arg("userInput"); //copy received data to local copy
  flagDatareceivedFromtheMobile = 1;          //set flag of received data to dispplay in loop
  flagPutDeviceSleepNoActivity  = 1;          //reset flag of sleep timer
  memset(bufferTemp,0x00,800);                //clear previous buffer of display
  loopindex = 0;                              //loop index to copy local copy of receievd data to global buffer for display
  while ( userInput[loopindex] != '\0') {     //copy until null!
    bufferTemp[loopindex] = userInput[loopindex];
    loopindex++;
  }
  if ( server.arg("userInput") == "secret?#") { //secret msg received !
    flagDatareceivedFromtheMobile = 2;         //set 2nd flag
    server.send(200, "text/html", htmlSecretpage); //display secret webpage!
  } else {
    server.send(200, "text/html", html_page);  //when no secret message just show normal page
  }
} else {                                       //webpage recieved user input has nothing?
  server.send(200, "text/html", html_page);
}
}//end of function callbackWebHandle

//if(server.arg("username") == "John Doe" && server.arg("password") == "password123") { // If both the username and the password are correct
//server.send(200, "text/html", "<h1>Welcome, " + server.arg("username") + "!</h1><p>Login successful</p>");
//server.on("/", HTTP_GET, handleRoot1);        // Call the 'handleRoot' function when a client requests URI "/"
//server.on("/login", HTTP_POST, handleLogin); // Call the 'handleLogin' function when a POST request is made to URI "/login"

//-------------sleep handler---------------------------------------------------------------//
void sleepHandle(unsigned long setInterval) {
//----when time is up! sleep device!---------//
//1. no activity deep sleep
//2. activity done deep sleep
//3. reset sleep timer when activity happened!
unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= setInterval) {
  previousMillis = currentMillis;
  sleepInstantHandle();
}//check for continous time is up?
}

//-----INstant Sleep---------------------------//
void sleepInstantHandle() {
display.displayClear();
memset(bufferTemp,0x00,800); //clear display buffer
server.close(); //close server
server.stop(); //close server
WiFi.forceSleepBegin(0xFFFFFFF);//Forced Modem Sleep for a more Instant Deep Sleep
//LCD_POWER_OFF();
LCD_BACKLIGHT_OFF(); //disbale backlight
ESP.deepSleepInstant(10E6, WAKE_NO_RFCAL); // deep sleep
}
