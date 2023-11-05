#pragma once
#include <ActionState.h>
struct KeyConfig
{
  static const KeyConfig& get()
  {
    static KeyConfig singleton;
    return singleton;
  }

  const ActionState SHOOT{ "game_shoot", false };
  const ActionState ACCELERATE{ "game_accelerate" };
private:
  KeyConfig() {}
};