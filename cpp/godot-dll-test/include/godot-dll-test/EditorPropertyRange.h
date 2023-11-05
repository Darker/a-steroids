#pragma once
#include <godot_cpp/variant/string.hpp>

struct EditorPropertyRange
{
  EditorPropertyRange() = default;
  
  EditorPropertyRange(
    double minRange,
    double maxRange,
    double rangeStep = 1.0,
    bool exponential = false,
    const godot::String& unitSuffix = ""
  )
    : min(minRange < maxRange ? minRange : maxRange)
    , max(minRange > maxRange ? minRange : maxRange)
    , step(rangeStep)
    , exp(exponential)
    , suffix{unitSuffix}
  {}
  EditorPropertyRange(
    double minRange,
    double maxRange,
    const godot::String& unitSuffix = ""
  )
    : min(minRange < maxRange ? minRange : maxRange)
    , max(minRange > maxRange ? minRange : maxRange)
    , step(1.0)
    , exp(false)
    , suffix{ unitSuffix }
  {}

  double min = std::numeric_limits<double>::quiet_NaN();
  double max = std::numeric_limits<double>::quiet_NaN();
  double step = 1.0;
  bool exp = false;
  godot::String suffix = "";

  /*constexpr */bool isValid() const { return !std::isnan(min) && !std::isnan(max); }
};