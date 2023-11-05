#pragma once


struct Cooldown
{
  Cooldown(double duration) : duration(duration) {}

  /// <summary>
  /// Elapses the time for given delta time. If after that
  /// the cooldown is no longer active, return true and reset the cooldown.
  /// </summary>
  /// <param name="deltaTime"></param>
  /// <returns>True if the cooldown has reset and the guarded action may be taken</returns>
  bool trigger(double deltaTime)
  {
    elapse(deltaTime);
    if (!isActive())
    {
      reset();
      return true;
    }
    return false;
  }
  /// <summary>
  /// Resets the cooldown back to max duration
  /// </summary>
  void reset() { time = duration; }
  /// <summary>
  /// Removes the given dt from cooldowns remaining time.
  /// </summary>
  void elapse(double deltaTime)
  {
    if (time > 0.0)
      time -= deltaTime;
    if (time < 0.0)
      time = 0.0;
  }
  /// <returns>True if the cooldown is active (time not elapsed yet)</returns>
  bool isActive() const { return time > 0.0; }
  void setDuration(double newDuration) { duration = newDuration; }
  double getDuration() const { return duration; }
  double getRemaining() const { return time; }
private:
  double duration;
  double time = 0;
};
