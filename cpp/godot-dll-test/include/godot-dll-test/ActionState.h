#pragma once
#include <string>
#include <string_view>

class ActionState
{
public:
  ActionState(std::string_view name, bool inverse = false)
    : actionName{name}
    , inverse{inverse}
  {}
  ActionState()
    : actionName{ "" }
    , inverse{ false }
  {}
  virtual bool isActive() const;
  virtual bool isValid() const;
protected:
  std::string actionName;
  bool inverse;
};