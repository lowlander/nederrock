//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "mutation_parser_node.hpp"
#include "mutator_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "expression_parser_node.hpp"
#include "target_parser_node.hpp"
#include "modifier_parser_node.hpp"
#include "assignable_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Mutation_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_mutator->dump(output);
  m_separator->dump(output);
  m_expression->dump(output);
  m_target->dump(output);
  if (m_modifier) { m_modifier->dump(output); }
}

void Mutation_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_mutator->dump(output);
  m_separator->dump(output);
  m_assignable->dump(output);
  if (m_modifier) { m_modifier->dump(output); }
}

Mutation_Parser_Node::Mutation_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Mutation_Parser_Node::Mutation_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Mutation_Parser_Node_Ptr
Mutation_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_mutator = Mutator_Parser_Node::parse(input);
  if (choice.m_mutator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator = Separator_Parser_Node::parse(input);
  if (choice.m_separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_expression = Expression_Parser_Node::parse(input);
  if (choice.m_expression == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_target = Target_Parser_Node::parse(input);
  if (choice.m_target == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_modifier = Modifier_Parser_Node::parse(input);

  return std::make_shared<Mutation_Parser_Node>(std::move(choice));
}

Mutation_Parser_Node_Ptr
Mutation_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_mutator = Mutator_Parser_Node::parse(input);
  if (choice.m_mutator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator = Separator_Parser_Node::parse(input);
  if (choice.m_separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_assignable = Assignable_Parser_Node::parse(input);
  if (choice.m_assignable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_modifier = Modifier_Parser_Node::parse(input);

  return std::make_shared<Mutation_Parser_Node>(std::move(choice));
}

Mutation_Parser_Node_Ptr
Mutation_Parser_Node::parse(Token_Stream& input)
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

void Mutation_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Mutation_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_target->generate_cpp(state);
  state->add_code(L"=");
  m_mutator->generate_cpp(state);
  state->add_code(L"(");
  m_expression->generate_cpp(state);
  if (m_modifier) {
    state->add_code(L",");
    m_modifier->generate_cpp(state);
  }
  state->add_code(L")");
}

void Mutation_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_assignable->generate_cpp(state);
  state->add_code(L"=");
  m_mutator->generate_cpp(state);
  state->add_code(L"(");
  m_assignable->generate_cpp(state);
  if (m_modifier) {
    state->add_code(L",");
    m_modifier->generate_cpp(state);
  }
  state->add_code(L")");
}

void Mutation_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);
}

void Mutation_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_target->collect_variables(state);
  m_expression->collect_variables(state);
}

void Mutation_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_assignable->collect_variables(state);
  m_assignable->collect_variables(state);
}

void Mutation_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.collect_variables(state); }, m_choice);
}
