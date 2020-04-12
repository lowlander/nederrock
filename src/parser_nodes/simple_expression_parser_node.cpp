//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "simple_expression_parser_node.hpp"
#include "function_call_parser_node.hpp"
#include "constant_parser_node.hpp"
#include "lookup_parser_node.hpp"
#include "literal_parser_node.hpp"
#include "pronoun_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Simple_Expression_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_function_call->dump(output);
}

void Simple_Expression_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_constant->dump(output);
}

void Simple_Expression_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  m_lookup->dump(output);
}

void Simple_Expression_Parser_Node::Choice_4::dump(std::wostream& output) const
{
  m_literal->dump(output);
}

void Simple_Expression_Parser_Node::Choice_5::dump(std::wostream& output) const
{
  m_pronoun->dump(output);
}

Simple_Expression_Parser_Node::Simple_Expression_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Simple_Expression_Parser_Node::Simple_Expression_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Simple_Expression_Parser_Node::Simple_Expression_Parser_Node(Choice_3&& choice)
  : m_choice(std::move(choice))
{
}

Simple_Expression_Parser_Node::Simple_Expression_Parser_Node(Choice_4&& choice)
  : m_choice(std::move(choice))
{
}

Simple_Expression_Parser_Node::Simple_Expression_Parser_Node(Choice_5&& choice)
  : m_choice(std::move(choice))
{
}

Simple_Expression_Parser_Node_Ptr
Simple_Expression_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_function_call= Function_Call_Parser_Node::parse(input);
  if (choice.m_function_call == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Simple_Expression_Parser_Node>(std::move(choice));
}

Simple_Expression_Parser_Node_Ptr
Simple_Expression_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_constant = Constant_Parser_Node::parse(input);
  if (choice.m_constant == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Simple_Expression_Parser_Node>(std::move(choice));
}

Simple_Expression_Parser_Node_Ptr
Simple_Expression_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_3 choice;

  choice.m_lookup = Lookup_Parser_Node::parse(input);
  if (choice.m_lookup == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Simple_Expression_Parser_Node>(std::move(choice));
}

Simple_Expression_Parser_Node_Ptr
Simple_Expression_Parser_Node::parse_choice_4(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_4 choice;

  choice.m_literal = Literal_Parser_Node::parse(input);
  if (choice.m_literal == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Simple_Expression_Parser_Node>(std::move(choice));
}

Simple_Expression_Parser_Node_Ptr
Simple_Expression_Parser_Node::parse_choice_5(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_5 choice;

  choice.m_pronoun = Pronoun_Parser_Node::parse(input);
  if (choice.m_pronoun == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Simple_Expression_Parser_Node>(std::move(choice));
}

Simple_Expression_Parser_Node_Ptr
Simple_Expression_Parser_Node::parse(Token_Stream& input)
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

  node = parse_choice_5(input);
  if (node) {
    return node;
  }

  input.seekg(offset);
  return nullptr;
}

void Simple_Expression_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice){ choice.dump(output); }, m_choice );
}

void Simple_Expression_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_function_call->generate_cpp(state);
}

void Simple_Expression_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_constant->generate_cpp(state);
}

void Simple_Expression_Parser_Node::Choice_3::generate_cpp(Scope_State_Ptr state) const
{
  m_lookup->generate_cpp(state);
}

void Simple_Expression_Parser_Node::Choice_4::generate_cpp(Scope_State_Ptr state) const
{
  m_literal->generate_cpp(state);
}

void Simple_Expression_Parser_Node::Choice_5::generate_cpp(Scope_State_Ptr state) const
{
  m_pronoun->generate_cpp(state);
}

void Simple_Expression_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.generate_cpp(state); }, m_choice );
}


void Simple_Expression_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_function_call->collect_variables(state);
}

void Simple_Expression_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  (void)state;
}

void Simple_Expression_Parser_Node::Choice_3::collect_variables(Scope_State_Ptr state) const
{
  m_lookup->collect_variables(state);
}

void Simple_Expression_Parser_Node::Choice_4::collect_variables(Scope_State_Ptr state) const
{
  (void)state;
}

void Simple_Expression_Parser_Node::Choice_5::collect_variables(Scope_State_Ptr state) const
{
  (void)state;
}

void Simple_Expression_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.collect_variables(state); }, m_choice );
}
