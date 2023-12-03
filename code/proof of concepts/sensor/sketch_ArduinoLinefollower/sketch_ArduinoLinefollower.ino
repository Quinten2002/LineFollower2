#include "SerialCommand.h"
#include "EEPROMAnything.h"

#define SerialPort Serial
#define Baudrate 115200

SerialCommand sCmd(SerialPort);
bool debug;
unsigned long previous, calculationTime;

const int sensor[] = {A0, A1, A2, A3, A4, A5};

struct param_t
{
  unsigned long cycleTime;
  /* andere parameters die in het eeprom geheugen moeten opgeslagen worden voeg je hier toe ... */
  int black[6];
  int white[6];
} params;

void setup()
{
  SerialPort.begin(Baudrate);

  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.addCommand("calibrate", onCalibrate);
  sCmd.setDefaultHandler(onUnknownCommand);

  EEPROM_readAnything(0, params);

  pinMode(13, OUTPUT);
  SerialPort.println("ready");
}

void loop()
{
  sCmd.readSerial();
 
  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;

    int normalised [6];


    /* code die cyclisch moet uitgevoerd worden programmeer je hier ... */
    SerialPort.print("normalised values: ");
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

    if (index == 0) position = -30;
    else if (index == 5) position = 30; 
    else
    {
    int sNul = normalised[index];
    int sMinEen = normalised[index-1];
    int sPlusEen = normalised[index+1];

    float b = sPlusEen - sMinEen;
    b = b / 2;

    float a = sPlusEen - b - sNul;

    position = -b / (2 * a);
    position += index ;
    position -= 2.5;

    position *= 10;

    
    }

    SerialPort.print("positie: ");
    SerialPort.println(position);

  }

  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;
}

void onUnknownCommand(char *command)
{
  SerialPort.print("unknown command: \"");
  SerialPort.print(command);
  SerialPort.println("\"");
}

void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();  
  
  if (strcmp(param, "cycle") == 0) params.cycleTime = atol(value);
  /* parameters een nieuwe waarde geven via het set commando doe je hier ... */
  
  EEPROM_writeAnything(0, params);
}

void onDebug()
{
  SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);
  
  /* parameters weergeven met behulp van het debug commando doe je hier ... */
  SerialPort.print("black: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.black[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("white: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.white[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");
  
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