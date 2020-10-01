#pragma once

#include "Kaleidoscope-LEDControl.h"

struct LayerSpec {
  uint8_t layer;
  cRGB color;
};

class LayerLight_ : public kaleidoscope::Plugin {
 public:
  LayerLight_(void) : init(false) {}

  static uint8_t specLength;
  static const struct LayerSpec * specs;

  kaleidoscope::EventHandlerResult onSetup(void);
  kaleidoscope::EventHandlerResult onLayerChange();
  kaleidoscope::EventHandlerResult afterEachCycle();
  void dump_state();

 private:
  bool init;
  void updateKeys();
};

extern LayerLight_ LayerLight;
