/*******************************************************************************

 Lansing Makers Network
 ------------------------------
 
 Artemis23consoleMultiJoyStick.ino - Console Keyboard and Joystick emulator
 
 This work is licensed under a Creative Commons Attribution-ShareAlike 3.0 
 Unported License (CC BY-SA 3.0) http://creativecommons.org/licenses/by-sa/3.0/
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

*******************************************************************************/

/*Uncomment the desired console*/
#define DEBUG_CONSOLE   1
//#define ENGINEERING 	  2
//#define WEAPONS 			  3
//#define SCIENCE 			  4
//#define FIGHTER 			  5
//#define COMMUNICATIONS  6
//#define HELM 						7 (also uses Joystick)

#include <MPR121.h>
#include <Wire.h>
#ifdef HELM
	#include <Joystick.h>
#endif // HELM
#include <Keyboard.h>
#define LENGTH_OF_ARRAY(x) ((sizeof(x)/sizeof(x[0])))
#define numElectrodes 12

typedef struct 
  {
      MPR121_t device;
      uint8_t address;
      unsigned char tthresh[12];
      unsigned char rthresh[12];
      uint8_t modifier[12];
      uint8_t key[12];
      uint8_t pressDuration[12];
  } mprs;

MPR121_t MPR121A;
MPR121_t MPR121B;
MPR121_t MPR121C;

#ifdef ENGINEERING
mprs chips[] = {
                    (mprs) {MPR121A, 0x5A, 
                     //00    01    02    03    04    05    06    07    08    09    10    11                             
                      {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh      
                      {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh     
                      {00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   0x81, 0x81}, //modifier     
                      {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x30, 0x39}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration
                    },
                    
                    (mprs) {MPR121B, 0x5B, 
                     //00    01    02    03    04    05    06    07    08    09    10    11                             
                      {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh      
                      {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh     
                      {0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,   00,   00,   00,   00}, //modifier     
                      {0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0xB0, 0x65, 0x77, 0x20}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration
                    },
                    (mprs) {MPR121C, 0x5C,
                     //00    01    02    03    04    05    06    07    08    09    10    11                             
                      {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh      
                      {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh     
                      {00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00}, //modifier     
                      {0x64, 0x00, 0x61, 0x73, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration                    
                    }
                  };
#endif // ENGINEERING
#ifdef WEAPONS
mprs chips[] = {
                    (mprs) {MPR121A, 0x5A, 
                     //00    01    02    03    04    05    06    07    08    09    10    11                             
                      {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh      
                      {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh     
                      {00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00}, //modifier     
                      {0x6C, 0x6B, 0x00, 0x00, 0x00, 0x62, 0xD7, 0xD8, 0x75, 0x69, 0xC3, 0xC5}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration
                    },
                    
                    (mprs) {MPR121B, 0x5B, 
                     //00    01    02    03    04    05    06    07    08    09    10    11                             
                      {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh      
                      {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh     
                      {00,   00,   0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 00,   00}, //modifier     
                      {0x00, 0xC4, 0x31, 0x32, 0x33, 0x34, 0x34, 0x33, 0x32, 0x31, 0x34, 0x33}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration
                    },
                    (mprs) {MPR121C, 0x5C,
                     //00    01    02    03    04    05    06    07    08    09    10    11                             
                      {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh      
                      {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh     
                      {00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00}, //modifier     
                      {0x32, 0x31, 0x35, 0xC6, 0xC7, 0xC9, 0xC8, 0x79, 0x74, 0x00, 0x00, 0x00}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration                    
                    }
                  };
#endif // WEAPONS
#ifdef SCIENCE
mprs chips[] = {
                    (mprs) {MPR121A, 0x5A, 
                     //00    01    02    03    04    05    06    07    08    09    10    11                             
                      {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh      
                      {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh     
                      {00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00}, //modifier     
                      {0xB0, 0x75, 0x00, 0x69, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration
                    }
                  };
#endif // SCIENCE
#ifdef FIGHTER
mprs chips[] = {
                    (mprs) {MPR121A, 0x5A, 
                     //00    01    02    03    04    05    06    07    08    09    10    11                             
                      {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh      
                      {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh     
                      {00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00}, //modifier     
                      {0x20, 0xB0, 0x77, 0x77, 0x71, 0x72, 0x00, 0x64, 0x61, 0x73, 0xD1, 0xD4}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration
                    }
                  };
#endif // FIGHTER
#ifdef COMMUNICATIONS
mprs chips[] = {
                    (mprs) {MPR121A, 0x5A, 
                     //00    01    02    03    04    05    06    07    08    09    10    11                             
                      {30,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh      
                      {20,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh     
                      {00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00}, //modifier     
                      {0x72, 0x36, 0x35, 0x34, 0x33, 0x31, 0x32, 0x30, 0x37, 0x38, 0x39, 0x00}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration
                    }
                  };

#endif // COMMUNICATIONS
#ifdef HELM
mprs chips[] = {
                    (mprs) {MPR121A, 0x5A, 
                     //00    01    02    03    04    05    06    07    08    09    10    11                             
                      {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh      
                      {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh     
                      {00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00}, //modifier     
                      {0x00, 0x00, 0x00, 0xD1, 0xC5, 0x6B, 0xC3, 0x00, 0xC6, 0xC4, 0xD7, 0xD4}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration
                    },
                    
                    (mprs) {MPR121B, 0x5B, 
                     //00    01    02    03    04    05    06    07    08    09    10    11                             
                      {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh      
                      {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh     
                      {00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00,   00}, //modifier     
                      {0xD8, 0x72, 0x20, 0xB1, 0x34, 0x33, 0x32, 0x31, 0x79, 0x74, 0x6C, 0x00}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration
                    }
                  };
#endif // HELM
#ifdef DEBUG_CONSOLE
mprs chips[] = {
                    (mprs) {MPR121A, 0x5A,                           
                      {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh      
                      {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}, //modifier     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration
                    },
                    
                    (mprs) {MPR121B, 0x5B, 
                      {20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //tthresh      
                      {10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10}, //rthresh     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}, //modifier     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration
                    }/*,
                    (mprs) {MPR121C, 0x5C, 
                      {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}, //tthresh      
                      {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, //rthresh     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}, //modifier     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}, //key     
                      {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00}  //pressDuration
                    }*/
                  };
#endif // DEBUG_CONSOLE

const long interval = 50;
unsigned long previousMillis = 0;
int8_t lastxAxis = 0;
int8_t lastyAxis = 0;
int8_t lasttAxis = 0;
int8_t lastAxis[] = {0,0,0};


void setup()
{

  Serial.begin(115200);
  //while(!Serial);  // only needed if you want serial feedback with the
  		   // Arduino Leonardo or Bare Touch Board
  Serial.println("started");
  #ifndef DEBUG_CONSOLE
		#ifdef HELM
		  Joystick.begin();
	  #endif // HELM
  Keyboard.begin();
  #endif // DEBUG_CONSOLE
  Wire.begin();
  
  // 0x5C is the MPR121 I2C address on the Bare Touch Board
  for(int index=0; index<LENGTH_OF_ARRAY(chips); index++){
    Serial.print("Initializing MPR #");
    Serial.print(index);
    Serial.println();

    if(!chips[index].device.begin(chips[index].address)){ 
      Serial.println("error setting up MPR121");  
      switch(chips[index].device.getError()){
        case NO_ERROR:
          Serial.println("no error");
          break;  
        case ADDRESS_UNKNOWN:
          Serial.println("incorrect address");
          break;
        case READBACK_FAIL:
          Serial.println("readback failure");
          break;
        case OVERCURRENT_FLAG:
          Serial.println("overcurrent on REXT pin");
          break;      
        case OUT_OF_RANGE:
          Serial.println("electrode out of range");
          break;
        case NOT_INITED:
          Serial.println("not initialised");
          break;
        default:
          Serial.println("unknown error");
          break;      
      }
      while(1);
    }

    // pin 4 is the MPR121 interrupt on the Bare Touch Board
    chips[index].device.setInterruptPin(7);

    for(unsigned char channel=0; channel < numElectrodes; channel++){

      // this is the touch threshold - setting it low makes it more like a proximity trigger
      // default value is 40 for touch
      chips[index].device.setTouchThreshold(channel, chips[index].tthresh[channel]);
      
      // this is the release threshold - must ALWAYS be smaller than the touch threshold
      // default value is 20 for touch
      chips[index].device.setReleaseThreshold(channel, chips[index].rthresh[channel]);  
    }
    
    // initial data update
    chips[index].device.updateTouchData();
  }

  Serial.println("end setup");
}

void loop()
{
  unsigned long currentMillis = millis();

	// Capacitive Touch Sensor Keyboard emulator
  for(int index=0; index<LENGTH_OF_ARRAY(chips); index++){
    if(chips[index].device.touchStatusChanged()){
      chips[index].device.updateTouchData();
      for(uint8_t channel=0; channel < numElectrodes; channel++){
          if(chips[index].device.isNewTouch(channel)){
            #ifndef DEBUG_CONSOLE
              if (chips[index].modifier[channel] > 0) {
                Keyboard.press(chips[index].modifier[channel]);
                delay(100);
              }
              if (chips[index].key[channel] > 0) {
              Keyboard.press(chips[index].key[channel]);
              delay(100);
              delay(chips[index].pressDuration[channel]);
              Keyboard.release(chips[index].key[channel]);
              }
              delay(100);
              Keyboard.releaseAll();
            
            #endif // DEBUG_CONSOLE
            Serial.print("device ");
            Serial.print(index, DEC);
            Serial.print(" electrode ");
            Serial.print(channel, DEC);
            Serial.print(" key ");
            Serial.print(chips[index].key[channel], HEX);
            Serial.println(" was just touched!");  
          } else if(chips[index].device.isNewRelease(channel)){
            Serial.print("device ");
            Serial.print(index, DEC);
            Serial.print(" electrode ");
            Serial.print(channel, DEC);
            Serial.println(" was just released.");  
          }
      } 
    }
  }

#ifdef HELM // Joystick controls are only needed if HELM
  // only debounce polling of JoyStick values
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    int8_t xAxis = map(analogRead(A0), 0, 1023, -127, 127);
    if (lastxAxis != xAxis) {
      lastxAxis = xAxis;
      Joystick.setXAxis(xAxis);
    }

    int8_t yAxis = map(analogRead(A1), 0, 1023, -127, 127);
    if (lastyAxis != yAxis) {
      lastyAxis = yAxis;
      Joystick.setYAxis(yAxis);
    }

    int8_t tAxis = map(analogRead(A2), 0, 1023, 0, 255);
    if (lasttAxis != tAxis) {
      lasttAxis = tAxis;
      Joystick.setThrottle(tAxis);
    }
  }
#endif //HELM


}
