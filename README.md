# DG-light

Lightweight module to iluminate various flying discs. (frisbee, DiscGolf,...? )

## Hardware

Kept simple and lightweight as possible. Only: MCU, leds, button, passives.
- **MCU**: **ATMEGA8A**
- **LEDs**:
 - 16x 5mm edge oriented LEDs
 - 1x SMD status LED
- **Buttons**:
 - user button (all settings)
 - reset button (wake from power off)

## V1
Proof of concept. Partialy worked.
### problems:
- **SW**: needed recalibration often (changing surfaces like: grass and asphalt caused capacity changes)
- **HW**:
 - missing charging port 
 - missing battery connect jumper
 - ISP inder battery
*sensitive to palm under electronics -> used as another control element (reset after pickup)*

