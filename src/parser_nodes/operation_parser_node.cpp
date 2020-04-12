//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "operation_parser_node.hpp"
#include "readline_parser_node.hpp"
#include "output_parser_node.hpp"
#include "crement_parser_node.hpp"
#include "mutation_parser_node.hpp"
#include "assignment_parser_node.hpp"
#include "rounding_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Operation_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_readline->dump(output);
}

void Operation_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_output->dump(output);
}

void Operation_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  m_crement->dump(output);
}

void Operation_Parser_Node::Choice_4::dump(std::wostream& output) const
{
  m_mutation->dump(output);
}

void Operation_Parser_Node::Choice_5::dump(std::wostream& output) const
{
  m_assignment->dump(output);
}

void Operation_Parser_Node::Choice_6::dump(std::wostream& output) const
{
  m_rounding->dump(output);
}

Operation_Parser_Node::Operation_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Operation_Parser_Node::Operation_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Operation_Parser_Node::Operation_Parser_Node(Choice_3&& choice)
  : m_choice(std::move(choice))
{
}

Operation_Parser_Node::Operation_Parser_Node(Choice_4&& choice)
  : m_choice(std::move(choice))
{
}

Operation_Parser_Node::Operation_Parser_Node(Choice_5&& choice)
  : m_choice(std::move(choice))
{
}

Operation_Parser_Node::Operation_Parser_Node(Choice_6&& choice)
  : m_choice(std::move(choice))
{
}

Operation_Parser_Node_Ptr
Operation_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_readline = Readline_Parser_Node::parse(input);
  if (choice.m_readline == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Operation_Parser_Node>(std::move(choice));
}

Operation_Parser_Node_Ptr
Operation_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_output = Output_Parser_Node::parse(input);
  if (choice.m_output == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Operation_Parser_Node>(std::move(choice));
}


Operation_Parser_Node_Ptr
Operation_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_3 choice;

  choice.m_crement = Crement_Parser_Node::parse(input);
  if (choice.m_crement == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Operation_Parser_Node>(std::move(choice));
}

Operation_Parser_Node_Ptr
Operation_Parser_Node::parse_choice_4(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_4 choice;

  choice.m_mutation = Mutation_Parser_Node::parse(input);
  if (choice.m_mutation == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Operation_Parser_Node>(std::move(choice));
}

Operation_Parser_Node_Ptr
Operation_Parser_Node::parse_choice_5(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_5 choice;

  choice.m_assignment = Assignment_Parser_Node::parse(input);
  if (choice.m_assignment == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Operation_Parser_Node>(std::move(choice));
}

Operation_Parser_Node_Ptr
Operation_Parser_Node::parse_choice_6(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_6 choice;

  choice.m_rounding = Rounding_Parser_Node::parse(input);
  if (choice.m_rounding == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Operation_Parser_Node>(std::move(choice));
}

Operation_Parser_Node_Ptr
Operation_Parser_Node::parse(Token_Stream& input)
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

  node = parse_choice_6(input);
  if (node) {
    return node;
  }

  input.seekg(offset);

  return nullptr;
}

void Operation_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Operation_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_readline->generate_cpp(state);
}

void Operation_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_output->generate_cpp(state);
}

void Operation_Parser_Node::Choice_3::generate_cpp(Scope_State_Ptr state) const
{
  m_crement->generate_cpp(state);
}

void Operation_Parser_Node::Choice_4::generate_cpp(Scope_State_Ptr state) const
{
  m_mutation->generate_cpp(state);
}

void Operation_Parser_Node::Choice_5::generate_cpp(Scope_State_Ptr state) const
{
  m_assignment->generate_cpp(state);
}

void Operation_Parser_Node::Choice_6::generate_cpp(Scope_State_Ptr state) const
{
  m_rounding->generate_cpp(state);
}

void Operation_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);
}

void Operation_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_readline->collect_variables(state);
}

void Operation_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_output->collect_variables(state);
}

void Operation_Parser_Node::Choice_3::collect_variables(Scope_State_Ptr state) const
{
  m_crement->collect_variables(state);
}

void Operation_Parser_Node::Choice_4::collect_variables(Scope_State_Ptr state) const
{
  m_mutation->collect_variables(state);
}

void Operation_Parser_Node::Choice_5::collect_variables(Scope_State_Ptr state) const
{
  m_assignment->collect_variables(state);
}

void Operation_Parser_Node::Choice_6::collect_variables(Scope_State_Ptr state) const
{
  m_rounding->collect_variables(state);
}

void Operation_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.collect_variables(state); }, m_choice);
}
