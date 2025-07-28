/////////////////////////  REPORTING /////////////////////////////////////////////
// If debugging we don't send the external sensors.
void SendHeader(void) {
  // Fields are nominally 9 char.
  Serial.print("Minutes  TBAcc    ");
  Serial.print("SBAcc    IntAcc   RawSG    ScaleGms DriftGms UIPH     AmbF     LCT     ");
  Serial.println("Wind     RH         TDT      ");
}

void SendInfo(void) {
  long Seconds = millis() / 1000;
  float Minutes = Seconds / 60.0;
  float SBAccInches = ReportedHund * 0.01;
  float IntInches = IntHund * 0.01;
  ReadAnemometer();
  ReadTemps();
  String Spacer = "     ";

  Serial.print(Minutes, 2);
  Serial.print( Spacer);
  Serial.print(XTBAcc, 2);
  Serial.print( Spacer);
  Serial.print(SBAccInches, 2);
  Serial.print( Spacer);
  Serial.print(IntInches, 2);
  Serial.print( Spacer);
  Serial.print(RawScaleGms, 1);
  Serial.print( Spacer);
  Serial.print(ScaleGms, 1);
  Serial.print( Spacer);
  Serial.print(DriftTareGms, 1);
  Serial.print( Spacer);
  Serial.print( UIPH, 3);
  Serial.print( Spacer);
  Serial.print(TempF, 1);
  Serial.print( Spacer);
  Serial.print(LCTemp, 1);
  Serial.print( Spacer);
    Serial.print(AnemMPH, 0);
  Serial.print( Spacer);
  Serial.print(Humidity, 0);
  Serial.print( Spacer);
  Serial.print( TareDriftThrGms, 2);
  Serial.print( Spacer);  
  Serial.println();

}

void SendLCTempComp( void ){
  if ( ReportedHund < 2) {
    if (HiLCT > LowLCT + 15){
      float NomLCTComp = ( ScaleAtLow - ScaleAtHi ) / ( HiLCT - LowLCT);

      // Adjust NomLCTComp by as much as 0.05
      if (LCTempComp < NomLCTComp) {
          LCTempComp += 0.05;
          if (LCTempComp > NomLCTComp) {
              LCTempComp = NomLCTComp;
          }
      } else if (LCTempComp > NomLCTComp) {
          LCTempComp -= 0.05;
          if (LCTempComp < NomLCTComp) {
              LCTempComp = NomLCTComp;
          }
      }
      Serial.print( "Nominal LCTComp: ");
      Serial.print( NomLCTComp, 3 );
      Serial.print( " New LCTComp: ");
      Serial.println( LCTempComp,3 );
    } else {
      Serial.println( "Not enough temp change to compute LTCTempComp");
    }
  } else {
      Serial.println( "Accumulation so no change to LTCTempComp");
  }
}


