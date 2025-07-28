void setup() {
  pinMode(TB_IN, INPUT);
  pinMode(MOTOR_ON, OUTPUT);
  digitalWrite(MOTOR_ON, LOW);

  Serial.begin(57600);
  Serial.println("Spinning Bowl Rain Gauge SW rev 2024.09.10.1");
  Serial.println("Assuming Deep Bowl 2.5");
  Serial.print( "Calibration for proto # ");
  Serial.print( ProtoNum );
  Serial.print( " ScaleFactor: ");
  Serial.print( scale_factor, 2);
  Serial.print( " LCTempComp: ");
  Serial.print( LCTempComp, 2);
  Serial.print( "KFlow: ");
  Serial.print( KFlow, 2);
  Serial.print( " KFill: ");
  Serial.println( KFill, 2);

  scale.begin(LOADCELL_DOUT, LOADCELL_SCK);

  scale.set_scale(scale_factor);
  sensors.begin();  // Start up the library

  // The Debugging mode constants are set up so sampleas are quick and it is easy to see
  // Accumulation, Tare Drift, Emptying, and Default Spings.
  float FullThrHund;
  float NotEmptyThrHund;    
  if (!Debugging) {
    SampleSecs = 120;
    FullThrHund = 150.0;  // Or 1.50"
    NotEmptyThrHund = 160.0;  // Or 1.60"    
    FullThrGms = FullThrHund * GmsPerHund;
    NotEmptyThrGms = NotEmptyThrHund * GmsPerHund;
    SpinDefaultSecs = 3600 * 8;  // 8 Hours
    SpinNotEmptySecs = 3600 * 2; // 2 hours.
    TareDriftThrGms = 0.5;  // That's 0.158 hunds  / 2 minutes, or .05" per hr.
  } else {
    SampleSecs = 10;
    FullThrHund = 30.0;  // Or 0.20"
    FullThrGms = FullThrHund * GmsPerHund;
    NotEmptyThrHund = 20.0;  // Or 0.20"
    NotEmptyThrGms = NotEmptyThrHund * GmsPerHund;
    SpinDefaultSecs = 300;  // 5 Minutes  
    SpinNotEmptySecs = 60;
    TareDriftThrGms = 1;  // So we can see drift
    Serial.println( "Debugging Mode.");
  }
  ReadTemps();  // Sometimes we get an odd first reading, so we make a reading now.

}
