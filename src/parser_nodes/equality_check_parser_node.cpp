//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "equality_check_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "eq_parser_node.hpp"
#include "not_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

void Equality_Check_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_not->dump(output);
  m_separator_1->dump(output);
  m_eq->dump(output);
  m_separator_2->dump(output);
  m_equality_check->dump(output);
}

void Equality_Check_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_not->dump(output);
}

Equality_Check_Parser_Node::Equality_Check_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Equality_Check_Parser_Node::Equality_Check_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Equality_Check_Parser_Node_Ptr Equality_Check_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_not = Not_Parser_Node::parse(input);
  if (choice.m_not == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_1 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_eq = Eq_Parser_Node::parse(input);
  if (choice.m_eq == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_2 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_equality_check = Equality_Check_Parser_Node::parse(input);
  if (choice.m_equality_check == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Equality_Check_Parser_Node>(std::move(choice));
}

Equality_Check_Parser_Node_Ptr Equality_Check_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_not = Not_Parser_Node::parse(input);
  if (choice.m_not == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Equality_Check_Parser_Node>(std::move(choice));
}

Equality_Check_Parser_Node_Ptr Equality_Check_Parser_Node::parse(Token_Stream& input)
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

void Equality_Check_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Equality_Check_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_not->generate_cpp(state);
  m_eq->generate_cpp(state);
  m_equality_check->generate_cpp(state);
}

void Equality_Check_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_not->generate_cpp(state);
}

void Equality_Check_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);
}

void Equality_Check_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_not->collect_variables(state);
  m_equality_check->collect_variables(state);
}

void Equality_Check_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_not->collect_variables(state);
}

void Equality_Check_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.collect_variables(state); }, m_choice);
}
