
#pragma once

#include <numbers>

namespace juke {

class HighPassFilter {
  public:
	HighPassFilter(float cutoff_hz, float sample_rate) { set_cutoff(cutoff_hz, sample_rate); }

	void set_cutoff(float cutoff_hz, float sample_rate) {
		if (cutoff_hz <= 0.0f) {
			m_alpha = 0.0f;
			return;
		}

		auto const dt = 1.0f / sample_rate;
		auto const rc = 1.0f / (2.0f * std::numbers::pi * cutoff_hz);
		m_alpha = rc / (rc + dt);
	}

	float process(float input) {
		if (m_alpha == 0.0f) return input;
		auto output = m_alpha * (m_prev_out + input - m_prev_in);
		m_prev_in = input;
		m_prev_out = output;
		return output;
	}

  private:
	float m_prev_in{};
	float m_prev_out{};
	float m_alpha{};
};

} // namespace juke
