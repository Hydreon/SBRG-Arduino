///////////////////////////  OTHER SENSORS ////////////////////////////////////////
// Delay included in each loop.  Read the tipping bucket.
void delay10mS(int Ndelay) {
  int i;
  for (i = 0; i < Ndelay; i++) {
    TBState = digitalRead(TB_IN);
    if (TBState && !PrevTBState) {
      XTBHunds += 1;
      XTBAcc = XTBHunds / 100.0;
      NewTBAcc = 1;
    }
    PrevTBState = TBState;
    delay(10);
  }
}

void ReadAnemometer(void) {
  AnemVal = analogRead(ANEMOMETER_PIN);
  AnemMPH = AnemVal * MPH_PER_BIT;
}


void ReadTemps(void) {
  // Read ambint temp.
  int err = SimpleDHTErrSuccess;
  float TempC = 0;
  do {
    err = dht22.read2(&TempC, &Humidity, NULL);

  } while (err != SimpleDHTErrSuccess);
  TempF = 32.0 + (TempC * 1.8);

  // Read Load cell temp
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures();
  LCTemp = sensors.getTempFByIndex(0);

  // Record into a FIFO so we may later calculate most much temperature is changing.
  LCTFIFO.addNumber( LCTemp );
  
  // Record Hi and Low load cell temp.
  if ( LCTemp > HiLCT ) {
    HiLCT = LCTemp;
    ScaleAtHi = RawScaleGms;
  } 
  if ( LCTemp < LowLCT ) {
    LowLCT = LCTemp;
    ScaleAtLow = RawScaleGms;
  } 

}
