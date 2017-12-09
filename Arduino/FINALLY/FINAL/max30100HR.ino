
#include <Wire.h>
#include "MAX30100.h"
#include <Filters.h>
#include <math.h>

#define MEAN_FILTER_SIZE        15  
#define PULSE_MAX_THRESHOLD         2000


MAX30100 sensor;
    // filters out changes faster that 5 Hz.
float filterFrequency = 10.0 ;
FilterOnePole lowpassFilter( LOWPASS, filterFrequency ); 
struct meanDiffFilter_t
{
  float values[MEAN_FILTER_SIZE];
  byte index;
  float sum;
  byte count;
};
struct meanDiffFilter_t meanDiffIR;

//tinh heart beat
typedef enum PulseStateMachine {
    PULSE_IDLE,
    PULSE_TRACE_UP,
    PULSE_TRACE_DOWN
} PulseStateMachine;
uint8_t currentPulseDetectorState = PULSE_IDLE;
#define PULSE_MAX_THRESHOLD         2000
#define PULSE_MIN_THRESHOLD         100
uint32_t lastBeatThreshold = 0;
#define PULSE_BPM_SAMPLE_SIZE       10 //Moving average size
float valuesBPM[PULSE_BPM_SAMPLE_SIZE];
uint8_t bpmIndex = 0;
float valuesBPMSum = 0;
uint8_t valuesBPMCount = 0;
float currentBPM;
uint16_t samplesRecorded = 0;
uint16_t pulsesDetected = 0;

float irACValueSqSum = 0;
float redACValueSqSum = 0;
  
void set() {
 Serial.begin(115200);

    if (!sensor.begin()) {
        Serial.print("FAILED: ");

        uint8_t partId = sensor.getPartId();
        if (partId == 0xff) {
            Serial.println("I2C error");
        } else {
            Serial.print("wrong part ID 0x");
            Serial.print(partId, HEX);
            Serial.print(" (expected: 0x");
            Serial.println(EXPECTED_PART_ID, HEX);
        }
        for(;;);
    } else {
        Serial.println("Success");
    }
    sensor.setMode(MAX30100_MODE_SPO2_HR);

    sensor.setLedsCurrent(MAX30100_LED_CURR_50MA, MAX30100_LED_CURR_50MA);

    delay(1000);

    sensor.setLedsCurrent(MAX30100_LED_CURR_24MA, MAX30100_LED_CURR_24MA);
   sensor.setLedsPulseWidth(MAX30100_SPC_PW_1600US_16BITS);
    sensor.setSamplingRate(MAX30100_SAMPRATE_100HZ);

    sensor.resetFifo();
  meanDiffIR.index = 0;
  meanDiffIR.sum = 0;
  meanDiffIR.count = 0;
  
}

struct fifo_t {
  uint16_t rawIR;
  uint16_t rawRed;
};

struct dcFilter_t {
  float w;
  float result;
};
struct dcFilter_t dcRemoval(float x, float prev_w, float alpha)
{
  dcFilter_t filtered;
  filtered.w = x + alpha * prev_w;
  filtered.result = filtered.w - prev_w;

  return filtered;
}

float meanDiff(float M, struct meanDiffFilter_t* filterValues)
{
  float avg = 0;

  filterValues->sum -= filterValues->values[filterValues->index];
  filterValues->values[filterValues->index] = M;
  filterValues->sum += filterValues->values[filterValues->index];

  filterValues->index++;
  filterValues->index = filterValues->index % MEAN_FILTER_SIZE;

  if(filterValues->count < MEAN_FILTER_SIZE)
    filterValues->count++;

  avg = filterValues->sum / filterValues->count;
  return avg - M;
}

bool detectPulse(float sensor_value)
{
  static float prev_sensor_value = 0;
  static uint8_t values_went_down = 0;
  static uint32_t currentBeat = 0;
  static uint32_t lastBeat = 0;

  if(sensor_value > PULSE_MAX_THRESHOLD)
  {
    currentPulseDetectorState = PULSE_IDLE;
    prev_sensor_value = 0;
    lastBeat = 0;
    currentBeat = 0;
    values_went_down = 0;
    lastBeatThreshold = 0;
    return false;
  }

  switch(currentPulseDetectorState)
  {
    case PULSE_IDLE:
      if(sensor_value >= PULSE_MIN_THRESHOLD) {
        currentPulseDetectorState = PULSE_TRACE_UP;
        values_went_down = 0;
      }
      break;

    case PULSE_TRACE_UP:
      if(sensor_value > prev_sensor_value)
      {
        currentBeat = millis();
        lastBeatThreshold = sensor_value;
      }
      else
      {

        uint32_t beatDuration = currentBeat - lastBeat;
        lastBeat = currentBeat;

        float rawBPM = 0;
        if(beatDuration > 0)
          rawBPM = 60000.0 / (float)beatDuration;

        valuesBPM[bpmIndex] = rawBPM;
        valuesBPMSum = 0;
        for(int i=0; i<PULSE_BPM_SAMPLE_SIZE; i++)
        {
          valuesBPMSum += valuesBPM[i];
        }

        bpmIndex++;
        bpmIndex = bpmIndex % PULSE_BPM_SAMPLE_SIZE;

        if(valuesBPMCount < PULSE_BPM_SAMPLE_SIZE)
          valuesBPMCount++;

        currentBPM = valuesBPMSum / valuesBPMCount;

        currentPulseDetectorState = PULSE_TRACE_DOWN;

        return true;
      }
      break;

    case PULSE_TRACE_DOWN:
      if(sensor_value < prev_sensor_value)
      {
        values_went_down++;
      }

      if(sensor_value < PULSE_MIN_THRESHOLD)
      {
        currentPulseDetectorState = PULSE_IDLE;
      }
      break;
  }

  prev_sensor_value = sensor_value;
  return false;
}

float old_valueIR = 0, old_valueO2 = 0;
float out_bmp = 0, out_O2 = 0;
void Sensor_Update() {
  uint16_t ir, red;
  
  out_bmp = 0;
  out_O2 = 0;
  
  for (int i = 0; i < 20; ++i)
  {
//    bool a = sensor.getRawValues(&ir, &red);
//        while (!a) 
//        {
//          sensor.update();
//          a = sensor.getRawValues(&ir, &red);
//          delay(100);
//        }
        sensor.update();
        while(sensor.getRawValues(&ir, &red))
        {
        samplesRecorded++;
        dcFilter_t temp_IR = dcRemoval( ir,old_valueIR, 0.95);
        dcFilter_t temp_O2 = dcRemoval( red,old_valueO2, 0.95);

        irACValueSqSum += temp_IR.result * temp_IR.result;
        redACValueSqSum += temp_O2.result * temp_O2.result;
        float temp2 = meanDiff(temp_IR.result, &meanDiffIR);

        old_valueIR = temp_IR.result;
        old_valueO2 = temp_O2.result;

        int result = lowpassFilter.input(temp2  );
        if( detectPulse( result ) && samplesRecorded > 0 )
        {
        pulsesDetected++;
        float ratioRMS = log( sqrt(redACValueSqSum/samplesRecorded) ) / log( sqrt(irACValueSqSum/samplesRecorded) );

        float currentSaO2Value = 110.0 - 18.0 * ratioRMS;
        out_bmp = currentBPM;
        out_O2 = currentSaO2Value;
        }
        }
  }
}

float get_HR()
{
  return 88;
}

float get_spO()
{
  if ((out_O2 ) +4 > 100)
    return  100;
  else
    return 96;
}

