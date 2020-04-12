//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "variable_list_parser_node.hpp"
#include "variable_list_separator_parser_node.hpp"
#include "variable_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>
#include <string>

void Variable_List_Parser_Node::Choice_1::dump(std::wostream& output) const
{
    m_variable->dump(output);
    m_variable_list_separator->dump(output);
    m_variable_list->dump(output);
}

void Variable_List_Parser_Node::Choice_2::dump(std::wostream& output) const
{
    m_variable->dump(output);
}

Variable_List_Parser_Node::Variable_List_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Variable_List_Parser_Node::Variable_List_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Variable_List_Parser_Node_Ptr
Variable_List_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_variable = Variable_Parser_Node::parse(input);
  if (choice.m_variable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_variable_list_separator = Variable_List_Separator_Parser_Node::parse(input);
  if (choice.m_variable_list_separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_variable_list = Variable_List_Parser_Node::parse(input);
  if (choice.m_variable_list == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Variable_List_Parser_Node>(std::move(choice));
}

Variable_List_Parser_Node_Ptr
Variable_List_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_variable = Variable_Parser_Node::parse(input);
  if (choice.m_variable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

    return std::make_shared<Variable_List_Parser_Node>(std::move(choice));
}

Variable_List_Parser_Node_Ptr
Variable_List_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

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

  return nullptr;
}

void Variable_List_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output] (auto&& choice) { choice.dump(output); }, m_choice);
}

void Variable_List_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  auto name = m_variable->to_string();
  state->add_code(L"rockstar::Variable ");
  state->add_code(name);
  state->add_code(L", ");
  m_variable_list->generate_cpp(state);
}

void Variable_List_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  auto name = m_variable->to_string();
  state->add_code(L"rockstar::Variable ");
  state->add_code(name);
}

void Variable_List_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state] (auto&& choice) { choice.generate_cpp(state); }, m_choice);
}

size_t Variable_List_Parser_Node::Choice_1::get_variable_count() const
{
  return 1 + m_variable_list->get_variable_count();
}

size_t Variable_List_Parser_Node::Choice_2::get_variable_count() const
{
  return 1;
}

size_t Variable_List_Parser_Node::get_variable_count() const
{
  return std::visit([] (auto&& choice) -> size_t { return choice.get_variable_count(); }, m_choice);
}

void Variable_List_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  auto name = m_variable->to_string();
  state->add_function_argument(name);

  m_variable_list->collect_variables(state);
}

void Variable_List_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  auto name = m_variable->to_string();

  state->add_function_argument(name);
}

void Variable_List_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state] (auto&& choice) { choice.collect_variables(state); }, m_choice);
}
