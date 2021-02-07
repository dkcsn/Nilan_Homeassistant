#include "esphome.h"
#include "sensor.h"
#include "ModbusMaster.h"

#define REGISTERCOUNT 17
#define MAXREGSIZE 26
#define SENDINTERVAL 30000 // normally set to 180000 milliseconds = 3 minutes. Define as you like
#define VENTSET 1003
#define RUNSET 1001
#define MODESET 1002
#define TEMPSET 1004

enum reqtypes
{
  reqtemp = 0,
  reqalarm,
  reqtime,
  reqcontrol,
  reqspeed,
  reqairtemp,
  reqairflow,
  reqairheat,
  requser,
  requser2,
  reqinfo,
  reqinputairtemp,
  reqapp,
  reqoutput,
  reqdisplay1,
  reqdisplay2,
  reqdisplay,
  reqmax
};

static int16_t rsbuffer[MAXREGSIZE];

struct NamedSensor
{
  const char* name;
  Sensor* sensor;
};

class TestSensor : public PollingComponent
{
 public:
  TestSensor() : PollingComponent(SENDINTERVAL) {}
  
  // No need to initialize before wifi connection is active
  float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }
  
  Sensor* t0_sensor = new Sensor;
  Sensor* t3_sensor = new Sensor;
  Sensor* t4_sensor = new Sensor;
  Sensor* t7_sensor = new Sensor;
  Sensor* t8_sensor = new Sensor;

  NamedSensor sensors[REGISTERCOUNT][MAXREGSIZE];
  void setup() override {

    /*for( auto type : rr )
    {
      ESP_LOGD("sensor_creation", "Created sensor for group: %d",  rr[type]);
      for(auto i=0; i< regsizes[type]; ++i)
      {
        if(regnames[type][i] != nullptr)
        {
          NamedSensor n;
          n.name = regnames[type][i];
          n.sensor = new Sensor;
          sensors[type][i] = n;
        }
      }
    }*/

    Serial.begin(19200, SERIAL_8E1);
    node.begin(30, Serial);
    
  }
  void update() override {
    //ESP_LOGD("sensor_update", "Sent update for sensor : %s",  sensors[0][0].name);
    
    //ESP_LOGD("sensor_update", "UPDATE");
    for (int i = 0; i < (sizeof(rr)/sizeof(rr[0])); i++)
    {
      reqtypes r = rr[i];
      char result = ReadModbus(regaddresses[r], regsizes[r], rsbuffer, regtypes[r] & 1); 
      if (result == 0)
      {
        ESP_LOGD("sensor_update", "Updating sensors");
        //for (int i = 0; i < regsizes[r]; i++)
        //{
          //auto name = regnames[r][i];
          //sensor_map_[name]->publish_state(rsbuffer[i]);
          //const char *name = getName(r, i);
          //char numstr[8];
          //if (name != nullptr && strlen(name) > 0)
          //{
          //  String mqname = "temp/";
            switch (r)
            {
            /*case reqcontrol:
              //mqname = "ventilation/control/"; // Subscribe to the "control" register
              control = rsbuffer[i];
              control_sensor->publish_state(control);
              //itoa((rsbuffer[i]), numstr, 10);
              break;
            case reqoutput:
              //mqname = "ventilation/output/"; // Subscribe to the "output" register
              //itoa((rsbuffer[i]), numstr, 10);
              output = rsbuffer[i];
              output_sensor->publish_state(output);
              break;
            case reqdisplay:
              mqname = "ventilation/display/"; // Subscribe to the "input display" register
              itoa((rsbuffer[i]), numstr, 10);
              break;
            case reqspeed:
              mqname = "ventilation/speed/"; // Subscribe to the "speed" register
              itoa((rsbuffer[i]), numstr, 10);
              break;
            case reqalarm:
              mqname = "ventilation/alarm/"; // Subscribe to the "alarm" register
              itoa((rsbuffer[i]), numstr, 10);
              break;
            case reqinputairtemp:
              //mqname = "ventilation/inputairtemp/"; // Subscribe to the "inputairtemp" register
              //itoa((rsbuffer[i]), numstr, 10);
              ESP_LOGD("custom", "The value of sensor is: %f", input_air_sensor->state);
              input_air_temp = rsbuffer[i];
              input_air_sensor->publish_state(input_air_temp);
              break;
            case requser:
              mqname = "ventilation/user/"; // Subscribe to the "user" register
              itoa((rsbuffer[i]), numstr, 10);
              break;  */        
            case reqtemp:
              //t0_sensor->publish_state(rsbuffer[0]/100.0);
              //t3_sensor->publish_state(rsbuffer[3]/100.0);
              //t4_sensor->publish_state(rsbuffer[4]/100.0);
              //t7_sensor->publish_state(rsbuffer[7]/100.0);
              //t8_sensor->publish_state(rsbuffer[8]/100.0);
              t0_sensor->publish_state(10);
              t3_sensor->publish_state(13);
              t4_sensor->publish_state(14);
              t7_sensor->publish_state(17);
              t8_sensor->publish_state(18);
              break;
            case reqtime:
            case reqairtemp:
            case reqairheat:
            case requser2:
            case reqinfo:
            case reqapp:
            case reqdisplay1:
            case reqdisplay2:
            case reqmax:
            default:
              break;
            }
            //mqname += (char *)name;
            //mqttclient.publish(mqname.c_str(), numstr);
          }
        }
      //}
    //}

    // Handle text fields
/*    for (int i = 0; i < (sizeof(rr2)/sizeof(rr2[0])); i++) // change value "5" to how many registers you want to subscribe to
    {
      reqtypes r = rr2[i];

      char result = ReadModbus(regaddresses[r], regsizes[r], rsbuffer, regtypes[r] & 1);
      if (result == 0)
      {
        String text = "";
        String mqname = "ventilation/text/";

        for (int i = 0; i < regsizes[r]; i++)
        {
            const char *name = getName(r, i);

            if ((rsbuffer[i] & 0x00ff) == 0xDF) {
              text += (char)0x20; // replace degree sign with space
            } else {
              text += (char)(rsbuffer[i] & 0x00ff);
            }
            if ((rsbuffer[i] >> 8) == 0xDF) {
              text += (char)0x20; // replace degree sign with space
            } else {
              text += (char)(rsbuffer[i] >> 8);
            }
            mqname += name;
        }
      }
    }*/
  }

 private:
  char ReadModbus(uint16_t addr, uint8_t sizer, int16_t *vals, int type)
  {
    char result = 0;
    switch (type)
    {
    case 0:
      result = node.readInputRegisters(addr, sizer);
      break;
    case 1:
      result = node.readHoldingRegisters(addr, sizer);
      break;
    }
    if (result == node.ku8MBSuccess)
    {
      for (int j = 0; j < sizer; j++)
      {
        vals[j] = node.getResponseBuffer(j);
      }
      return result;
    }
    return result;
  }
  char WriteModbus(uint16_t addr, int16_t val)
  {
    node.setTransmitBuffer(0, val);
    char result = 0;
    result = node.writeMultipleRegisters(addr, 1);
    return result;
  }
  
  /*const char *getName(reqtypes type, int address)
  {
    if (address >= 0 && address <= regsizes[type])
    {
      return regnames[type][address];
    }
    return nullptr;
  }*/
  
  int control;
  int output;
  int input_air_temp;
  
  //WiFiServer server(80);
  //WiFiClient client;
  ModbusMaster node;
  reqtypes rr[8] = {reqtemp, reqcontrol, reqoutput, reqspeed, reqalarm, reqinputairtemp, requser, reqdisplay}; // put another register in this line to subscribe
  reqtypes rr2[2] = {reqdisplay1, reqdisplay2}; // put another register in this line to subscribe
  byte regsizes[REGISTERCOUNT] = {23, 10, 6, 8, 2, 6, 2, 0, 6, 6, 14, 7, 4, 26, 4, 4, 1};
  int regaddresses[REGISTERCOUNT] = {200, 400, 300, 1000, 200, 1200, 1100, 0, 600, 610, 100, 1200, 0, 100, 2002, 2007, 3000};
  byte regtypes[REGISTERCOUNT] = {8, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 1, 4, 4, 8};

  const char *regnames[REGISTERCOUNT][MAXREGSIZE] = {
    //temp
    {"T0_Controller", nullptr, nullptr, "T3_Exhaust", "T4_Outlet", nullptr, nullptr, "T7_Inlet", "T8_Outdoor", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "T15_Room", nullptr, nullptr, nullptr, nullptr, nullptr, "RH", nullptr},
    //alarm
    {"Status", "List_1_ID ", "List_1_Date", "List_1_Time", "List_2_ID ", "List_2_Date", "List_2_Time", "List_3_ID ", "List_3_Date", "List_3_Time"},
    //time
    {"Second", "Minute", "Hour", "Day", "Month", "Year"},
    //control
    {"Type", "RunSet", "ModeSet", "VentSet", "TempSet", "ServiceMode", "ServicePct", "Preset"},
    //speed
    {"ExhaustSpeed", "InletSpeed"},
    //airtemp
    {"CoolSet", "TempMinSum", "TempMinWin", "TempMaxSum", "TempMaxWin", "TempSummer"},
    //airflow
    {"AirExchMode", "CoolVent"},
    //airheat
    {},
    //program.user
    {"UserFuncAct", "UserFuncSet", "UserTimeSet", "UserVentSet", "UserTempSet", "UserOffsSet"},
    //program.user2
    {"User2FuncAct", "User2FuncSet", "User2TimeSet", "User2VentSet", "UserTempSet", "UserOffsSet"},
    //info
    {"UserFunc", "AirFilter", "DoorOpen", "Smoke", "MotorThermo", "Frost_overht", "AirFlow", "P_Hi", "P_Lo", "Boil", "3WayPos", "DefrostHG", "Defrost", "UserFunc_2"},
    //inputairtemp
    {"IsSummer", "TempInletSet", "TempControl", "TempRoom", "EffPct", "CapSet", "CapAct"},
    //app
    {"Bus.Version", "VersionMajor", "VersionMinor", "VersionRelease"},
    //output
    {"AirFlap", "SmokeFlap", "BypassOpen", "BypassClose", "AirCircPump", "AirHeatAllow", "AirHeat_1", "AirHeat_2", "AirHeat_3", "Compressor", "Compressor_2", "4WayCool", "HotGasHeat", "HotGasCool", "CondOpen", "CondClose", "WaterHeat", "3WayValve", "CenCircPump", "CenHeat_1", "CenHeat_2", "CenHeat_3", "CenHeatExt", "UserFunc", "UserFunc_2", "Defrosting"},
    //display1
    {"Text_1_2", "Text_3_4", "Text_5_6", "Text_7_8"},
    //display2
    {"Text_9_10", "Text_11_12", "Text_13_14", "Text_15_16"},
    //airbypass
    {"AirBypass/IsOpen"}};
};