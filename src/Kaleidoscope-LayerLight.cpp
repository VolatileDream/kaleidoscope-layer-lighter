#include "Kaleidoscope.h"
#include "Kaleidoscope-LayerLight.h"
#include "Kaleidoscope-LEDControl.h"

const struct LayerSpec * LayerLight_::specs;
uint8_t LayerLight_::specLength;

kaleidoscope::EventHandlerResult LayerLight_::onSetup(void) {
  return kaleidoscope::EventHandlerResult::OK;
}

void LayerLight_::dump_state() {
  Serial.print("LayerLight_::dump_state()\nlayers: ");
  Serial.println(specLength);
  for (uint8_t specIndex = 0; specIndex < specLength; specIndex ++) {
    Serial.print("layer: ");
    Serial.print(specs[specIndex].layer);
    Serial.print(" color: ");
    Serial.print(" r:");
    Serial.print(specs[specIndex].color.r);
    Serial.print(" g:");
    Serial.print(specs[specIndex].color.g);
    Serial.print(" b:");
    Serial.print(specs[specIndex].color.b);
    Serial.println("");
  }
}

kaleidoscope::EventHandlerResult LayerLight_::onLayerChange() {
  uint8_t activeLayers = 0;
  for (uint8_t specIndex = 0; specIndex < specLength; specIndex ++) {
    if (Layer.isActive(specs[specIndex].layer)) {
      activeLayers++;
    }
  }

  // Were active, now not.
  if (activeLayers == 0) {
    if (init) {
      init = false;
      // need to unset the keys that we set earlier.
      LEDControl.set_mode(LEDControl.get_mode_index());
    }
    return kaleidoscope::EventHandlerResult::OK;
  }

  init = true;
  updateKeys();

  return kaleidoscope::EventHandlerResult::OK;
}

kaleidoscope::EventHandlerResult LayerLight_::afterEachCycle() {
  constexpr uint8_t lock_bits = (KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP);
  if (!init) {
    return kaleidoscope::EventHandlerResult::OK;
  }

  cRGB color = breath_compute();
  for (uint8_t r = 0; r < Kaleidoscope.device().matrix_rows; r++) {
    for (uint8_t c = 0; c < Kaleidoscope.device().matrix_columns; c++) {
      KeyAddr addr = KeyAddr(r, c);
      Key k = Layer.lookupOnActiveLayer(addr);
      if (k.getFlags() == lock_bits && Layer.isActive(k.getKeyCode())) {
        LEDControl.setCrgbAt(addr, color);
      }
    }
  }
  return kaleidoscope::EventHandlerResult::OK;
}

void LayerLight_::updateKeys() {
  // Can only loop through the keys once. otherwise we run into problems with
  // displaying when multiple layers are active.
  cRGB color = breath_compute();
  for (uint8_t r = 0; r < Kaleidoscope.device().matrix_rows; r++) {
    for (uint8_t c = 0; c < Kaleidoscope.device().matrix_columns; c++) {
      KeyAddr addr = KeyAddr(r, c);
      Key k = Layer.lookupOnActiveLayer(addr);

      bool override_opt = false;
      cRGB override_color;
      for (uint8_t i = 0; i < specLength; i++) {
        if (!Layer.isActive(specs[i].layer)) continue;
        Key layer_key = Layer.getKey(specs[i].layer, addr);

        if (k == LockLayer(specs[i].layer)) {
          override_opt = true;
          override_color = color;
          break; // disallow other layers from changing this.
        } else if ((k != layer_key) || (k == Key_NoKey) || (k.flags != KEY_FLAGS)) {
          // nop. We want to allow refreshAt to trigger. But only if all the layers agree.
        } else {
          override_opt = true;
          override_color = specs[i].color;
        }
      } // loop through layers
      if (override_opt) {
        LEDControl.setCrgbAt(r, c, override_color);
      } else {
        LEDControl.refreshAt(r, c);
      }
    }
  }
}

LayerLight_ LayerLight;
