# XOX Drums User Manual

Use this document to understand the behavior, controls, and design inspirations behind each drum module.

---

## CV + Knob Behavior

**All CV inputs are bipolar and clamped to 10vpp. Each knob functions as a 10vpp offset.**

---

## ACCENT

Incoming triggers dynamically increase the volume of a sound source.

- **Trigger**: Rising-edge one-shot trigger input for Accent. Gate length is ignored.
- **Amount**: Controls the volume differential between the accented signal and the unaccented source. Increasing this value reduces the source level while boosting the accented signal.
- **Input**: Audio input  
- **Output**: Audio output

---

## CLAP

Clap emulates the iconic 808 clap. A filtered noise source is shaped by four staggered decay envelopes to simulate multiple people clapping at slightly different times. The fourth envelope features a longer decay, perceived by the listener as reverb.

- **Trigger**: Rising-edge one-shot trigger input for Clap. Gate length is ignored.
- **Energy**: Decay time for envelopes 1–3 (5–15 ms)
- **Spread**: Delay times for envelopes 2–4 (Env2: 20–40 ms, Env3: 35–50 ms, Env4: 45–60 ms)
- **Color**: Cutoff frequency for the bandpass filter on the noise source (800 Hz – 16 kHz)
- **Verb Decay**: Decay time for envelope 4 (20–100 ms)
- **Verb Volume**: Output volume of envelope 4
- **Saturation**: Amount of distortion applied

---

## CLAVE

Clave is inspired by the 808 clave and other classic transistor drum machines of the ’70s and ’80s. A sine wave oscillator is amplitude-modulated by a decay envelope.

- **Trigger**: Rising-edge one-shot trigger input for Clave. Gate length is ignored.
- **Pitch**: Cutoff frequency of the bandpass filter (1 kHz – 2 kHz)
- **Decay**: Envelope decay time for amplitude control (5–25 ms)

---

## CONGABONGO

Inspired by the 808 and other transistor drum machines from the ’70s, Congabongo features two fixed-pitch drums tuned a perfect fourth apart, mimicking traditional conga and bongo tuning. The Slap input both generates a short envelope and chokes the Tone input to replicate the sound of muting a drum while striking its edge.

- **Slap LO**: Rising-edge one-shot trigger input for LO "Slap" input. Gate length is ignored.
- **Tone LO**: Rising-edge one-shot trigger input for LO "Tone" input. Gate length is ignored.
- **Slap HI**: Rising-edge one-shot trigger input for HI "Slap" input. Gate length is ignored.
- **Tone HI**: Rising-edge one-shot trigger input for HI "Tone" input. Gate length is ignored.
- **Pitch**: Base frequency for both drums; pitch range depends on Conga/Bongo switch. Tone Hi is tuned a perfect fourth above Tone Lo.
- **Decay**: Decay time for both tones’ amplitude envelopes. Range varies with Conga/Bongo mode.
- **Conga/Bongo**: Switches between Conga (98–198 Hz, 40–80 ms) and Bongo (200–400 Hz, 15–25 ms)

---

## COWBELL

Cowbell combines synthesis techniques from '70s transistor drum machines with the 808 cymbal/cowbell circuitry. Two detuned square waves are summed, filtered, and shaped by a decay envelope.

- **Trigger**: Rising-edge one-shot trigger input for Cowbell. Gate length is ignored.
- **Pitch**: Frequencies of the oscillators (240–540 Hz and 500–800 Hz)
- **Decay**: Amplitude decay time (20–120 ms)

---

## DUCK

Duck applies an inverse envelope to the input audio, emulating the sidechain compression effect popularized by DJs and producers. This creates a dynamic "pumping" sensation.

- **Trigger**: Rising-edge one-shot trigger input for Duck. Gate length is ignored.
- **Amount**: Controls the intensity of the ducking effect
- **Time**: Duration for the volume to return to normal (50 ms – 2 s)

---

## HI-HAT

Modeled after the 808 hi-hat, this circuit uses four square wave oscillators mixed and processed through multiple filters. A decay envelope controls amplitude. Closed hi-hat uses a fixed decay, while open hi-hat uses a variable one. When choke is enabled, a closed hi-hat trigger immediately silences the open hi-hat.

- **CH. Trig**: Rising-edge one-shot trigger input for closed hi-hat. Gate length is ignored.
- **OH. Trig**: Rising-edge one-shot trigger input for open hi-hat. Gate length is ignored.
- **Pitch**: Base frequency of the oscillators (1 kHz – 2 kHz)
- **Decay**: Amplitude decay time for open hi-hat only (50–300 ms)
- **Brightness**: Cutoff frequency for the bandpass filter (1 kHz – 5 kHz)
- **Thickness**: Cutoff frequency for the highpass filter after the bandpass filter (1 kHz – 10 kHz)
- **Choke**: Enables/disables the choke function between open and closed hi-hat triggers

---

## KICK

Kick is a hybrid of the 808 and 909 bass drum circuits, enhanced with output saturation. Two simultaneous envelopes control amplitude and pitch, fine-tuned for bass drum synthesis.

- **Trigger**: Rising-edge one-shot trigger input for Kick. Gate length is ignored
- **Pitch**: Base frequency of the kick drum (10–40 Hz)
- **Pitch Depth**: Modulation depth of the pitch envelope
- **Pitch Decay**: Decay time for the pitch envelope (5–30 ms)
- **Amp Decay**: Decay time for the amplitude envelope (5–300 ms)
- **Saturation**: Output saturation amount
- **L/M/H**: Selects saturation intensity: Low (2x), Medium (10x), High (100x)

---

## MARACA

Maraca draws from the 808 and other vintage transistor drum machines. A white noise source with a fixed highpass filter is controlled by a decay envelope to create short, snappy bursts.

- **Trigger**: Rising-edge one-shot trigger input for Maraca. Gate length is ignored
- **Decay**: Decay time of the amplitude envelope (2–20 ms)

---

## RIMSHOT

Inspired by the 808 rimshot and Kraftwerk’s "electro click," this circuit uses a short trigger pulse to excite a resonant bandpass filter. The same trigger also modulates the filter’s cutoff slightly to add articulation.

- **Trigger**: Rising-edge one-shot trigger input for Rimshot. Gate length is ignored
- **Pitch**: Bandpass filter cutoff frequency (200 Hz – 2 kHz)
- **Decay**: Resonance of the bandpass filter

---

## SNARE

The snare circuit blends elements of the 808 and 909 snares. A sine oscillator and filtered noise each have individual amplitude envelopes. A crossfader blends the two, while saturation is added post-mix for aggressive tones.

- **Trigger**: Rising-edge one-shot trigger input for Snare. Gate length is ignored
- **Pitch**: Frequency of the sine oscillator (150–380 Hz)
- **Pitch Decay**: Decay time of the pitch envelope (5–25 ms)
- **Pitch Env Amt**: Modulation depth of the pitch envelope
- **Noise Color**: Cutoff for the noise filter (1 kHz – 5 kHz)
- **Noise Decay**: Decay time of the noise envelope (5–80 ms)
- **Body Decay**: Decay time of the sine envelope (5–50 ms)
- **Body/Noise**: Crossfade between sine and noise sources
- **Saturation**: Output distortion amount
- **L/M/H**: Saturation gain level: Low (2x), Medium (10x), High (100x)

---

## TOM

Tom is based on 808 toms and other vintage transistor drums. A sine wave’s pitch and amplitude are each controlled by dedicated decay envelopes.

- **Trigger**: Rising-edge one-shot trigger input for Tom. Gate length is ignored
- **Pitch**: Base oscillator frequency; range depends on L/M/H switch setting
- **Amp Decay**: Amplitude envelope decay time (25–100 ms)
- **Pitch Decay**: Pitch envelope decay time (5–50 ms)
- **Env Depth**: Modulation depth of pitch envelope
- **L/M/H**: Pitch range selection: Low (40–140 Hz), Medium (80–280 Hz), High (160–560 Hz)
