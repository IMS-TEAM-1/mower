//Attaches interupts to the encoders when storing the puleses, may be used for dead reckoning
float gyroValueAtStart = 0;
float gyroValueAtEnd = 0;

void setupEncoderInterrupts(){
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
}

//Following two functions are used when reading the pulses generated by the encoders when the motors are moving
void isr_process_encoder1(void)
{
  if(digitalRead(Encoder_1.getPortB()) == 0){
    Encoder_1.pulsePosMinus();
  }else{
    Encoder_1.pulsePosPlus();
  }
}
void isr_process_encoder2(void)
{
  if(digitalRead(Encoder_2.getPortB()) == 0){
    Encoder_2.pulsePosMinus();
  }else{
    Encoder_2.pulsePosPlus();
  }
}

long getEncoder1Pulses(){
  return Encoder_1.getPulsePos();
}
long getEncoder2Pulses(){
  return Encoder_2.getPulsePos();
}

void printEncoderPulseValues(){
  Serial.println("Encoder 1: ");
  Serial.println(getEncoder1Pulses());
  Serial.println("Encoder 2: ");
  Serial.println(getEncoder2Pulses());
}

void resetEncoderValues(){
  Encoder_1.setPulsePos(0);
  Encoder_2.setPulsePos(0);
  resetGyroStartAndEnd();
}

float getEncoderAverage(){
  return (((-1* getEncoder1Pulses()) + getEncoder2Pulses() ) * 0.5);
}

float getDistanceTravelled(){
  return (getEncoderAverage() * MILLIMETER_PER_ENCOER_PULSE);
}

void setGyroValueAtStart(float value){
  gyroValueAtStart = value;
}

void setGyroValueAtEnd(float value){
  gyroValueAtEnd = value;
}

void resetGyroValueAtStart(){
  setGyroValueAtStart(getGyroZ());
}

void resetGyroValueAtEnd(){
  setGyroValueAtEnd(getGyroZ());
}

void resetGyroStartAndEnd(){
  resetGyroValueAtStart();
  resetGyroValueAtEnd();
}

float getGyroValueAtStart(){
  return gyroValueAtStart;
}

float getGyroValueAtEnd(){
  return gyroValueAtEnd;
}

float getAverageGyroValue(){
  return ((getGyroValueAtStart() + getGyroValueAtEnd()) * 0.5);
}
