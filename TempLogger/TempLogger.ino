// #include <LiquidCrystal.h>
#include <SD.h>
#include <SPI.h>
#include <Time.h>

#define MESSAGE_HEADER 'M' //Text message header
#define MESSAGE_LENGTH 17

#define TIME_HEADER 'T' //Time header
#define TIME_LENGTH 11

//TMP36 Pin Variables
int sensorPin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures

const int chipSelect = 4;

// LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
 void setup()
 {
	lcd.begin(16, 2);
	lcd.setCursor(0,0);
	lcd.print("Waiting for time");

	Serial.begin(9600);  //Start the serial connection with the computer
	                   //to view the result open the serial monitor 

	// See if the card is present and can be initialized
	if (!SD.begin(chipSelect)){
		Serial.println("Card failed, or not present!");
		lcd.print("SD CARD FAILED!");
		return; // Stop execution
	}

	Serial.println("Card sucessfully initialized!");
	lcd.setCursor(0,1);
	lcd.print("Card Ready!");

}

void loop()                     // run over and over again
{
	// Check to see if any messages have arrived
	if (Serial.available()){
		processSyncMessage();
	}

	if(timeStatus() == timeNotSet){
		Serial.println("Waiting for the time to be set before logging.");
		delay(1000);
		return;
	}

	//getting the voltage reading from the temperature sensor
	int reading = analogRead(sensorPin);  

	// converting that reading to voltage, for 3.3v arduino use 3.3
	float voltage = reading * 5.0;
	voltage /= 1024.0; 

	// print out the voltage
	Serial.print(voltage); Serial.println(" volts");

	// now print out the temperature
	float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
	                                   //to degrees ((voltage - 500mV) times 100)
	Serial.print(temperatureC); Serial.println(" degrees C");

	// now convert to Fahrenheit
	float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
	Serial.print(temperatureF); Serial.println(" degrees F");

	// Open data file.
	File dataFile = SD.open("Temp.log", FILE_WRITE);

	// Write temperature to card
	if  (dataFile){
		String dataToWrite = getFormatedTime() +
							", " +
							String(voltage) +
							", " +
							String(temperatureF);
		// Write data to card
		dataFile.println(dataToWrite);
		dataFile.close();
		Serial.println(dataToWrite);
	}
	else{
		Serial.println("Error opening log file!!");
	}

	delay(1000);                                     //waiting a second
}

void processSyncMessage(){
	while(Serial.available() >= MESSAGE_LENGTH){
		char id = Serial.read();
		if (id == 'M'){

			String inMessage;
			char c;

			for(int i=0; i < MESSAGE_LENGTH-1; i++){
				inMessage += String(char(Serial.read()));
			}
			Serial.println("I got a message!");
			Serial.println("Here's the message: " + inMessage);
		}
		else if (id == 'T'){
			Serial.println("I got a time!");
			String inTime;
			for(int i=0; i < TIME_LENGTH-1; i++){
				inTime += String(char(Serial.read()));
			}
			Serial.println("Here's the time: " + inTime);
			setTime(inTime.toInt());
		}
	}
}

String getFormatedTime(){
	String hours = formatTimeDigits(hour());
	String minutes = formatTimeDigits(minute());
	String seconds = formatTimeDigits(second());
	String months = monthShortStr(month());
	String days = formatTimeDigits(day());
	return months + " " + days + " " + year() + " " + hours + ":" + minutes + ":" + seconds; 
}

String formatTimeDigits(int digits){
  // add a zero on to the front if less than 9
  String output;
  if (digits < 10){
  	output += "0";
  }
  output += digits;
  return output;
}

