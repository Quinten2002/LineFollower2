
#define MOT_A1_PIN 11
#define MOT_A2_PIN 10
#define MOT_B1_PIN 6
#define MOT_B2_PIN 5

void setup(void)
{
  
  pinMode(MOT_A1_PIN, OUTPUT);
  pinMode(MOT_A2_PIN, OUTPUT);
  pinMode(MOT_B1_PIN, OUTPUT);
  pinMode(MOT_B2_PIN, OUTPUT);

  
  digitalWrite(MOT_A1_PIN, LOW);
  digitalWrite(MOT_A2_PIN, LOW);
  digitalWrite(MOT_B1_PIN, LOW);
  digitalWrite(MOT_B2_PIN, LOW);

  
  Serial.begin(9600);
}

void loop(void)
{
  
  for (int i = 0; i < 11; i++) {
    spin_and_wait(25*i, 25*i, 500);
  }
  
  spin_and_wait(255,255,2000);

  
  for (int i = 0; i < 21 ; i++) {
    spin_and_wait(255 - 25*i, 255 - 25*i, 500);
  }

  
  spin_and_wait(-255,-255,2000);

  
  spin_and_wait(0,0,2000);

  
  spin_and_wait(255, 255, 2000);
  spin_and_wait(0, 0, 1000);
  spin_and_wait(-255, 255, 2000);
  spin_and_wait(0, 0, 1000);
  spin_and_wait(-255, -255, 2000);
  spin_and_wait(0, 0, 1000);
  spin_and_wait(255, -255, 2000);
  spin_and_wait(0, 0, 1000);
}

/// Set the current on a motor channel using PWM and directional logic.
///
/// \param pwm    
/// \param IN1_PIN  
/// \param IN2_PIN  
void set_motor_pwm(int pwm, int IN1_PIN, int IN2_PIN)
{
  if (pwm < 0) {  
    analogWrite(IN1_PIN, -pwm);
    digitalWrite(IN2_PIN, LOW);

  } else { 
    digitalWrite(IN1_PIN, LOW);
    analogWrite(IN2_PIN, pwm);
  }
}


/// \param pwm_A  
/// \param pwm_B  
void set_motor_currents(int pwm_A, int pwm_B)
{
  set_motor_pwm(pwm_A, MOT_A1_PIN, MOT_A2_PIN);
  set_motor_pwm(pwm_B, MOT_B1_PIN, MOT_B2_PIN);

  
  Serial.print("Set motor A PWM = ");
  Serial.print(pwm_A);
  Serial.print(" motor B PWM = ");
  Serial.println(pwm_B);
}


/// \param pwm_A  
/// \param pwm_B  
/// \param duration 
void spin_and_wait(int pwm_A, int pwm_B, int duration)
{
  set_motor_currents(pwm_A, pwm_B);
  delay(duration);
}