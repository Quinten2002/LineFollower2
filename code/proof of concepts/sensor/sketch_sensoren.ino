#include "SerialCommand.h"
#include "EEPROMAnything.h"

#define SerialPort Serial
#define Baudrate 115200

SerialCommand sCmd(SerialPort);
bool debug;
unsigned long previous, calculationTime;

const int sensor[] = {A0,A1,A2,A3,A4,A5};


struct param_t
{
  unsigned long cycleTime;

} params;

void setup()
{
  SerialPort.begin(Baudrate);

  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.setDefaultHandler(onUnknownCommand);

  EEPROM_readAnything(0, params);

  pinMode (13, OUTPUT);
  SerialPort.println("ready");
  }

  void loop()
  {
    sCmd.readSerial();
    unsigned long current = micros();
    if (current - previous >= params.cycleTime)
    {
      previous = current;

      SerialPort.print("Values: ");
      for (int i = 0; i < 6; i++)
      {
        SerialPort.print(analogRead(sensor[i]));
        SerialPort.print(" ");
      }
      SerialPort.println();
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

    if(strcmp(param, "cycle") == 0) params.cycleTime = atol(value);

    EEPROM_writeAnything(0, params);
  }

  void onDebug()
  {
    SerialPort.print("cycle time: ");
    SerialPort.println(params.cycleTime);

    SerialPort.print("calculation time: ");
    SerialPort.println(calculationTime);
    calculationTime = 0;
  }





