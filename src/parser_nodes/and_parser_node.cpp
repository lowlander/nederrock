//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "and_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "equality_check_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>
#include <variant>

void And_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_equality->dump(output);
  m_separator_1->dump(output);
  output << m_keyword;
  m_separator_2->dump(output);
  m_and->dump(output);
}

void And_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_equality->dump(output);
}

And_Parser_Node::And_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

And_Parser_Node::And_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

And_Parser_Node_Ptr And_Parser_Node::And_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_equality = Equality_Check_Parser_Node::parse(input);
  if (choice.m_equality == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_1 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword = check_and_remove(input, L"and");
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_2 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_and = And_Parser_Node::parse(input);
  if (choice.m_and == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<And_Parser_Node>(std::move(choice));
}

And_Parser_Node_Ptr And_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_equality = Equality_Check_Parser_Node::parse(input);
  if (choice.m_equality == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<And_Parser_Node>(std::move(choice));
}

And_Parser_Node_Ptr And_Parser_Node::parse(Token_Stream& input)
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

void And_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void And_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"(");
  m_equality->generate_cpp(state);
  state->add_code(L"&&");
  m_and->generate_cpp(state);
  state->add_code(L")");
}

void And_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_equality->generate_cpp(state);
}

void And_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);
}

void And_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_equality->collect_variables(state);
  m_and->collect_variables(state);
}

void And_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_equality->collect_variables(state);
}

void And_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.collect_variables(state); }, m_choice);
}
