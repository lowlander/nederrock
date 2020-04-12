//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "nor_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "or_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Nor_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_or->dump(output);
  m_separator_1->dump(output);
  output << m_keyword;
  m_separator_2->dump(output);
  m_nor->dump(output);
}

void Nor_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_or->dump(output);
}

Nor_Parser_Node::Nor_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Nor_Parser_Node::Nor_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Nor_Parser_Node_Ptr
Nor_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_or = Or_Parser_Node::parse(input);
  if (choice.m_or == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_1 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword = check_and_remove(input, L"nor");
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_2 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_nor = Nor_Parser_Node::parse(input);
  if (choice.m_nor == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Nor_Parser_Node>(std::move(choice));
}

Nor_Parser_Node_Ptr
Nor_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_or = Or_Parser_Node::parse(input);
  if (choice.m_or == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Nor_Parser_Node>(std::move(choice));
}

Nor_Parser_Node_Ptr
Nor_Parser_Node::parse(Token_Stream& input)
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

void Nor_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Nor_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"!(");
  m_or->generate_cpp(state);
  state->add_code(L" || ");
  m_nor->generate_cpp(state);
  state->add_code(L")");
}

void Nor_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_or->generate_cpp(state);
}

void Nor_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);
}

void Nor_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_or->collect_variables(state);
  m_nor->collect_variables(state);
}

void Nor_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_or->collect_variables(state);
}

void Nor_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.collect_variables(state); }, m_choice);
}
