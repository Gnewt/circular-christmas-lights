#include <GEColorEffects.h>

#define outPin 4
#define lightCount 36

#define HEADER 0xF
#define FOOTER 0xE

struct RGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    struct RGB *next;
};
typedef struct RGB rgb;

GEColorEffects lights(outPin, lightCount);
rgb *head;
uint8_t r, g, b;
int i;
bool update = true;

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  lights.enumerate();
  populate_linked_list();
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

void populate_linked_list()
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
}
  
void add_color(uint8_t r, uint8_t g, uint8_t b)
{
  head->r = r;
  head->g = g;
  head->b = b;
  head = head->next;
}

void let_there_be_light()
{
  int i;
  rgb *curr;
  curr = head;
  color_t color;
  
  for (i = 0; i < lightCount; i++) {
    color = lights.color(curr->r, curr->g, curr->b);
    lights.set_color(i, DEFAULT_INTENSITY, color);
    curr = curr->next;
  }
}

void handle_serial() {
  uint16_t combo;
  uint8_t r, g, b;
  rgb result;
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

