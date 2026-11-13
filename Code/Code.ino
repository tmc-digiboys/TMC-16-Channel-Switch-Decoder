//
// File:      Code.ino
// Author:    Aiko Pras
// History:   2025/12/01 AP Version 1.0
//
// Purpose:   16 Channel switch-decoder for the TMC (Twentse Modelspoorweg Club).
//            To be used in conjunction with he "DCC decoder voor 16 wissels" board (2025/09 AP/V1.0).
//
// Processor AVR32DA48
// Programmer Serial UPDI (2 pins)
//
// Behaviour at the DCC side is comparable to a normal (switch) decoder. The DCC address can be changed 
// via the onboard button. CVs (Configuration Variables) allow modification of various settings; these
// CVs can be accessed via Service Mode (Programming Track) as well as PoM (Programming on Main) messages.
//           
//******************************************************************************************************
#include <Arduino.h>                  // For general definitions
#include "core_Functions.h"           // Include objects such as dcc, accCmd, and core decoder functions
#include "Hardware.h"                 // Pin assignments
#include "MyDefaults.h"               // Default values for this specific board

#define SOFTWARE_VERSION 20           // To distinguish between different software releases


//******************************************************************************************************
DccTimer dccLedTimer;                 // To switch the LED off that signals there is a valid DCC signal
DccTimer accLedTimer;                 // To switch the LED off that signals the DCC command is for me
DccTimer errorLedTimer;               // To switch the LED off that signals an shortcut
IO_Pin_class myHardware;              // Initialises the (IO-pins for) relays, LEDs, and serial  
adc_class myADC;                      // Initialises and uses the ADC
myDefaults_class myDefaults;          // The default address for this board (unique for each board)
 
unsigned int firstDecoderAddress;     // Derived from CV1 + CV9 (1..511)
bool showCommands;                    // Derived from CV34. Should we print accessory commands?


//******************************************************************************************************
void printInfo() {
  if (cvValues.addressNotSet()) Serial.println("Address not set");
  Serial.print("Decoder: "); Serial.print(DECODER); 
  Serial.print(" (SW-release: "); Serial.print(SOFTWARE_VERSION); Serial.println(")");
  Serial.print("First Decoder Address: "); Serial.println(cvValues.storedAddress()); 
  Serial.print("First Accessory Address: ");  Serial.println(cvValues.storedAddress() * 4 + 1); 
  Serial.print("CV33 (value for shortcut): ");   Serial.println(cvValues.read(Shortcut)); 
}                  


void print_details(uint8_t nummer) {
  Serial.print("Relays: ");
  Serial.print(nummer);
  if (accCmd.position == 1) Serial.println(" +");
    else Serial.println(" -");
}


void errorLedOn() {
  digitalWriteFast(LED_ERROR, HIGH);
  errorLedTimer.setTime(5000);  
}

//******************************************************************************************************

void setup() {
  // Step 1: initialise the AVR ports en the ADC 
  myHardware.init(); 
  myADC.init(cvValues.read(Shortcut));  // if the ADC value exceeds this CV value, we assume a shortcut
  //
  // Step 2: Set CV default values (see core_CvValues.h for details) 
  // Decoder type (DecType) and software version (version) are set using cvValues.init().
  // After cvValues.init() is called, modified CV default values are taken from myDefaults.h
  // Updated values take effect after setDefaults() is called, either by a long push of the 
  // onboard programming button, or by a CV-POM or CV-SM message to CV8 (VID).
  cvValues.init(TMC16ChannelSwitchDecoder, SOFTWARE_VERSION);
  myDefaults.init();
  // Step 3: initialise the dcc object with address and other settings
  decoderHardware.init();
  // For 16 relays, we need to listen to 4 decoder addresses
  firstDecoderAddress = cvValues.storedAddress();
  accCmd.setMyAddress(firstDecoderAddress, firstDecoderAddress + 3);
  //
  showCommands = cvValues.read(PrintDetails);
  printInfo();
}


//******************************************************************************************************
void loop() { 
  // Step1: Check if we received a DCC message for this decoder
  // If yes, write to the associataed output port
  unsigned int switchNumber;                  // switch this DCC command is aimed at (1..16)
  if (dcc.input()) {    
    // Step 1: Is the received DCC message intended for me?
    if (dcc.cmdType == Dcc::MyAccessoryCmd) {
      // To what switch (relay) is it directed and is it ON (+) or OFF (-)
      switchNumber = (accCmd.decoderAddress - firstDecoderAddress) * 4 + accCmd.turnout;
      boolean react = false; 
      switch (switchNumber) {        
        // POORT0
        case 1: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY1, HIGH);
            if (myADC.shortcut(ADC_RELAY1)) {
              digitalWriteFast(RELAY1, LOW);
              errorLedOn();
            }   
          }
          else digitalWriteFast(RELAY1, LOW);
        break;
        case 2: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY2, HIGH);
            if (myADC.shortcut(ADC_RELAY2)) {
              digitalWriteFast(RELAY2, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY2, LOW);
        break;
        case 3: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY3, HIGH);
            if (myADC.shortcut(ADC_RELAY3)) {
              digitalWriteFast(RELAY3, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY3, LOW);
        break;
        case 4: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY4, HIGH);
            if (myADC.shortcut(ADC_RELAY4)) {
              digitalWriteFast(RELAY4, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY4, LOW);
        break;
        case 5: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY5, HIGH);
            if (myADC.shortcut(ADC_RELAY5)) {
              digitalWriteFast(RELAY5, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY5, LOW);
        break;
        case 6: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY6, HIGH);
            if (myADC.shortcut(ADC_RELAY6)) {
              digitalWriteFast(RELAY6, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY6, LOW);
        break;
        case 7: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY7, HIGH);
            if (myADC.shortcut(ADC_RELAY7)) {
              digitalWriteFast(RELAY7, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY7, LOW);
        break;
        case 8: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY8, HIGH);
            if (myADC.shortcut(ADC_RELAY8)) {
              digitalWriteFast(RELAY8, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY8, LOW);
        break;
        case 9: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY9, HIGH);
            if (myADC.shortcut(ADC_RELAY9)) {
              digitalWriteFast(RELAY9, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY9, LOW);
        break;
        case 10: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY10, HIGH);
            if (myADC.shortcut(ADC_RELAY10)) {
              digitalWriteFast(RELAY10, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY10, LOW);
        break;
        case 11: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY11, HIGH);
            if (myADC.shortcut(ADC_RELAY11)) {
              digitalWriteFast(RELAY11, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY11, LOW);
        break;
        case 12: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY12, HIGH);
            if (myADC.shortcut(ADC_RELAY12)) {
              digitalWriteFast(RELAY12, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY12, LOW);
        break;
        case 13: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY13, HIGH);
            if (myADC.shortcut(ADC_RELAY13)) {
              digitalWriteFast(RELAY13, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY13, LOW);
        break;
        case 14: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY14, HIGH);
            if (myADC.shortcut(ADC_RELAY14)) {
              digitalWriteFast(RELAY14, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY14, LOW);
        break;
        case 15:
          react = true; 
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY15, HIGH);
            if (myADC.shortcut(ADC_RELAY15)) {
              digitalWriteFast(RELAY15, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY15, LOW);
        break;
        case 16: 
          react = true;
          if (accCmd.position == HIGH) {
            digitalWriteFast(RELAY16, HIGH);
            if (myADC.shortcut(ADC_RELAY16)) {
              digitalWriteFast(RELAY16, LOW);
              errorLedOn();
            }
          }
          else digitalWriteFast(RELAY16, LOW);
        break;
        default:
        break;
      };
      // Turn the ACC LED on, to indicate we have reacted on the accesory message
      if (react) {
        digitalWriteFast(LED_ACC, HIGH);
        accLedTimer.setTime(1000);
        if (showCommands) print_details(switchNumber);    // For debugging 
      };
    }
    else { // PoM or SM programming??
      if (dcc.cmdType == Dcc::MyPomCmd) cvProgramming.processMessage(Dcc::MyPomCmd);
      else if (dcc.cmdType == Dcc::SmCmd) cvProgramming.processMessage(Dcc::SmCmd);
    };
    // Turn the DCC LED on, to indicate the DCC signal is valid
    // We use analogWite, since otherwise the LED shines too bright.
    analogWrite(LED_DCC, 16);
    dccLedTimer.setTime(1000);
  }; 
  //
  // Step 2: After 1 second turn the LEDs off
  if (dccLedTimer.expired()) {analogWrite(LED_DCC, 0);}
  if (accLedTimer.expired()) {digitalWriteFast(LED_ACC, LOW);}
  if (errorLedTimer.expired()) {digitalWriteFast(LED_ERROR, LOW);}
  //
  // Step 3: as frequent as possible we should check if the programming button is pushed 
  // and if the status of the DCC, ACC, PROG and ERROR LEDs should be changed.
  decoderHardware.update();
};
