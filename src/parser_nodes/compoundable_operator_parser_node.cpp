//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "compoundable_operator_parser_node.hpp"
#include "add_parser_node.hpp"
#include "subtract_parser_node.hpp"
#include "multiply_parser_node.hpp"
#include "divide_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Compoundable_Operator_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_add->dump(output);
}

void Compoundable_Operator_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_subtract->dump(output);
}

void Compoundable_Operator_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  m_multiply->dump(output);
}

void Compoundable_Operator_Parser_Node::Choice_4::dump(std::wostream& output) const
{
  m_divide->dump(output);
}

Compoundable_Operator_Parser_Node::Compoundable_Operator_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Compoundable_Operator_Parser_Node::Compoundable_Operator_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Compoundable_Operator_Parser_Node::Compoundable_Operator_Parser_Node(Choice_3&& choice)
  : m_choice(std::move(choice))
{
}

Compoundable_Operator_Parser_Node::Compoundable_Operator_Parser_Node(Choice_4&& choice)
  : m_choice(std::move(choice))
{
}

Compoundable_Operator_Parser_Node_Ptr
Compoundable_Operator_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_1 choice;

  choice.m_add = Add_Parser_Node::parse(input);
  if (choice.m_add == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Compoundable_Operator_Parser_Node>(std::move(choice));
}

Compoundable_Operator_Parser_Node_Ptr
Compoundable_Operator_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_2 choice;

  choice.m_subtract = Subtract_Parser_Node::parse(input);
  if (choice.m_subtract == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Compoundable_Operator_Parser_Node>(std::move(choice));
}

Compoundable_Operator_Parser_Node_Ptr
Compoundable_Operator_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_3 choice;

  choice.m_multiply = Multiply_Parser_Node::parse(input);
  if (choice.m_multiply == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Compoundable_Operator_Parser_Node>(std::move(choice));
}

Compoundable_Operator_Parser_Node_Ptr
Compoundable_Operator_Parser_Node::parse_choice_4(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_4 choice;

  choice.m_divide = Divide_Parser_Node::parse(input);
  if (choice.m_divide == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Compoundable_Operator_Parser_Node>(std::move(choice));
}

Compoundable_Operator_Parser_Node_Ptr
Compoundable_Operator_Parser_Node::parse(Token_Stream& input)
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

void Compoundable_Operator_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice){ choice.dump(output); }, m_choice );
}

void Compoundable_Operator_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_add->generate_cpp(state);
}

void Compoundable_Operator_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_subtract->generate_cpp(state);
}

void Compoundable_Operator_Parser_Node::Choice_3::generate_cpp(Scope_State_Ptr state) const
{
  m_multiply->generate_cpp(state);
}

void Compoundable_Operator_Parser_Node::Choice_4::generate_cpp(Scope_State_Ptr state) const
{
  m_divide->generate_cpp(state);
}

void Compoundable_Operator_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.generate_cpp(state); }, m_choice );
}
