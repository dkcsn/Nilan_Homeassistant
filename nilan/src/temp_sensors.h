#include "esphome.h"
//#include "sensor.h"
//#include "component.h"
//#include "esphome/core/component.h"
//#include "esphome/components/modbus/modbus.h"

//#define REGISTERCOUNT 17
//#define MAXREGSIZE 26
//#define SENDINTERVAL 30000 // normally set to 180000 milliseconds = 3 minutes. Define as you like
//#define VENTSET 1003
//#define RUNSET 1001
//#define MODESET 1002
//#define TEMPSET 1004

#define READINTERVAL 3000
#define REGSIZE 23

/*enum reqtypes
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
};*/

//static int16_t rsbuffer[MAXREGSIZE];
static const char *TAG = "nilan_temperature";

class TempSensors : public PollingComponent, public esphome::modbus::ModbusDevice
{
  public:
    TempSensors() : PollingComponent(READINTERVAL) {
      dummyData = 0;
      controller_temp_sensor_->publish_state(dummyData);
      exhaust_temp_sensor_->publish_state(dummyData);
      outlet_temp_sensor_->publish_state(dummyData);
      inlet_temp_sensor_->publish_state(dummyData);
      outdoor_temp_sensor_->publish_state(dummyData);
    };
    //void set_controller_temp_sensor(Sensor *controller_temp_sensor) { controller_temp_sensor_ = controller_temp_sensor; }
    //void set_exhaust_temp_sensor(Sensor *exhaust_temp_sensor) { exhaust_temp_sensor_ = exhaust_temp_sensor; }
    //void set_outlet_temp_sensor(Sensor *outlet_temp_sensor) { outlet_temp_sensor_ = outlet_temp_sensor; }
    //void set_inlet_temp_sensor(Sensor *inlet_temp_sensor) { inlet_temp_sensor_ = inlet_temp_sensor; }
    //void set_outdoor_temp_sensor(Sensor *outdoor_temp_sensor) { outdoor_temp_sensor_ = outdoor_temp_sensor; }
   
    void update() override {
      //auto read_result = this->read();
      //ESP_LOGD(TAG, "Update. Read: %d", read_result);
      
      dummyData++;
      controller_temp_sensor_->publish_state(dummyData);
      exhaust_temp_sensor_->publish_state(dummyData+100);
      outlet_temp_sensor_->publish_state(dummyData*1.1);
      inlet_temp_sensor_->publish_state(dummyData*-1);
      outdoor_temp_sensor_->publish_state(dummyData-100);
      dump_config();
    };

    void on_modbus_data(const std::vector<uint8_t> &data) override {
      ESP_LOGD(TAG, "Data received...");
      if (data.size() < 16) {
        ESP_LOGW(TAG, "Invalid data size received");
        return;
      }
    }

    void dump_config() override  {
      ESP_LOGCONFIG(TAG, "Nilan temperature:");
      ESP_LOGCONFIG(TAG, " Address: 0x%02X", this->address_);
      LOG_SENSOR("", "Controller", this->controller_temp_sensor_);
      LOG_SENSOR("", "Exhaust", this->exhaust_temp_sensor_);
      LOG_SENSOR("", "Outlet", this->outlet_temp_sensor_);
      LOG_SENSOR("", "Inlet", this->inlet_temp_sensor_);
      LOG_SENSOR("", "Outdoor", this->outdoor_temp_sensor_);
    }

    float get_setup_priority() const override { 
      return esphome::setup_priority::AFTER_WIFI; 
    }

  protected:
    Sensor *controller_temp_sensor_ = new Sensor();
    Sensor *exhaust_temp_sensor_ = new Sensor();
    Sensor *outlet_temp_sensor_ = new Sensor();
    Sensor *inlet_temp_sensor_ = new Sensor();
    Sensor *outdoor_temp_sensor_ = new Sensor();
    
  private:
    int dummyData;
};