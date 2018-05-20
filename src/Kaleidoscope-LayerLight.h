#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Macros.h"
#include "LEDUtils.h"

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
  kaleidoscope::EventHandlerResult afterEachCycle();
  void dump_state();

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
 protected:
  void begin();
  static void legacyLoopHook(bool is_post_clear);
#endif

 private:
  bool init;
  void updateKeys();
};

extern LayerLight_ LayerLight;
