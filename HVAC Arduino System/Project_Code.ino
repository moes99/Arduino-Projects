//Inputs 
const int temp_setpoint = A0; //pot for controlling temperature of room
const int temp_sensor = A4; //tmp36 for measuring temperature.
const int brightness_sensor = A2; //LDR for measuring outside brightness.
const int occupancy_toggle = 2; //Push button that controls the whole system.

//Outputs
const int room_bulb = 3; //the bulb that turns on based on the outside brightness.
const int fan = 5; //motor that pushes the conditioned air into the room.
const int valve_relay = 4; //relay that controls which mode (heating or cooling) turns on.
const int cooling_indicator = 6; //blue LED that indicates when cooling is turned on.
const int heating_indicator = 7; //red LED that indicates when heating is turned on.

//Input Containers
float room_temp; //temperature measured by the temperature sensor.
float user_setpoint; //temperature at which user wants room to be.
float delta_T;
float outside_brightness; //outside brightness measured by the LDR.
int occupancy_state; //determines if people are present in the room.

//Current Variables
float current_room_temp = 0; //Ensures system indicates the change in the room temperature once.
float current_delta_T = 0;
float current_outside_brightness = 0; //Ensures system indicates the change in outdoor brightness once.
float current_user_setpoint = 0; //Ensures system indicates the change in the desired room temperature once.

//Non-physical Variables
int p = 0; //used to limit the number of times the arduino counts one physical press of the occupancy button.
boolean occupancy_state_save = LOW; //saves the state of occupancy_state.

//Time Delay
int time = 500;

//Fan Speeds
int half_speed = 128; //Half speed is 1/2 of 255 (approximately 128) since the digital pins are 8-bit.
int low_speed = 64; //I chose low speed to be 1/4 of full speed i.e. 255/4.

void setup(){
  //Inputs
  pinMode(temp_setpoint, INPUT);
  pinMode(temp_sensor, INPUT);
  pinMode(brightness_sensor, INPUT);
  pinMode(occupancy_toggle, INPUT);
  
  //Output
  pinMode(room_bulb, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(valve_relay, OUTPUT);
  pinMode(cooling_indicator, OUTPUT);
  pinMode(heating_indicator, OUTPUT);
  
  //Initial states
  digitalWrite(fan, LOW);
  digitalWrite(room_bulb, LOW);
  digitalWrite(valve_relay, LOW);
  digitalWrite(cooling_indicator, LOW);
  digitalWrite(heating_indicator, LOW);
  
  //Serial Communication
  Serial.begin(9600);
  delay(time);
  Serial.println("System is now online.");
  delay(time);
  Serial.println("Jarvis is awaiting your command.");
  Serial.println("---------------------------------------");
  
}

void loop(){
  
  //System Initialization
  occupancy_state = digitalRead(occupancy_toggle);
  if (occupancy_state == HIGH && p == 0){
    p = 1;
    Serial.println("This room shows signs of life");
    Serial.println("---------------------------------------");
  }
  
  while (p == 1){
    //Measurements
    delay(time);
    user_setpoint = 14*analogRead(temp_setpoint)/1023 + 16;
    room_temp = 30*analogRead(temp_sensor)/1023 + 10;
    outside_brightness = analogRead(brightness_sensor);
    delta_T = room_temp - user_setpoint;
    
    
    //Indoor Lighting
    if(outside_brightness != current_outside_brightness || room_temp != current_room_temp || user_setpoint != current_user_setpoint){ 
      if (outside_brightness >= 600){
        digitalWrite(room_bulb, LOW);
        Serial.print("It is daytime, and outdoor brightness is: ");
        Serial.println(outside_brightness);
        Serial.println("---------------------------------------");
      }
      else{
        digitalWrite(room_bulb, HIGH);
        Serial.print("It is nightime, and the outdoor brightness is: ");
        Serial.println(outside_brightness);
        Serial.println("---------------------------------------");
      }
      delay(4*time);
    
      //Displaying Room Temperature
      Serial.print("The room's temperature is: ");
      Serial.print(room_temp);
      Serial.println("°C");
    
      //Displaying Setpoint Temperature
      Serial.print("The desired room temperature is: ");
      Serial.print(user_setpoint);
      Serial.println("°C");
    
      //Valve Control
      if (delta_T >= 0) { //This means room is hot.
        digitalWrite(valve_relay, LOW);
        digitalWrite(cooling_indicator, HIGH);
        digitalWrite(heating_indicator, LOW);
        delay(time);
        Serial.println("Cooling valve has been opened.");
      }
      else { //This means room is cold.
        digitalWrite(valve_relay, HIGH);
        digitalWrite(heating_indicator, HIGH);
        digitalWrite(cooling_indicator, LOW);
        delay(time);
        Serial.println("Heating valve has been opened.");
      }
    
      delay(6*time);

      //Fan Control
      if (abs(delta_T) >= 2){
        digitalWrite(fan, HIGH);
        delay(time);
        Serial.println("Fan is on high speed");
        Serial.println("---------------------------------------");
      }

      else if (abs(delta_T) >= 1){
        analogWrite(fan, half_speed);
        delay(time);
        Serial.println("Fan is on medium speed");
        Serial.println("---------------------------------------");
      }

      else if (abs(delta_T) > 0){
        analogWrite(fan, low_speed);
        delay(time);
        Serial.println("Fan is on low speed");
        Serial.println("---------------------------------------");
      }

      else {
        digitalWrite(fan, LOW);
        delay(time);
        Serial.println("Fan is off");
        Serial.println("---------------------------------------");
      }
    }
    
    //Setting Current Values
    //Indoor Brightness
    current_outside_brightness = outside_brightness;
    
    //Temperatures
    current_user_setpoint = user_setpoint;
    current_room_temp = room_temp;
    current_delta_T = delta_T; 
    
    //Exitng while(p == 1)
    occupancy_state = digitalRead(occupancy_toggle);
    if (occupancy_state == HIGH && p == 1){
      occupancy_state_save = HIGH;
      p = 2;
    }
  }
  
  //System Standby
  if (occupancy_state_save == HIGH && p == 2){
    p = 0;
    occupancy_state_save = 0;
    current_room_temp = 0;
    current_user_setpoint = 0;
    current_delta_T = 0;
    current_outside_brightness = 0;
    Serial.println("Humans are evacuating the room.");
    delay(time);
    Serial.println("Initiating standby mode in...");
    delay(2*time);
    Serial.println("3...");
    delay(2*time);
    Serial.println("2...");
    delay(2*time);
    Serial.println("1...");
    delay(2*time);
    
    //Turning off HVAC system
    Serial.println("Turning off HVAC system...");
    delay(time);
    digitalWrite(fan, LOW);
    delay(time);
    Serial.println("Fan is turned off.");
    delay(time);
    digitalWrite(valve_relay, LOW);
    digitalWrite(cooling_indicator, LOW);
    digitalWrite(heating_indicator, LOW);
    Serial.println("Valves are closed.");
    delay(time);
    Serial.println("HVAC system is now off");
    delay(2*time);
    
    //Turning off Indoor Lighting
    Serial.println("Turning off indoor lighting...");
    delay(time);
    digitalWrite(room_bulb, LOW);
    delay(time);
    Serial.println("Indoor lighting is off.");
    delay(time);
    Serial.println("Jarvis is on standby mode.");
    delay(time);
    Serial.println("Press the start/stop button to wake me up.");
    Serial.println("---------------------------------------");
  }
}
