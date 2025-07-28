/////////////////  SCALE FUNCTIONS ////////////////////////
// Return a temperature-compensated scale grams.
// TO-DO return true if there is some anomaly preventing us from
// getting a valid reading.
float GetScaleGms(void) {
  int i;
  int attempts;
  long value;
  float BowlGmsReading;
  float BowlGmsAcc = 0;
  long GBowlUI = 0;
  long ReadingStart;
  float PrevBowlGmsReading = scale.get_units();

  for (i = 0; i < SampleSecs; i++) {
    // The scale read takes quite a bit of time so we need to use the
    // millis() funciton to get an accurate once second sample.
    ReadingStart = millis();
    // Read until we get a reading that is reasonable.
    // A spurious read means the scale ahs returned a value that is so far off
    // it is probabably an error reading, not an actual object.  Usually these
    // are readings of 602.6 gms.
    attempts = 0;
    while (attempts < 10) {
      BowlGmsReading = scale.get_units();
      // Uncomment to test spurious read detection.
      // if (random( 0, 3 ) == 0) BowlGmsReading = 602.6;
      if (abs(BowlGmsReading - PrevBowlGmsReading) < 100) {
        if (attempts == 0) break;
        // Serial.print("Accepted BowlGmsReaading: ");
        // Serial.println(BowlGmsReading, 1);
        break;
      }
      // Serial.print("Spurious BowlGmsReading: ");
      // Serial.println(BowlGmsReading, 1);
      attempts++;
      delay10mS(5);
    }
    PrevBowlGmsReading = BowlGmsReading;
    BowlGmsAcc += BowlGmsReading;
    // Wait until 1 second has elapsed.
    do {
      delay10mS(1);
    } while ((millis() - ReadingStart) / 1000 < 1);
  }
  RawScaleGms = BowlGmsAcc / (float)SampleSecs;
  ScaleGms = RawScaleGms + LCTempComp * (LCTemp - TempAtTare);
  // Uncomment to observe the details of GetBowlGms
  // Serial.print("Sum of readings during the sample: "); Serial.print( BowlGmsAcc, 1 );
  // Serial.print("  ScaleGms: "); Serial.println( ScaleGms, 1 );
  return true;
}

// Set the tare by averaging for long time.
void SetTare(void) {
  long valueAcc = 0;
  long value;
  long valueAvg = 0;
  int i;
  long ReadingStart;

  Serial.print("Setting Tare. ");
  // Need to set the offset to zero, or it will offset the readings from last time!
  scale.set_offset(0);
  for (i = 0; i < SampleSecs; i++) {
    ReadingStart = millis();
    value = scale.get_value();
    // Serial.print("Tare setting value: "); Serial.println( value );
    // TO-DO provision for rejecting spurious readings.
    valueAcc += value;
    do {
      delay10mS(1);
    } while ((millis() - ReadingStart) / 1000 < 1);
  }
  valueAvg = valueAcc / SampleSecs;
  // Serial.print("Tare value Avg: "); Serial.println( valueAvg );
  scale.set_offset(valueAvg);
  
  // Record temperture at tare.
  ReadTemps();
  TempAtTare = LCTemp;
  Serial.print("TempAtTare: ");
  Serial.print(LCTemp, 1);
    
  Serial.println(" Reset Accumulation.");
  AccGms = 0;
  ReportedHund = 0;
  XTBHunds = 0;
  XTBAcc = 0;
  LowLCT = 100;
  HiLCT = 0;
}
