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
int index;
LightInfo lightArray[lightCount];
int i;
bool update = true;

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  lights.enumerate();
  index = 0;
  populate_circular_buffer();
  //populate_linked_list();
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

/*void populate_linked_list()
{
  int i;
  rgb *curr;
  curr = head = (rgb*)malloc(sizeof(rgb));
  
  for (i = 0; i < lightCount-1; i++) {
      curr->r = 15;
      curr->g = curr->b = 0;
      curr->next = (rgb*)malloc(sizeof(rgb));
      curr = curr->next;
  }
  curr->r = 15;
  curr->g = curr->b = 0;
  curr->next = head;    
  curr = head;
}*/

void populate_circular_buffer()
{
  memset(&lights, 0xFF, lightCount*sizeof(LightInfo));
}
  
  
void add_color(uint8_t r, uint8_t g, uint8_t b)
{
  lightArray[index].r = r;
  lightArray[index].g = g;
  lightArray[index].b = b;
  if (index >= lightCount-1) {
    index = 0;
  }
  else {
    index++;
  }
}

void let_there_be_light()
{
  int i;
  color_t color;
  
  for (i = 0; i < lightCount; i++) {
    color = lights.color(lightArray[index].r, lightArray[index].g, lightArray[index].b);
    lights.set_color(i, DEFAULT_INTENSITY, color);
    if (index >= lightCount - 1) {
      index = 0;
    }
    else {
      index++;
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
  //r = 0; g = 15; b = 0;
  add_color(r, g, b);
}

