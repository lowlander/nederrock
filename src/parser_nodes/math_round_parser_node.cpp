//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "math_round_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "variable_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>
#include <string>

void Math_Round_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  output << m_keyword_1;
  m_separator_1->dump(output);
  m_variable->dump(output);
  m_separator_2->dump(output);
  output << m_keyword_2;
}

void Math_Round_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  output << m_keyword_1;
  m_separator_1->dump(output);
  output << m_keyword_2;
  m_separator_2->dump(output);
  m_variable->dump(output);
}

Math_Round_Parser_Node::Math_Round_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Math_Round_Parser_Node::Math_Round_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Math_Round_Parser_Node_Ptr
Math_Round_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_keyword_1 = check_and_remove(input, L"turn");
  if (choice.m_keyword_1.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_1 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_variable = Variable_Parser_Node::parse(input);
  if (choice.m_variable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_2 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword_2 = check_and_remove(input, { L"round", L"around"} );
  if (choice.m_keyword_2.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Math_Round_Parser_Node>(std::move(choice));
}

Math_Round_Parser_Node_Ptr
Math_Round_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_keyword_1 = check_and_remove(input, L"turn");
  if (choice.m_keyword_1.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_1 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword_2 = check_and_remove(input, { L"round", L"around" } );
  if (choice.m_keyword_2.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_2 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_variable = Variable_Parser_Node::parse(input);
  if (choice.m_variable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Math_Round_Parser_Node>(std::move(choice));
}


Math_Round_Parser_Node_Ptr
Math_Round_Parser_Node::parse(Token_Stream& input)
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

void Math_Round_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Math_Round_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"rockstar::math_round(");
  m_variable->generate_cpp(state);
  state->add_code(L")");
}

void Math_Round_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"rockstar::math_round(");
  m_variable->generate_cpp(state);
  state->add_code(L")");
}

void Math_Round_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);
}

void Math_Round_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_variable->collect_variables(state);
}

void Math_Round_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_variable->collect_variables(state);
}

void Math_Round_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.collect_variables(state); }, m_choice);
}
