// Pins
int ANEMOMETER_PIN = A0;
const int MOTOR_ON = 3;
const int TB_IN = 4;
const int LOADCELL_SCK = 5;
const int LOADCELL_DOUT = 6;
const int DHT_PIN = 2;
const int ONE_WIRE_BUS = 7;

                                    // Constants 
const long MAX_SAMPLE_UI = 50000;  // More than 0.05" in a sample is a spurious reading.
float MPH_PER_BIT = 0.098;  // Anemometer scale factor.

                                  // Bowl Paramters
const float GmsPerHund =  3.823;  // This many hundredths of an inch per gram of water, for 5.450" dia bowl.
const float KFlow = 0.3;               // Was 0.70 for shallower bowl
const float KFill = 0.0;               // Ratio per inch.  Was 3.33 for shallower bowl

bool TBState = 1;  // External Tipping Bucket.
bool PrevTBState = 1;
long XTBHunds = 0;
float XTBAcc;       // In inches.
bool NewTBAcc = 0;  // Flag TB has accumulated.
int AnemVal;
float AnemMPH;
float AccGms = 0;  // Master Accumulation.
int HundOffset = 1;		// Hundredths of offset.  This much accumulates before we report.  (Like a real TB!).
int IntHund = 0;		// Internal hundredths.  Can go up or down.
int ReportedHund = 0;		// Reported hundredths.  Only goes up.

                        // Global for visibility in reporting.
long TimeOfTare;        // Record the time the tare took place in millisecs since bootup.
long SinceTareSecs;     // How many seconds since the tare took place.
float ScaleGms;          // Scale reading, temperature compensated.
float DriftCompedAccGms;  // Scale reading compensated for temperature and drift.
float RawScaleGms;       // Scale without temperature compensation.  Only needed to know temp comp is working.
float SampleGms;         // Computed from current and previous scale readings.
float DriftTareGms = 0;  // Drift since last Tare.
float TempF = 0;      // Ambient
float Humidity = 0;
float LCTemp = 0;       // Load Cell temperature.
float TempAtTare = 0;   // Of the load cell.
float UIPH = 0;         // Uncompensated Inches Per Hour flow rate.
float LowLCT = 100;
float ScaleAtLow = 0;
float ScaleAtHi = 0;
float HiLCT = 0;

////////////////////////////////////// Load Cell Calibration.  Uncomment one of these groups
// int ProtoNum = 1;              // LOAD CELL NUMBER 1
// float scale_factor = 441.98;   // To get load cell to grams.  For 5 kG load cell.
// float scale_factor = 1073.32;   // To get load cell to grams.  For the 1 kg load cell. number 1
// float LCTempComp = 0.153;       // Load Cell compensation in Grams per degree F.

// int ProtoNum = 2;               // LOAD CELL NUMBER 2
// float scale_factor = 1065.22;   // To get load cell to grams.  For the 1 kg load cell number 2
// float LCTempComp = -0.1;       // Load Cell compensation in Grams per degree F.

int ProtoNum = 3;               // LOAD CELL NUMBER 3
float scale_factor = 1034.05;   // To get load cell to grams.  For the 1 kg load cell number 2
float LCTempComp = 0.2;       // Load Cell compensation in Grams per degree F.

                        // Objects 
FifoAverager DriftFIFO;
FifoAverager LCTFIFO;

// Constants set on setup.
int SampleSecs;         // How long to average reading of the scale to get Bowl Weight.
long SpinDefaultSecs;   // Spin at least this often, bowl full or not.
long SpinNotEmptySecs;  // If the bowl is not empty spin after this many secs.
float FullThrGms;       // If gms exceed this, the bowl is full.
float NotEmptyThrGms;   // If gms exceed this, the bowl is not empty.
float TareDriftThrGms;  // In Gram per sample.  Less than this is tare drift.
float ObjThr = 30.0;    // More than this in a minute is a foreign object.
float NegAnomalyThr = -30.0;
float SpinSecs;  // Will be set by EmptyBowl.

// DEVICES
HX711 scale;                 // Instantiates the scale.
SimpleDHT22 dht22(DHT_PIN);  // Instantiate Ambient Temperature Sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature sensor
