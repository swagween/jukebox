
#pragma once

#include <numbers>

namespace juke {

class LowPassFilter {
  public:
	LowPassFilter(float cutoff_hz, float sample_rate) { set_cutoff(cutoff_hz, sample_rate); }

	void set_cutoff(float cutoff_hz, float sample_rate) {
		auto const rc = 1.0f / (2.0f * std::numbers::pi * cutoff_hz);
		auto const dt = 1.0f / sample_rate;
		m_alpha = dt / (rc + dt);
	}

	float process(float in) {
		m_prev_out = m_alpha * in + (1.0f - m_alpha) * m_prev_out;
		return m_prev_out;
	}

  private:
	float m_prev_out{};
	float m_alpha{};
};

} // namespace juke
