//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "assignment_parser_node.hpp"

#include "parser_node.hpp"
#include "separator_parser_node.hpp"
#include "is_parser_node.hpp"
#include "assignable_parser_node.hpp"
#include "poetic_number_parser_node.hpp"
#include "poetic_string_parser_node.hpp"
#include "compoundable_operator_parser_node.hpp"
#include "target_parser_node.hpp"
#include "literal_parser_node.hpp"
#include "expression_parser_node.hpp"

#include <memory>
#include <istream>
#include <string>
#include <variant>

void Assignment_Parser_Node::Choice_1_1::dump(std::wostream& output) const
{
  m_literal->dump(output);
}

void Assignment_Parser_Node::Choice_1_2::dump(std::wostream& output) const
{
  m_poetic_number->dump(output);
}

void Assignment_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_assignable->dump(output);
  m_is->dump(output);
  for (const auto& s: m_separators) { s->dump(output); }
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Assignment_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_assignable->dump(output);
  for (const auto& s: m_separators) { s->dump(output); }
  output << m_keyword;
  m_poetic_string->dump(output);
}

void Assignment_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  output << m_keyword;
  for (const auto& s: m_separators) { s->dump(output); }
  m_expression->dump(output);
  m_target->dump(output);
}

void Assignment_Parser_Node::Choice_4::dump(std::wostream& output) const
{
  output << m_keyword_1;
  for (const auto& s: m_separators_1) { s->dump(output); }
  m_assignable->dump(output);
  for (const auto& s: m_separators_2) { s->dump(output); }
  output << m_keyword_2;
  m_compoundable_operator->dump(output);
  m_expression->dump(output);
}

void Assignment_Parser_Node::Choice_5::dump(std::wostream& output) const
{
  output << m_keyword_1;
  for (const auto& s: m_separators_1) { s->dump(output); }
  m_assignable->dump(output);
  for (const auto& s: m_separators_2) { s->dump(output); }
  output << m_keyword_2;
  for (const auto& s: m_separators_3) { s->dump(output); }
  m_expression->dump(output);
}

Assignment_Parser_Node::Assignment_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Assignment_Parser_Node::Assignment_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Assignment_Parser_Node::Assignment_Parser_Node(Choice_3&& choice)
  : m_choice(std::move(choice))
{
}

Assignment_Parser_Node::Assignment_Parser_Node(Choice_4&& choice)
  : m_choice(std::move(choice))
{
}

Assignment_Parser_Node::Assignment_Parser_Node(Choice_5&& choice)
  : m_choice(std::move(choice))
{
}

// assignable is _* (literal / poetic_number)
Assignment_Parser_Node_Ptr
Assignment_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_assignable = Assignable_Parser_Node::parse(input);
  if (choice.m_assignable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_is = Is_Parser_Node::parse(input);
  if (choice.m_is == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);

  auto tmp_offset = input.tellg();

  auto literal = Literal_Parser_Node::parse(input);
  if (literal) {
    choice.m_choice = Choice_1_1{ std::move(literal) };
  } else {
    input.seekg(tmp_offset);

    auto poetic_number = Poetic_Number_Parser_Node::parse(input);
    if (poetic_number == nullptr) {
      input.seekg(offset);
      return nullptr;
    }

    choice.m_choice = Choice_1_2{ std::move(poetic_number) };
  }

  return std::make_shared<Assignment_Parser_Node>(std::move(choice));
}

// / assignable _+ 'says 'i poetic_string
Assignment_Parser_Node_Ptr
Assignment_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_assignable = Assignable_Parser_Node::parse(input);
  if (choice.m_assignable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);
  if (choice.m_separators.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword = check_and_remove(input, L"says " );
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_poetic_string = Poetic_String_Parser_Node::parse(input);
  if (choice.m_poetic_string == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Assignment_Parser_Node>(std::move(choice));
}

// / 'put'i _+ expression target
Assignment_Parser_Node_Ptr
Assignment_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_3 choice;

  choice.m_keyword = check_and_remove(input, L"put");
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);
  if (choice.m_separators.empty()) {
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

  return std::make_shared<Assignment_Parser_Node>(std::move(choice));
}

// / 'let'i _+ assignable _+ 'be'i compoundable_operator expression
Assignment_Parser_Node_Ptr
Assignment_Parser_Node::parse_choice_4(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_4 choice;

  choice.m_keyword_1 = check_and_remove(input, L"let");
  if (choice.m_keyword_1.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separators_1 = parse_zero_or_more<Separator_Parser_Node>(input);
  if (choice.m_separators_1.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_assignable = Assignable_Parser_Node::parse(input);
  if (choice.m_assignable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separators_2 = parse_zero_or_more<Separator_Parser_Node>(input);
  if (choice.m_separators_2.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword_2 = check_and_remove(input, L"be");
  if (choice.m_keyword_2.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_compoundable_operator = Compoundable_Operator_Parser_Node::parse(input);
  if (choice.m_compoundable_operator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_expression = Expression_Parser_Node::parse(input);
  if (choice.m_expression == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Assignment_Parser_Node>(std::move(choice));
}

// / 'let'i _+ assignable _+ 'be'i _+ expression
Assignment_Parser_Node_Ptr
Assignment_Parser_Node:: parse_choice_5(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_5 choice;

  choice.m_keyword_1 = check_and_remove(input, L"let");
  if (choice.m_keyword_1.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separators_1 = parse_zero_or_more<Separator_Parser_Node>(input);
  if (choice.m_separators_1.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_assignable = Assignable_Parser_Node::parse(input);
  if (choice.m_assignable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separators_2 = parse_zero_or_more<Separator_Parser_Node>(input);
  if (choice.m_separators_2.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword_2 = check_and_remove(input, L"be");
  if (choice.m_keyword_2.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separators_3 = parse_zero_or_more<Separator_Parser_Node>(input);
  if (choice.m_separators_3.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_expression = Expression_Parser_Node::parse(input);
  if (choice.m_expression == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Assignment_Parser_Node>(std::move(choice));
}

Assignment_Parser_Node_Ptr
Assignment_Parser_Node::parse(Token_Stream& input)
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

void Assignment_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Assignment_Parser_Node::Choice_1_1::generate_cpp(Scope_State_Ptr state) const
{
  m_literal->generate_cpp(state);
}

void Assignment_Parser_Node::Choice_1_2::generate_cpp(Scope_State_Ptr state) const
{
  m_poetic_number->generate_cpp(state);
}

void Assignment_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code( L"/* a 1 */" );

  m_assignable->generate_cpp(state);

  //auto last_var = state->get_last_variable();

  state->add_code( L" = " );

  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);

  //state->set_last_variable(last_var);

  state->add_code( L";\n" );
}

void Assignment_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code( L"/* a 2 */" );

  m_assignable->generate_cpp(state);

  //auto last_var = state->get_last_variable();

  state->add_code( L" = " );

  m_poetic_string->generate_cpp(state);

  //state->set_last_variable(last_var);

  state->add_code( L";\n" );
}

void Assignment_Parser_Node::Choice_3::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code( L"/* a 3 */" );

  m_target->generate_cpp(state);

  //auto last_var = state->get_last_variable();

  state->add_code( L" = " );

  m_expression->generate_cpp(state);

  //state->set_last_variable(last_var);

  state->add_code( L";\n" );
}

void Assignment_Parser_Node::Choice_4::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code( L"/* a 4 */" );

  m_assignable->generate_cpp(state);

  state->add_code( L" = " );

  m_assignable->generate_cpp(state);

  //auto last_var = state->get_last_variable();

  m_compoundable_operator->generate_cpp(state);

  m_expression->generate_cpp(state);

  //state->set_last_variable(last_var);

  state->add_code( L";\n" );
}

void Assignment_Parser_Node::Choice_5::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code( L"/* a 5 */" );

  m_assignable->generate_cpp(state);

  //auto last_var = state->get_last_variable();

  state->add_code( L" = " );

  m_expression->generate_cpp(state);

  //state->set_last_variable(last_var);

  state->add_code( L";\n" );
}

void Assignment_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);
}

void Assignment_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_assignable->collect_variables(state);
}

void Assignment_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_assignable->collect_variables(state);
}

void Assignment_Parser_Node::Choice_3::collect_variables(Scope_State_Ptr state) const
{
  m_target->collect_variables(state);
  m_expression->collect_variables(state);
}

void Assignment_Parser_Node::Choice_4::collect_variables(Scope_State_Ptr state) const
{
  m_assignable->collect_variables(state);
  m_expression->collect_variables(state);
}

void Assignment_Parser_Node::Choice_5::collect_variables(Scope_State_Ptr state) const
{
  m_assignable->collect_variables(state);
  m_expression->collect_variables(state);
}

void Assignment_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.collect_variables(state); }, m_choice);
}
