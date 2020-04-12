//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_SCOPE_STATE_HPP
#define NEDERROCK_SRC_SCOPE_STATE_HPP

#include <string_view>
#include <string>
#include <sstream>
#include <set>
#include <memory>
#include <vector>

class Scope_State;
using Scope_State_Ptr = std::shared_ptr<Scope_State>;
using Scope_State_Vector = std::vector<Scope_State_Ptr>;

class Scope_State {
public:
  Scope_State();
  Scope_State(Scope_State_Ptr parent);

  void set_last_variable(std::wstring_view name);
  std::wstring get_last_variable();

  void add_variable(std::wstring_view name);
  void add_function(std::wstring_view name);
  void add_function_argument(std::wstring_view name);

  void add_code(std::wstring_view code);
  void add_pronoun_code();

  void set_expression_list_operator(std::wstring_view op);
  std::wstring get_expression_list_operator() const;

  std::wstring get_code() const;
private:
  bool is_function(const std::wstring& name) const;
  bool is_function_argument(const std::wstring& name) const;
  bool is_global(const std::wstring& name) const;
  bool is_variable(const std::wstring& name) const;
private:
  Scope_State_Ptr         m_parent;
  std::set<std::wstring>  m_variable_set;
  std::set<std::wstring>  m_function_set;
  std::set<std::wstring>  m_function_argument_set;
  std::wstring            m_last_variable;
  std::wstring            m_code;
  std::wstring            m_expression_list_operator;
};

#endif // NEDERROCK_SRC_SCOPE_STATE_HPP
