//Include Libraries needed
#include "WiFi.h"
#include "ThingSpeak.h"
#include "DHT.h"

//Defining WiFi network and password
#define WIFI_NETWORK "Shent"
#define WIFI_PASSWORD "12345678"
#define WIFI_TIMEOUT_MS (20000)

//Defining DHT and soil moisture pins
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int moisture_sensor_pin = 35; 
int soil_moisture = 0;  

//Defining the ThingSpeak channel ID and API key
#define CHANNEL_ID 1962844
#define CHANNEL_API_KEY "9UOD1HTYQK7AA90C"

WiFiClient client;

//Connect to wifi start
void connectToWiFi()
{
  //Setting ESP32 WiFi mode and giving the WiFi Network name and password
 Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK,WIFI_PASSWORD);

unsigned long startAttempTime = millis();

while(WiFi.status() != WL_CONNECTED && millis() - startAttempTime < WIFI_TIMEOUT_MS)
{
  Serial.print(".");
  delay(100);
}
if(WiFi.status() != WL_CONNECTED)
{
  Serial.print("Failed to connect");
}
else{
  Serial.println("Connected!");
  
}

}
//Connect to WiFi end


void setup() 
{
  Serial.begin(9600);
    dht.begin();  
    connectToWiFi();
    ThingSpeak.begin(client);
}
  
void loop() 
{
  //Temperature and humidity code start
  float temp = dht.readTemperature(); // Gets the values of the temperature
  delay(10);
  float humidity = dht.readHumidity(); // Gets the values of the humidity
  humidity = humidity -1;
 
  // Printing the results on the serial monitor
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.print(" °C ");
  Serial.print("    Humidity = ");
  Serial.print(humidity);
  Serial.println(" % ");
//Temperature and humidity code end

//Soil Moisture Code Start
  soil_moisture = ((4095 - analogRead(moisture_sensor_pin))/4095) * 100;     //Soil Moisture calibration 
  Serial.print("Soil Moisture Value : ");
  Serial.print(soil_moisture);
  Serial.println(" %");
  delay(500);
//Soil Moisture Code End

//Uploading into ThingSpeak
  ThingSpeak.setField(1,humidity );
  ThingSpeak.setField(2, soil_moisture);
  ThingSpeak.setField(3, temp);

  ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);
 


  Serial.println("------------------------------------------------------------------------------");
  
  delay(2000);             
}
