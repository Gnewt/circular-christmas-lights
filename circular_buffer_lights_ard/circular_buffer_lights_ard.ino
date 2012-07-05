#include <GEColorEffects.h>

#define outPin 4
#define lightCount 36

#define HEADER 0xF
#define FOOTER 0xE

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} LightInfo;

GEColorEffects lights(outPin, lightCount);
int arrayIndex;
LightInfo lightArray[lightCount];
int i;
bool update = true;

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  lights.enumerate();
  arrayIndex = 0;
  populate_circular_buffer();
  let_there_be_light();
}

void loop()
{
  if (Serial.available() >= 2) {
    handle_serial();
    if (update) {
      let_there_be_light();
    }
  }
}

void populate_circular_buffer()
{
  for (i = 0; i < lightCount; i++) {
    add_color(0, 0, 15);
  }  
}
  
  
void add_color(uint8_t r, uint8_t g, uint8_t b)
{
  lightArray[arrayIndex].r = r;
  lightArray[arrayIndex].g = g;
  lightArray[arrayIndex].b = b;
  if (arrayIndex >= lightCount-1) {
      arrayIndex = 0;
  }
  else {
      arrayIndex++;
  }
}

void let_there_be_light()
{
  int i;
  color_t color;
  
  for (i = 0; i < lightCount; i++) {
    color = lights.color(lightArray[arrayIndex].r, lightArray[arrayIndex].g, lightArray[arrayIndex].b);
    lights.set_color(i, DEFAULT_INTENSITY, color);
    if (arrayIndex >= lightCount - 1) {
      arrayIndex = 0;
    }
    else {
      arrayIndex++;
    }
  }
}

void handle_serial() {
  uint16_t combo;
  uint8_t r, g, b;
  combo = Serial.read()<<8;
  combo = combo + Serial.read();
  if (update) {
    if (combo >> 12 == HEADER) {
      update = false;
    }
  }
  else {
    if (combo >> 12 == FOOTER) {
      update = true;
    }
  }
  r = (combo << 4) >> 12;
  g = (combo << 8) >> 12;
  b = (combo << 12) >> 12;
  add_color(r, g, b);
}

