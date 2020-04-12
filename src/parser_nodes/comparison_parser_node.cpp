//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "comparison_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "comparator_parser_node.hpp"
#include "arithmetic_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>
#include <variant>

void Comparison_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_arithmetic->dump(output);
  m_separator_1->dump(output);
  m_comparator->dump(output);
  m_separator_2->dump(output);
  m_comparison->dump(output);
}

void Comparison_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_arithmetic->dump(output);
}

Comparison_Parser_Node::Comparison_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Comparison_Parser_Node::Comparison_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Comparison_Parser_Node_Ptr
Comparison_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_arithmetic = Arithmetic_Parser_Node::parse(input);
  if (choice.m_arithmetic == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_1 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_comparator = Comparator_Parser_Node::parse(input);
  if (choice.m_comparator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_2 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_comparison = Comparison_Parser_Node::parse(input);
  if (choice.m_comparison == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Comparison_Parser_Node>(std::move(choice));
}

Comparison_Parser_Node_Ptr
Comparison_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_arithmetic = Arithmetic_Parser_Node::parse(input);
  if (choice.m_arithmetic == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Comparison_Parser_Node>(std::move(choice));
}

Comparison_Parser_Node_Ptr
Comparison_Parser_Node::parse(Token_Stream& input)
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

void Comparison_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice );
}

void Comparison_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_arithmetic->generate_cpp(state);
  m_comparator->generate_cpp(state);
  m_comparison->generate_cpp(state);
}

void Comparison_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_arithmetic->generate_cpp(state);
}

void Comparison_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice );
}

void Comparison_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_arithmetic->collect_variables(state);
  m_comparison->collect_variables(state);
}

void Comparison_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_arithmetic->collect_variables(state);
}

void Comparison_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.collect_variables(state); }, m_choice );
}
