/*************************************************************************************

	Signalizer - cross-platform audio visualization plugin

	Frequency -> musical note helper (added for the hover frequency readout).
	Converts a frequency in Hz to the nearest note name and cents deviation,
	e.g. 220.0 -> "A3 +0", 47.6 -> "F#0 +50".

*************************************************************************************/

#ifndef SIGNALIZER_FREQUENCYTONOTE_H
#define SIGNALIZER_FREQUENCYTONOTE_H

#include <cmath>
#include <cstdio>
#include <cstddef>

namespace Signalizer
{
	// MIDI 69 = A4 = 440 Hz. Returns the nearest MIDI note and cents in [-50, +50].
	inline void frequencyToMidiCents(double hz, int& midiNote, int& cents)
	{
		const double midi = 69.0 + 12.0 * std::log2(hz / 440.0);
		midiNote = static_cast<int>(std::lround(midi));
		cents = static_cast<int>(std::lround((midi - midiNote) * 100.0));
	}

	// Writes e.g. "A3 +0" into out. For non-positive / non-finite hz writes "--".
	inline void frequencyToNoteName(double hz, char* out, std::size_t outSize)
	{
		if (!(hz > 0.0) || !std::isfinite(hz))
		{
			std::snprintf(out, outSize, "--");
			return;
		}

		static const char* const names[12] =
			{ "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

		int midiNote, cents;
		frequencyToMidiCents(hz, midiNote, cents);

		const int pitchClass = ((midiNote % 12) + 12) % 12;
		const int octave = midiNote / 12 - 1;   // MIDI 69 -> A4

		std::snprintf(out, outSize, "%s%d %+d", names[pitchClass], octave, cents);
	}
}

#endif
