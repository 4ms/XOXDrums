#pragma once
#include <algorithm>
#include <optional>

// CV from -10V to +10V is added to the knob such that 10V is equivalent to the full knob range
// The CV jack is normalized to 0V if not patched
constexpr inline float combineKnobBipolarCV(float knob_val, std::optional<float> cv_val) {
	float cvScale = cv_val.value_or(0.f) / 10.0f;
	float cvSum = knob_val + cvScale;
	return std::clamp(cvSum, 0.0f, 1.0f);
}

// CV at 0V has no effect on knob
static_assert(combineKnobBipolarCV(0.2f, 0.f) == 0.2f);

// Unplugged CV has no effect on knob
static_assert(combineKnobBipolarCV(0.2f, std::nullopt) == 0.2f);

// clamps at [0, 1]
static_assert(combineKnobBipolarCV(1.2f, 0.f) == 1.0f);

// 10V CV is equivalent to knob at 10%%
static_assert(combineKnobBipolarCV(0.f, 10.f) == 1.0f);

// 0V CV is equivalent to knob at 0%
static_assert(combineKnobBipolarCV(0.f, 0.f) == 0.f);

// -10V CV cancels out knob at 100%
static_assert(combineKnobBipolarCV(1.f, -10.f) == 0.f);

// -9V with knob at 100% is approx equal to knob at 10%
static_assert(combineKnobBipolarCV(1.f, -9.f) > 0.099999f);
static_assert(combineKnobBipolarCV(1.f, -9.f) < 0.100001f);
