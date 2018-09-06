#define WS2812BPin 2 //WS2812B Pin
#include <Adafruit_NeoPixel.h>
#define VERSION 0
#define MAXFREQUENCIES 16
#define MAGICVALUE 759387949
struct _g_Settings
{
    uint32_t magic;
    uint8_t version;
    uint8_t channels;
    int16_t pixels;
    int16_t threshold;
    uint32_t flags;
    uint16_t freq[16];
    uint8_t r[16];
    uint8_t g[16];
    uint8_t b[16];
};
_g_Settings g_Settings;
uint8_t g_channel = 0;
int16_t g_brightness = -1;
bool g_racestarted = false;
uint16_t readRSSI();
Adafruit_NeoPixel strip;
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
// ----------------------------------------------------------------------------
void setDefaults()
{
        g_Settings.magic = MAGICVALUE;
        g_Settings.version = VERSION;
        g_Settings.channels = 8; //lets use only 8 frequencies!!!
        g_Settings.pixels = 50;
        g_Settings.flags = 0;
        g_Settings.threshold = 190;
      
        g_Settings.freq[0] = 5658; //RB 1
        g_Settings.r[0] = 255;
        g_Settings.g[0] = 0;
        g_Settings.b[0] = 0;
        
        g_Settings.freq[1] = 5732; //RB 3
        g_Settings.r[1] = 0;
        g_Settings.g[1] = 255;
        g_Settings.b[1] = 0;
        
        g_Settings.freq[2] = 5806; //RB 5
        g_Settings.r[2] = 0;
        g_Settings.g[2] = 0;
        g_Settings.b[2] = 255;
        
        g_Settings.freq[3] = 5917; //RB 8
        g_Settings.r[3] = 255;
        g_Settings.g[3] = 255;
        g_Settings.b[3] = 0;
        
        g_Settings.freq[4] = 5880; //RB 7
        g_Settings.r[4] = 0;
        g_Settings.g[4] = 255;
        g_Settings.b[4] = 255;
        
        g_Settings.freq[5] = 5695; //RB 2
        g_Settings.r[5] = 255;
        g_Settings.g[5] = 0;
        g_Settings.b[5] = 255;
        
        g_Settings.freq[6] = 5769; //RB 4
        g_Settings.r[6] = 255;
        g_Settings.g[6] = 255;
        g_Settings.b[6] = 255;
        
        g_Settings.freq[7] = 5843; //RB 6
        g_Settings.r[7] = 255;
        g_Settings.g[7] = 153;
        g_Settings.b[7] = 53;
}
// ----------------------------------------------------------------------------
void TriggerGateInit() {
    setDefaults();

    strip = Adafruit_NeoPixel(g_Settings.pixels, WS2812BPin, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.show();
    unsigned int i;
    for(i = 0 ; i < strip.numPixels() ; i++)
    {
      strip.setPixelColor(i, Wheel(i * 255/strip.numPixels()));
    }
    strip.show();

}
// ----------------------------------------------------------------------------
void StandaloneTriggerGate()
{
  int i,_r,_g,_b;
  int rssi1=0, rssi2;
  for (i = 0 ; i < g_Settings.channels; i++)
  {
    setModuleFrequency(g_Settings.freq[i]);
    rssi2= readRSSI();
    if (rssi2 > g_Settings.threshold)
    {
      if (rssi2 > rssi1)
      {
        rssi1 = rssi2;
        g_channel = i;
        g_brightness = 16;
        g_racestarted = true;
      }
    }
  }
  if (g_brightness >= 0)
  {
    for(i = 0 ; i < strip.numPixels() ; i++)
    { 
      _r=g_Settings.r[g_channel] * (16 * g_brightness) / 256;
      _g=g_Settings.g[g_channel] * (16 * g_brightness) / 256;
      _b=g_Settings.b[g_channel] * (16 * g_brightness) / 256;
      strip.setPixelColor(i, strip.Color(_r,_g,_b));
    }
    g_brightness--;
    strip.show();
  }
  else
  {
    if (g_racestarted == true)
    {
      g_racestarted = false;
      for(i = 0 ; i < strip.numPixels() ; i++)
      {
        strip.setPixelColor(i, Wheel(i * 255/strip.numPixels()));
      }
      strip.show();
    }    
  }
}

