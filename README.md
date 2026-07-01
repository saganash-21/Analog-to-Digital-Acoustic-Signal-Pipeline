# Analog-to-Digital Acoustic Signal Pipeline & Isolated Load Controller
Jeremy Ndirangu

## ⚙️ Executive Summary
This project demonstrates a robust, custom-built analog signal conditioning pipeline designed to extract discrete, reliable logic triggers from a chaotic acoustic environment. To validate the system's industrial applicability, the processed digital signal bridges a mixed-voltage logic gap to control an ESP32 hardware interrupt, which subsequently drives a high-current inductive DC motor.



## 🏗️ System Architecture & Signal Journey
The architecture deliberately avoids monolithic sensor modules in favor of discrete signal conditioning. This ensures absolute control over hysteresis, voltage boundaries, and power isolation across distinct domains.

*   **1. Transducer Stage:** A KY-038 Electret Microphone converts raw physical air pressure into an Alternating Current (AC) wave superimposed on a 1.48V DC baseline.
*   **2. Analog Conditioning Stage:** An operational amplifier is biased with a 10 kΩ voltage divider to establish a 1.65V virtual ground. A 0.1 µF coupling capacitor establishes a 318.3 Hz high-pass filter to block environmental rumble. The signal then passes through a non-inverting amplifier configured with a 101x operational gain, scaling a nominal 20 mV acoustic spike into a 2.2V AC swing.
*   **3. Envelope Detector & Unity Buffer:** A half-wave rectifier diode strips the negative AC wave, charging a parallel RC tank circuit. The 10 µF capacitor and 10 kΩ resistor stretch the microsecond transient into a sustained 0.1-second DC logic envelope, which is then stabilized by a unity gain buffer.
*   **4. Digitizer & Logic Level Bridge:** A Schmitt Trigger configuration establishes a strict upper trip point of 1.05V and a lower trip point of 0.99V to eliminate signal chattering. This generates a clean 1.95V digital edge. A discrete 2N2222 NPN transistor operates as an inverting logic level shifter to bridge the 1.95V signal safely into the 3.3V domain.
*   **5. Firmware Engine (Digital Stage):** `IRAM_ATTR` hardware interrupts on the ESP32 catch the physical digital edge, deploying a mathematical debounce window to reject reverberations.
*   **6. Solid-State Actuation & Protection:** The ESP32 drives an N-Channel MOSFET (M1) via a 330 Ω series resistor, limiting instantaneous inrush current to the parasitic gate capacitance. The MOSFET controls a 5V Brushless DC (BLDC) fan. A 1N4004 flyback diode is placed in parallel across the fan load to provide a safe recirculation path for destructive back-EMF spikes during motor cutoff.
*   **7. Visual Status Indication:** Dedicated Yellow and Green LEDs provide hardware-level diagnostic feedback. Each LED is driven by isolated logic pins and protected by a 220 Ω current-limiting resistor tied directly to ground.

## 📐 Hardware Design Constraints
This system enforces a strict hardware design constraint at the logic bridge: the collector of the NPN transistor is tied to the 3.3V logic domain exclusively via a physical 5.1 kΩ external pull-up resistor. 

Internal microcontroller pull-ups are strictly prohibited in this design architecture. The physical 5.1 kΩ pull-up guarantees a pristine, high-current logic HIGH when the transistor is in cutoff, completely eliminating floating inputs and ensuring industrial-grade signal stability before the hardware interrupt is fired.

## 🧮 Mathematical Design & Schematics
*   **System Mathematics:** Detailed mathematical derivations for the op-amp gain stages, RC time constants, virtual ground bridging, and Schmitt trigger hysteresis are documented in `docs/System_Design_Criteria.pdf`.
*   **Wiring Diagram:** The complete mixed-signal PCB routing drafted in Eagle CAD is available in `schematics/Complete_Circuit_Diagram.pdf`.
*   **System Flow:** The high-level data pipeline mapping is available in `docs/Block_Diagram.png`.

## 💻 Firmware
The C++ logic is entirely interrupt-driven, minimizing processor polling cycles and allowing for concurrent operations. The source code is available in `firmware/main`.
