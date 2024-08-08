#include "SerialCommand.h"
#include "EEPROMAnything.h"

#define SerialPort Serial
#define Baudrate 115200

SerialCommand sCmd(SerialPort);
bool debug;
unsigned long previous, calculationTime;

const int sensor[] = {A5, A4, A3, A2, A1, A0};

struct param_t 
{
  unsigned long cycleTime;
  int black[6];
  int white[6];

} params;

void setup()
{
  SerialPort.begin(Baudrate);

  sCmd.setDefaultHandler(onUnknownCommand);
  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug); 
  sCmd.addCommand("calibrate", onCalibrate);

  EEPROM_readAnything(0, params);
  
  pinMode (13,OUTPUT);
  SerialPort.println("ready");
}

void loop()
{
  sCmd.readSerial();

  unsigned long current = micros();
  if ( current - previous >= params.cycleTime)
  {
    previous = current;

    int normalised[6];

    if (debug) SerialPort.println("loop running");

    SerialPort.print("Normalised values: ");
    for (int i = 0; i < 6; i++)
    {
      normalised[i] = map(analogRead(sensor[i]), params.black[i], params.white[i], 0, 1000);
      SerialPort.print(normalised[i]);
      SerialPort.print(" ");
    }
    SerialPort.println();
  

  int index = 0;
  for (int i = 1; i < 6; i++) if (normalised[i] < normalised[index]) index = i;

  float position;
  if (index == 0) position = -20;
  else if (index == 5) position = 20;
  else
  {
  int sNul = normalised[index];
  int sMinEen = normalised[index-1];
  int sPlusEen = normalised[index+1];

  float b = sPlusEen - sMinEen;
  b = b / 2;
  
  float a = sPlusEen - b - sNul;

  position = -b / (2 * a);
  position += index;
  position -= 2.5;

  position *= 10;
  }
  
  SerialPort.print("positie: ");
  SerialPort.println(position);
  }

  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;  
}

void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();

  if (strcmp(param, "debug") == 0) debug = (strcmp(value, "on") == 0);
  else if (strcmp(param, "cycle") == 0) params.cycleTime = atol(value);
  
  EEPROM_writeAnything(0, params);
}

void onUnknownCommand(char* command)
{
  SerialPort.print("Unknow Command: \"");
  SerialPort.print(command);
  SerialPort.println("\"");
}

void onDebug()
{
  SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);

  SerialPort.print("black: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.black[i]);
    SerialPort.print(" ");
  }
  SerialPort.print(" ");
  
  SerialPort.print("White: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.white[i]);
    SerialPort.print(" ");
  }
  SerialPort.print(" ");
  
  
  SerialPort.print("calculation time: ");
  SerialPort.println(calculationTime);
  calculationTime = 0;
}

void onCalibrate()
{
  char* param = sCmd.next();

  if (strcmp(param, "black") == 0)
  {
    SerialPort.print("start calibrating black... ");
    for (int i = 0; i < 6; i++) params.black[i] = analogRead(sensor[i]);
    SerialPort.println("done");
  }
  else if (strcmp(param, "white") == 0)
  {
    SerialPort.print("start calibrating white... ");    
    for (int i = 0; i < 6; i++) params.white[i] = analogRead(sensor[i]);  
    SerialPort.println("done");      
  }

  EEPROM_writeAnything(0, params);
}




