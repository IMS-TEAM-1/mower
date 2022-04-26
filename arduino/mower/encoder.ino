//Attaches interupts to the encoders when storing the puleses, may be used for dead reckoning
void setupEncoderInterrupts(){
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
}
