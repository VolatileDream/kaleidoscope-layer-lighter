# Kaleidoscope-LayerLight

This is a plugin for [Kaleidoscope][fw] that colours keys in a specific layer
when that layer is active. And allows for multiple layers to be lit up at the
same time. It also highlights "LockLayer" keys to help leave a layer.

This is a little more general than the [NumPad][np] plugin.

Caveat: This isn't an LEDMode, and won't conflict with other LED lighting, but
will cause noticable slowdown in LED transitions such as Rainbow. It also doesn't
change NumLock settings.

---

# Example

```
// Support for a better Numpad mode, which works on arbitrary layers.

#include "Kaleidoscope-LayerLight.h"

...

KEYMAPS(
   [LAYER_QWERTY] = KEYMAP_STACKED(...),
   [LAYER_GAMING] = KEYMAP_STACKED(...),
   [LAYER_NUMPAD] = KEYMAP_STACKED(...)
);

// Layer Light spec, which maps the layer to the color to display.

const struct LayerSpec specs [] =
    {
      // this is almost equivalent to the NumPad setup, but LayerLight doesn't change
      // the value of the NumLock for users.
      { layer: LAYER_NUMPAD, color: CRGB(160,0,0) },
      { layer: LAYER_GAMING, color: CRGB(0,160,0) }
    };

KALEIDOSCOPE_INIT_PLUGINS(
  ...
  LayerLight,
  ...
);


setup() {
 ...

 // Match the name of the 'struct LayerSpec' declared above.
 LayerLight.specs = specs;
 // Special C++ madness to get the array length of 'specs'
 // this will only work when specs is declared in the same file.
 LayerLight.specLength = sizeof(specs) / sizeof(struct LayerSpec);

 ...
}
```

 [fw]: https://github.com/keyboardio/Kaleidoscope
 [np]: https://github.com/keyboardio/Kaleidoscope-NumPad
