#pragma once
#include <stack>
#include <functional>

#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#pragma warning( pop )

template <typename TNode>
const TNode* findChild(const godot::Node* parent, const godot::String& pattern, bool recursive = true)
{
  return godot::Object::cast_to<TNode>(parent->find_child(pattern, recursive, false));
}

template <typename TNode>
TNode* findChild(godot::Node* parent, const godot::String& pattern, bool recursive = true)
{
  
  return godot::Object::cast_to<TNode>(parent->find_child(pattern, recursive, false));
  //using namespace godot;
  //std::stack<Node*> childStack;
  //childStack.push(parent);
  //while (!childStack.empty())
  //{
  //  Node* node = childStack.pop();
  //  const auto max = node->get_child_count();
  //  for (int32_t i = 0; i < max; ++i)
  //  {
  //    Node* child = node->get_child(i);
  //    if (child->get_name() == name)
  //    {

  //    }
  //    if (recursive)
  //    {
  //      childStack.push(child);
  //    }
  //  }
  //}
}
template <typename TNode>
using MutableChildCb = std::function<void(TNode*)>;
template <typename TNode>
void ifChild(godot::Node* parent, const godot::String& pattern, const MutableChildCb<TNode>& cb, bool recursive = true)
{
  TNode* node = findChild<TNode>(parent, pattern, recursive);
  if (node != nullptr)
  {
    cb(node);
  }
}

/*
* Returns pointer to given type if type and name match
*/
template <typename TNode>
TNode* checkChild(godot::Node* child, const godot::StringName& name)
{
  if (child->get_name() == name)
  {
    return godot::Object::cast_to<TNode>(child);
  }
  return nullptr;
}

