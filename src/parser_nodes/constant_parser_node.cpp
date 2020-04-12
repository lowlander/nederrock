//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "constant_parser_node.hpp"

#include "parser_node.hpp"
#include "null_parser_node.hpp"
#include "true_parser_node.hpp"
#include "false_parser_node.hpp"
#include "mysterious_parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Constant_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_null->dump(output);
}

void Constant_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_true->dump(output);
}

void Constant_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  m_false->dump(output);
}

void Constant_Parser_Node::Choice_4::dump(std::wostream& output) const
{
  m_mysterious->dump(output);
}

Constant_Parser_Node::Constant_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Constant_Parser_Node::Constant_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Constant_Parser_Node::Constant_Parser_Node(Choice_3&& choice)
  : m_choice(std::move(choice))
{
}

Constant_Parser_Node::Constant_Parser_Node(Choice_4&& choice)
  : m_choice(std::move(choice))
{
}

Constant_Parser_Node_Ptr
Constant_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_1 choice;

  choice.m_null = Null_Parser_Node::parse(input);
  if (choice.m_null == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Constant_Parser_Node>(std::move(choice));
}

Constant_Parser_Node_Ptr
Constant_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_2 choice;

  choice.m_true = True_Parser_Node::parse(input);
  if (choice.m_true == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Constant_Parser_Node>(std::move(choice));
}

Constant_Parser_Node_Ptr
Constant_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_3 choice;

  choice.m_false = False_Parser_Node::parse(input);
  if (choice.m_false == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Constant_Parser_Node>(std::move(choice));
}

Constant_Parser_Node_Ptr
Constant_Parser_Node::parse_choice_4(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_4 choice;

  choice.m_mysterious = Mysterious_Parser_Node::parse(input);
  if (choice.m_mysterious == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Constant_Parser_Node>(std::move(choice));
}

Constant_Parser_Node_Ptr
Constant_Parser_Node::parse(Token_Stream& input)
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

void Constant_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Constant_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_null->generate_cpp(state);
}

void Constant_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_true->generate_cpp(state);
}

void Constant_Parser_Node::Choice_3::generate_cpp(Scope_State_Ptr state) const
{
  m_false->generate_cpp(state);
}

void Constant_Parser_Node::Choice_4::generate_cpp(Scope_State_Ptr state) const
{
  m_mysterious->generate_cpp(state);
}

void Constant_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);
}
