

/*    
 *   "Arduino NeoPixel Ring Compass"
 *  Written by: David Ratliff  -  August 2013
 *  Released into the public domain.  The code is free for you to use, change, make your own, etc...
 * However, it is NOT WARRANTEED, GUARANTEED, or even promised to work as may be expected.
 *
 * That said...
 *  The following sketch was written for use with the Adafruit NeoPixel Ring and the 
 * Adafruit LSM303DLHC Compass/Accelerometer to create a simple "Compass" which will point North
 * (**Assuming the ring is oriented correctly!)
 *
 *  This sketch is written to use the SCL & SCK pins on the LSM303 Breakout Board
 *  And, communicating with the NeoPixel Ring on Digital Pin 5 of Arduino UNO R.3
 *      (This can of course be easily changed below.)
 *  
 *  This is an extremely simple and quickly written sketch to prove it could be done and make my video.
 *  --- It does not compensate for 3rd axis rotation. If not held rather horizontally...
 *      compass will typically point "down"  -- PLEASE, feel free to correct this & post updates!  :)
 *
 *  --- The methods used below were also heavily influenced by the
 *      "City Bike NeoPixel Helmet" tuorial & code found on learn.adafruit.com
 *
 *    Thank you!
 */


#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_TSL2561_U.h>
int lux;
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

// *** Uncomment the following line for debugging - to use serial monitor
// #define debug true

#define PIN 6  // Pin NeoWheel is on


/* Assign a unique ID to this sensor at the same time */
//Adafruit_LSM303_Mag mag = Adafruit_LSM303_Mag(12345);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
// Initiate NeoPixel Wheel
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);


//  Assign pixels to heading directions


const int NW = 7;

const int W = 6;

const int SW = 5;

const int S = 4;

const int sE = 3;

const int E = 2;

const int NE = 1;

const int N = 0;
// variable and initial value for "North" indicator
int indicator = 0;

//  brightness level / RGB value used for LEDs
int level = 10;    // valid vallues between 0 - 255... but 0 would be a tad hard to see

float heading;


void setup(void) 
{

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // the following serial functions only used if "#define debug true" at beginning of sketch is UNCOMMENTED 
#ifdef debug
  Serial.begin(9600);
  while (!Serial) ;     //wait for serial monitor to connect on Leonardo and Flora
  Serial.println("Magnetometer Test"); 
  Serial.println("");
Serial.println("Light Sensor Test"); Serial.println("");
    /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Setup the sensor gain and integration time */
  configureSensor();
  
  /* We're ready to go! */
  Serial.println("");
  /* Initialise the sensor */
  if(!tsl.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
#endif

  if(mag.begin())
  {
    // one cycle of red around the ring to signal we're good to go!
    colorWipe(strip.Color(55, 0, 0), 50); // Wipe RED around LED WHEEL - Verfies magnetometer ready
  }
}

void loop(void) 
{
   /* Get a new sensor event */ 
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Display the results (light is measured in lux) */
  if (event.light)
  {
    Serial.print(event.light); Serial.println(" lux");
     level = map(event.light, 0, 1023, 10, 255);
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    Serial.println("Sensor overload");
  }
  /* Get a new sensor event */
 // sensors_event_t event; 
  mag.getEvent(&event);

  const float Pi = 3.14159;

  // Calculate the angle of the vector y,x
  heading = (atan2(event.magnetic.y,event.magnetic.x) * 180) / Pi;

  // Normalize to 0-360
  if (heading < 0)
  {
    heading = 360 + heading;
  }

  // more ebugging serial monitor stuff
#ifdef debug
  Serial.print("Compass Heading: ");
  Serial.println(heading);
#endif

  // use heading to set & show NeoPixels on ring
  setIndicator();
  showIndicator();
  delay(50);    // just to let things settle a bit
}

void setIndicator()
{  // broken up into section to save processor time
  if ((heading >= 11.25)&&(heading < 101.25));  // NNE to E 
  {
   

      if ((heading >= 33.75) && (heading < 56.25))
    {
      indicator = NE;
    }  //end if NE

   

      if ((heading >= 78.75) && (heading < 101.25))
    {
      indicator = E;
    }  //end if E
  }    //end if NNE to E

    if ((heading >= 101.25) && (heading < 191.25))    // ESE to S
  {
   

      if ((heading >= 123.75) && (heading < 146.25))
    {
      indicator = sE;    // Do not change to SE -  "SE" is apparrently a library-used keyword
    }  //end if sE

    

      if ((heading >= 168.75) && (heading < 191.25))
    {
      indicator = S;
    }   //end if S
  }    //end if ESE to S
  if ((heading < 281.25) && (heading > 191.25))    // SSW to W
  {

  
      if ((heading >= 213.75) && (heading < 236.25))
    {
      indicator = SW;
    }   //end if SW

     

      if ((heading >= 258.75) && (heading < 281.25))
    {
      indicator = W;
    }    //end if W
  }    //end if SSW to W

    if ((heading >= 281.25) || (heading < 11.25))    // WNW to N
  {
    
      if ((heading >= 303.75) && (heading < 326.25))
    {
      indicator = NW;
    }  //end if NW

 

      if ((heading >= 348.75) || (heading < 11.25))
    {
      indicator = N;
    }   //end if N

  }  // end if WNW to N

}  // end void setIndicator()

void showIndicator()
{
  // set a little border to highlight the North indicator
  int indicatorLeft = indicator - 1;
  int indicatorRight = indicator + 1;

  // scale / normalize to 0 - 15

  if (indicatorLeft < 0)
  {
    indicatorLeft += 8; 
  }    //end if <0

  if (indicatorRight > 7)
  {
    indicatorRight -= 8; 
  }    //end if <15

  // Assign colors to 'dem thare variables!
  colorWipe(strip.Color(0, 0, level), 0);             //set All Blue (background dial color)
  strip.setPixelColor(indicator, level, 0, 0);        // set indicator RED
  strip.setPixelColor(indicatorLeft, 0, level, 0);    // set indicator border GREEN
  strip.setPixelColor(indicatorRight, 0, level, 0);   // set indicator border GREEN

  strip.show();                                   // Push bits out!

}  // end void showIndicator()

// Fill the pixels one after the other with a color    
// * Borrowed from ADAFRUIT Example Code
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */  
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
}











