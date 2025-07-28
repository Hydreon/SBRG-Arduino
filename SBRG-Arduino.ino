// Spinning Bowl Rain Gauge
#include "HX711.h"
#include <SimpleDHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

bool Debugging = false;            // If set thngs happen much faster.
#include "FifoAverager.h"
#include "SBRG.h"
#include "Sensors.h"
#include "Reporting.h"
#include "Spin.h"
#include "Scale.h"
#include "Setup.h"

///////////////////////////  THE MAIN LOOP ////////////////////////////////////////
void loop() {
  float PrevScaleGms = 0;
  EmptyTheBowl();
  SendInfo();   // Send so we see any ARG data.
  SetTare();
  TimeOfTare = millis();
  // CompensateForTareSample();  // TO-DO This is broken.
  DriftTareGms = 0.0;
  SendHeader();
  do {
    GetScaleGms();
    SampleGms = ScaleGms - PrevScaleGms;

    // Detect anomalies
    if ((SampleGms > ObjThr) || (SampleGms < NegAnomalyThr)) {
      Serial.print("Anomaly detected. Sample Gms: ");
      Serial.println(SampleGms, 1);
      // break;
    }

    updateDrift();
    // Accumulate.
    AccGms += SampleGms * NetComp( SampleGms);
    DriftCompedAccGms = AccGms - DriftTareGms;
    IntHund = int( DriftCompedAccGms / GmsPerHund);
    if ((IntHund - HundOffset) > ReportedHund) {
      ReportedHund = IntHund - HundOffset;
    }

    PrevScaleGms = ScaleGms;
    SinceTareSecs = (millis() - TimeOfTare) / 1000;
    SendInfo();
  } while (!TimeToSpin());
}

float NetComp( float SampleGms ){
  float GmsPerSec = SampleGms / SampleSecs;
  UIPH = ( GmsPerSec / GmsPerHund ) * 3600 / 100;
  
  float FlowComp = KFlow * UIPH;
  float FillComp = (IntHund *0.01) * KFill;
  float NetComp = 1.0 + FlowComp * (1.0 + FillComp);
  // Serial.print( "FlowComp: ");
  // Serial.print( FlowComp, 3 );
  // Serial.print( " FillComp: ");
  // Serial.print( FillComp, 3 );
  // Serial.print( " NetComp: ");
  // Serial.println( NetComp, 3);
  return NetComp;
}


// We do not accumulate during the time we spin or we tare.
// So, add to accumulation to compensate for this.
void CompensateForTareSample(void) {
  // Detect anomalies
  if ((SampleGms > ObjThr) || (SampleGms < NegAnomalyThr)) {
    return;
  }

  float CompFactor = ((float)SpinSecs + (float)SampleSecs) / (float)SampleSecs;
  float TareComp = CompFactor * SampleGms;
  Serial.print("Rain during Tare Comp Factor: ");
  Serial.print(CompFactor, 2);
  Serial.print("Rain during Tare Compensation gms: ");
  Serial.println(TareComp, 1);
  // Accumulate, unless it is Tare Drift.
  if (TareComp > TareDriftThrGms) {
    // SampleGms is over the threshold of drift, so accumulate.
    AccGms += TareComp;
  } else {
    // SampleGms is under the threshold of drift, so do not accumulate.
    DriftTareGms += TareComp;
  }
  SendInfo();
}

void updateDrift(void) {
  // We don't clear this with tare.
  // Because SampleGms is a derivative, unaffected by re-tare.
  DriftFIFO.addNumber(SampleGms);

  // Adjust drift thresholds with temperature swings.
  // If temperature is changing a lot then a small change in the scale is more likely to be drift, so raise threshold.
  float LCTRange = LCTFIFO.getRange();
  // Serial.print("LCTRange: "); Serial.print( LCTRange, 1 );
  TareDriftThrGms = 0.1 + LCTRange *  0.08;
  
  // Adjust threshold with humidity.  If it is dry,it is less likely that a small change in the scale is due
  // to a light mist, rather than real rain, so raise the threshold.
  float HumidityThr = 70;
  float HumidityAdder = 0;
  if ( Humidity < HumidityThr ){
    HumidityAdder = (HumidityThr - Humidity) * 0.02;
    if ( HumidityAdder > 0.5) HumidityAdder = 0.5;
  }
  TareDriftThrGms += HumidityAdder;
  // Serial.print( " DThresh: "); Serial.println( TareDriftThrGms, 2);
  
  // Call it drift if the average of SamleGms is below the thershold.
  // If the average sample is over that threshold, it is real rain, not drift.
  if (abs(DriftFIFO.getAverage()) < TareDriftThrGms) {
    DriftTareGms += DriftFIFO.getAverage();
  }
}

// Determine whether or not it is time to spin the bowl.
bool TimeToSpin() {
  // If the bowl is almost full, time to spin!
  if (ScaleGms > FullThrGms) {
    Serial.println("Emptying Bowl-- Full");
    return true;
  }

  // If it has been a long simce the last spin, time to spin!
  if (SinceTareSecs > SpinDefaultSecs) {
    Serial.println("Emptying Bowl-- Timeout");
    SendLCTempComp();
    return true;
  }

  // If there has been some accumulation, and it's been awile since emptying,
  // empty the bowl.
  if ((SinceTareSecs > SpinNotEmptySecs) && (DriftCompedAccGms > NotEmptyThrGms)) {
    Serial.println("Emptying Bowl-- Some Accumulation");
    return true;
  }

  return false;
}

