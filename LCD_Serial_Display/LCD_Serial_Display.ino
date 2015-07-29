#include <LiquidCrystal.h>
#include <Time.h>


LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 


#define MESSAGE_HEADER 'M' //Text message header
#define MESSAGE_LENGTH 17

#define TIME_HEADER 'T' //Time header
#define TIME_LENGTH 10

void setup() {
	Serial.begin(9600);

	lcd.begin(16, 2); 
	lcd.setCursor(0,0);
	lcd.print("Waiting...");
}

void loop() {
	lcd.setCursor(0,1);
	lcd.print(getFormatedTime());

	if (Serial.available()){
  		processSyncMessage();
	}
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
			lcd.setCursor(0,0);
			lcd.print(inMessage);
		}
		else if (id == 'T'){
			Serial.println("I got a time!");
		}
	}
}

String getFormatedTime(){
  String hours = formatTimeDigits(hour());
  String minutes = formatTimeDigits(minute());
  String seconds = formatTimeDigits(second());
  return hours + ":" + minutes + ":" + seconds; 
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

