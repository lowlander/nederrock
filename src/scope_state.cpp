//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "scope_state.hpp"

#include <string_view>
#include <string>
#include <sstream>

Scope_State::Scope_State()
{
}

Scope_State::Scope_State(Scope_State_Ptr parent)
  : m_parent(parent)
{
}

void Scope_State::set_last_variable(std::wstring_view name)
{
  m_last_variable = name;
}

std::wstring Scope_State::get_last_variable()
{
  return m_last_variable;
}

void Scope_State::add_variable(std::wstring_view name)
{
  m_variable_set.insert(std::wstring(name));
}

void Scope_State::add_function(std::wstring_view name)
{
  m_function_set.insert(std::wstring(name));
}

void Scope_State::add_function_argument(std::wstring_view name)
{
  m_function_argument_set.insert(std::wstring(name));
}

void Scope_State::add_code(std::wstring_view code)
{
  m_code += code;
}

void Scope_State::add_pronoun_code()
{
  m_code += m_last_variable;
}

bool Scope_State::is_function(const std::wstring& name) const
{
  if (m_function_set.find(name) != m_function_set.end()) {
    return true;
  }

  if (m_parent) {
    return m_parent->is_function(name);
  }

  return false;
}

bool Scope_State::is_function_argument(const std::wstring& name) const
{
  if (m_parent == nullptr) {
    return false;
  }

  if (m_parent->m_function_argument_set.find(name) != m_parent->m_function_argument_set.end()) {
    return true;
  }

  return false;
}

bool Scope_State::is_variable(const std::wstring& name) const
{
  if (m_variable_set.find(name) != m_variable_set.end()) {
    return true;
  }

  return false;
}

bool Scope_State::is_global(const std::wstring& name) const
{
  if (m_parent == nullptr) {
    return false;
  }

  if (m_parent->is_function(name) || m_parent->is_global(name) || m_parent->is_function_argument(name) || m_parent->is_variable(name)) {
    return true;
  }

  return false;
}

std::wstring Scope_State::get_code() const
{
  std::wstring res;

  res += L"/* VARS BEGIN */\n";

  for (const auto& v: m_variable_set) {
    if (!is_function(v) && !is_function_argument(v) && !is_global(v)) {
      res += L"rockstar::Variable ";
      res += v;
      res += L";\n";
    }
  }

  res += L"/* VARS END */\n";

  res += L"/* CODE START */\n";

  res += m_code;

  res += L"/* CODE END */\n";

  return res;
}

void Scope_State::set_expression_list_operator(std::wstring_view op)
{
  m_expression_list_operator = op;
}

std::wstring Scope_State::get_expression_list_operator() const
{
  return m_expression_list_operator;
}
