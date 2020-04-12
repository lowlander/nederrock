//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "variable_parser_node.hpp"
#include "common_variable_parser_node.hpp"
#include "proper_variable_parser_node.hpp"
#include "pronoun_parser_node.hpp"
#include "simple_variable_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Variable_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_common_variable->dump(output);
}

std::wstring Variable_Parser_Node::Choice_1::to_string() const
{
  return m_common_variable->to_string();
}

std::wstring Variable_Parser_Node::Choice_1::get_name() const
{
  return m_common_variable->to_string();
}

void Variable_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_proper_variable->dump(output);
}

std::wstring Variable_Parser_Node::Choice_2::to_string() const
{
  return m_proper_variable->to_string();
}

std::wstring Variable_Parser_Node::Choice_2::get_name() const
{
  return m_proper_variable->to_string();
}

void Variable_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  m_pronoun->dump(output);
}

std::wstring Variable_Parser_Node::Choice_3::to_string() const
{
  return m_pronoun->to_string();
}

std::wstring Variable_Parser_Node::Choice_3::get_name() const
{
  return {};
}

void Variable_Parser_Node::Choice_4::dump(std::wostream& output) const
{
  m_simple_variable->dump(output);
}

std::wstring Variable_Parser_Node::Choice_4::to_string() const
{
  return m_simple_variable->to_string();
}

std::wstring Variable_Parser_Node::Choice_4::get_name() const
{
  return m_simple_variable->to_string();
}

Variable_Parser_Node::Variable_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Variable_Parser_Node::Variable_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Variable_Parser_Node::Variable_Parser_Node(Choice_3&& choice)
  : m_choice(std::move(choice))
{
}

Variable_Parser_Node::Variable_Parser_Node(Choice_4&& choice)
  : m_choice(std::move(choice))
{
}

Variable_Parser_Node_Ptr
Variable_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_1 choice;

  choice.m_common_variable = Common_Variable_Parser_Node::parse(input);
  if (choice.m_common_variable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Variable_Parser_Node>(std::move(choice));
}

Variable_Parser_Node_Ptr
Variable_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_2 choice;

  choice.m_proper_variable = Proper_Variable_Parser_Node::parse(input);
  if (choice.m_proper_variable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Variable_Parser_Node>(std::move(choice));
}

Variable_Parser_Node_Ptr
Variable_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_3 choice;

  choice.m_pronoun = Pronoun_Parser_Node::parse(input);
  if (choice.m_pronoun == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Variable_Parser_Node>(std::move(choice));
}

Variable_Parser_Node_Ptr
Variable_Parser_Node::parse_choice_4(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_4 choice;

  choice.m_simple_variable = Simple_Variable_Parser_Node::parse(input);
  if (choice.m_simple_variable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Variable_Parser_Node>(std::move(choice));
}

Variable_Parser_Node_Ptr
Variable_Parser_Node::parse(Token_Stream& input)
{
  auto offset =input.tellg();

  auto node = parse_choice_1(input);
  if (node) {
    return node;
  }

  input.seekg(offset);

  node = parse_choice_2(input);
  if (node) {
    return node;
  }

  input.seekg(offset);

  node = parse_choice_3(input);
  if (node) {
    return node;
  }

  input.seekg(offset);

  node = parse_choice_4(input);
  if (node) {
    return node;
  }

  input.seekg(offset);
  return nullptr;
}

void Variable_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice){ choice.dump(output); }, m_choice );
}

std::wstring Variable_Parser_Node::to_string() const
{
  auto name = std::visit([](auto&& choice) -> std::wstring { return choice.to_string(); }, m_choice );
  return escape_variable_name(name);
}

std::wstring Variable_Parser_Node::get_name() const
{
  auto name = std::visit([](auto&& choice) -> std::wstring { return choice.get_name(); }, m_choice );
  return escape_variable_name(name);
}

void Variable_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  auto name = escape_variable_name(m_common_variable->to_string());

  state->add_code(name);
}

void Variable_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  auto name = escape_variable_name(m_proper_variable->to_string());

  state->add_code(name);
}

void Variable_Parser_Node::Choice_3::generate_cpp(Scope_State_Ptr state) const
{
  state->add_pronoun_code();
}

void Variable_Parser_Node::Choice_4::generate_cpp(Scope_State_Ptr state) const
{
  auto name = escape_variable_name(m_simple_variable->to_string());

  state->add_code(name);
}

void Variable_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice );
}


void Variable_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  auto name = escape_variable_name(m_common_variable->to_string());

  state->add_variable(name);
}

void Variable_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  auto name = escape_variable_name(m_proper_variable->to_string());

  state->add_variable(name);
}

void Variable_Parser_Node::Choice_3::collect_variables(Scope_State_Ptr state) const
{
  (void)state;
}

void Variable_Parser_Node::Choice_4::collect_variables(Scope_State_Ptr state) const
{
  auto name = escape_variable_name(m_simple_variable->to_string());

  state->add_variable(name);
}

void Variable_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.collect_variables(state); }, m_choice );
}
