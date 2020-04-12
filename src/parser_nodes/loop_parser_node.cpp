//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "loop_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "expression_parser_node.hpp"
#include "loopable_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>
#include <string>

void Loop_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  output << m_keyword;
  m_separator->dump(output);
  m_expression->dump(output);
  m_loopable->dump(output);
}

void Loop_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  output << m_keyword;
  m_separator->dump(output);
  m_expression->dump(output);
  m_loopable->dump(output);
}

Loop_Parser_Node::Loop_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Loop_Parser_Node::Loop_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Loop_Parser_Node_Ptr
Loop_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_keyword = check_and_remove(input, L"while");
  if (choice.m_keyword.empty()) {
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

  choice.m_loopable = Loopable_Parser_Node::parse(input);
  if (choice.m_loopable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Loop_Parser_Node>(std::move(choice));
}

Loop_Parser_Node_Ptr
Loop_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_keyword = check_and_remove(input, L"until");
  if (choice.m_keyword.empty()) {
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

  choice.m_loopable = Loopable_Parser_Node::parse(input);
  if (choice.m_loopable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Loop_Parser_Node>(std::move(choice));
}

Loop_Parser_Node_Ptr
Loop_Parser_Node::parse(Token_Stream& input)
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

void Loop_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice );
}


void Loop_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"while (");
  m_expression->generate_cpp(state);
  state->add_code(L") {\n");
  m_loopable->generate_cpp(state);
  state->add_code(L"} /* end while loop */\n");
}

void Loop_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"while (!(");
  m_expression->generate_cpp(state);
  state->add_code(L")) {\n");
  m_loopable->generate_cpp(state);
  state->add_code(L"} /* end until loop */\n");
}

void Loop_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice );
}

void Loop_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_expression->collect_variables(state);
  m_loopable->collect_variables(state);
}

void Loop_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_expression->collect_variables(state);
  m_loopable->collect_variables(state);
}

void Loop_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.collect_variables(state); }, m_choice );
}
