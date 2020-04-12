//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "statement_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "break_parser_node.hpp"
#include "continue_parser_node.hpp"
#include "function_parser_node.hpp"
#include "function_return_parser_node.hpp"
#include "function_call_parser_node.hpp"
#include "loop_parser_node.hpp"
#include "conditional_parser_node.hpp"
#include "operation_parser_node.hpp"
#include "expression_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <vector>

void Statement_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  for (const auto& s: m_separators) { s->dump(output); }
  m_break->dump(output);
}

void Statement_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  for (const auto& s: m_separators) { s->dump(output); }
  m_continue->dump(output);
}

void Statement_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  for (const auto& s: m_separators) { s->dump(output); }
  m_function->dump(output);
}

void Statement_Parser_Node::Choice_4::dump(std::wostream& output) const
{
  for (const auto& s: m_separators) { s->dump(output); }
  m_function_call->dump(output);
}

void Statement_Parser_Node::Choice_5::dump(std::wostream& output) const
{
  for (const auto& s: m_separators) { s->dump(output); }
  m_function_return->dump(output);
}

void Statement_Parser_Node::Choice_6::dump(std::wostream& output) const
{
  for (const auto& s: m_separators) { s->dump(output); }
  m_loop->dump(output);
}

void Statement_Parser_Node::Choice_7::dump(std::wostream& output) const
{
  for (const auto& s: m_separators) { s->dump(output); }
  m_conditional->dump(output);
}

void Statement_Parser_Node::Choice_8::dump(std::wostream& output) const
{
  for (const auto& s: m_separators) { s->dump(output); }
  m_operation->dump(output);
}

void Statement_Parser_Node::Choice_9::dump(std::wostream& output) const
{
  for (const auto& s: m_separators) { s->dump(output); }
  m_expression->dump(output);
}

Statement_Parser_Node::Statement_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Statement_Parser_Node::Statement_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Statement_Parser_Node::Statement_Parser_Node(Choice_3&& choice)
  : m_choice(std::move(choice))
{
}

Statement_Parser_Node::Statement_Parser_Node(Choice_4&& choice)
  : m_choice(std::move(choice))
{
}

Statement_Parser_Node::Statement_Parser_Node(Choice_5&& choice)
  : m_choice(std::move(choice))
{
}

Statement_Parser_Node::Statement_Parser_Node(Choice_6&& choice)
  : m_choice(std::move(choice))
{
}

Statement_Parser_Node::Statement_Parser_Node(Choice_7&& choice)
  : m_choice(std::move(choice))
{
}

Statement_Parser_Node::Statement_Parser_Node(Choice_8&& choice)
  : m_choice(std::move(choice))
{
}

Statement_Parser_Node::Statement_Parser_Node(Choice_9&& choice)
  : m_choice(std::move(choice))
{
}

Statement_Parser_Node_Ptr
Statement_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  // zero or more separators
  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);

  choice.m_break = Break_Parser_Node::parse(input);
  if (choice.m_break == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Statement_Parser_Node>(std::move(choice));
}

Statement_Parser_Node_Ptr
Statement_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  // zero or more separators
  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);

  choice.m_continue = Continue_Parser_Node::parse(input);
  if (choice.m_continue == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Statement_Parser_Node>(std::move(choice));
}

Statement_Parser_Node_Ptr
Statement_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_3 choice;

  // zero or more separators
  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);

  choice.m_function = Function_Parser_Node::parse(input);
  if (choice.m_function == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Statement_Parser_Node>(std::move(choice));
}

Statement_Parser_Node_Ptr
Statement_Parser_Node::parse_choice_4(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_4 choice;

  // zero or more separators
  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);

  choice.m_function_call = Function_Call_Parser_Node::parse(input);
  if (choice.m_function_call == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Statement_Parser_Node>(std::move(choice));
}

Statement_Parser_Node_Ptr
Statement_Parser_Node::parse_choice_5(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_5 choice;

  // zero or more separators
  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);

  choice.m_function_return = Function_Return_Parser_Node::parse(input);
  if (choice.m_function_return == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Statement_Parser_Node>(std::move(choice));
}

Statement_Parser_Node_Ptr
Statement_Parser_Node::parse_choice_6(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_6 choice;

  // zero or more separators
  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);

  choice.m_loop = Loop_Parser_Node::parse(input);
  if (choice.m_loop == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Statement_Parser_Node>(std::move(choice));
}

Statement_Parser_Node_Ptr
Statement_Parser_Node::parse_choice_7(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_7 choice;

  // zero or more separators
  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);

  choice.m_conditional = Conditional_Parser_Node::parse(input);
  if (choice.m_conditional == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Statement_Parser_Node>(std::move(choice));
}

Statement_Parser_Node_Ptr
Statement_Parser_Node::parse_choice_8(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_8 choice;

  // zero or more separators
  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);

  choice.m_operation = Operation_Parser_Node::parse(input);
  if (choice.m_operation == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Statement_Parser_Node>(std::move(choice));
}

Statement_Parser_Node_Ptr
Statement_Parser_Node::parse_choice_9(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_9 choice;

  // zero or more separators
  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);

  choice.m_expression = Expression_Parser_Node::parse(input);
  if (choice.m_expression == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Statement_Parser_Node>(std::move(choice));
}

Statement_Parser_Node_Ptr
Statement_Parser_Node::parse(Token_Stream& input)
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

  node = parse_choice_7(input);
  if (node) {
    return node;
  }

  input.seekg(offset);

  node = parse_choice_8(input);
  if (node) {
    return node;
  }

  input.seekg(offset);

  node = parse_choice_9(input);
  if (node) {
    return node;
  }

  input.seekg(offset);

  return nullptr;
}

void Statement_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Statement_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_break->generate_cpp(state);
}

void Statement_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_continue->generate_cpp(state);
}

void Statement_Parser_Node::Choice_3::generate_cpp(Scope_State_Ptr state) const
{
  m_function->generate_cpp(state);
}

void Statement_Parser_Node::Choice_4::generate_cpp(Scope_State_Ptr state) const
{
  m_function_call->generate_cpp(state);
}

void Statement_Parser_Node::Choice_5::generate_cpp(Scope_State_Ptr state) const
{
  m_function_return->generate_cpp(state);
}

void Statement_Parser_Node::Choice_6::generate_cpp(Scope_State_Ptr state) const
{
  m_loop->generate_cpp(state);
}

void Statement_Parser_Node::Choice_7::generate_cpp(Scope_State_Ptr state) const
{
  m_conditional->generate_cpp(state);
}

void Statement_Parser_Node::Choice_8::generate_cpp(Scope_State_Ptr state) const
{
  m_operation->generate_cpp(state);
}

void Statement_Parser_Node::Choice_9::generate_cpp(Scope_State_Ptr state) const
{
  m_expression->generate_cpp(state);
}

void Statement_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);
  state->add_code(L";\n");
}



void Statement_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  (void)state;
}

void Statement_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  (void)state;
}

void Statement_Parser_Node::Choice_3::collect_variables(Scope_State_Ptr state) const
{
  m_function->collect_variables(state);
}

void Statement_Parser_Node::Choice_4::collect_variables(Scope_State_Ptr state) const
{
  m_function_call->collect_variables(state);
}

void Statement_Parser_Node::Choice_5::collect_variables(Scope_State_Ptr state) const
{
  m_function_return->collect_variables(state);
}

void Statement_Parser_Node::Choice_6::collect_variables(Scope_State_Ptr state) const
{
  m_loop->collect_variables(state);
}

void Statement_Parser_Node::Choice_7::collect_variables(Scope_State_Ptr state) const
{
  m_conditional->collect_variables(state);
}

void Statement_Parser_Node::Choice_8::collect_variables(Scope_State_Ptr state) const
{
  m_operation->collect_variables(state);
}

void Statement_Parser_Node::Choice_9::collect_variables(Scope_State_Ptr state) const
{
  m_expression->collect_variables(state);
}

void Statement_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.collect_variables(state); }, m_choice);
}
