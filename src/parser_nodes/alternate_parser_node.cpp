//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "alternate_parser_node.hpp"

#include "separator_parser_node.hpp"
#include "statement_parser_node.hpp"
#include "eol_parser_node.hpp"
#include "block_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>
#include <string>

void Alternate_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_separator_1->dump(output);
  output << m_keyword;
  m_separator_2->dump(output);
  m_statement->dump(output);
}

void Alternate_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  for(const auto& e: m_eols) { e->dump(output); }
  output << m_keyword;
  m_separator->dump(output);
  m_statement->dump(output);
}

void Alternate_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  for(const auto& e: m_eols) { e->dump(output); }
  output << m_keyword;
  m_eol->dump(output);
  m_block->dump(output);
}

void Alternate_Parser_Node::Choice_4::dump(std::wostream& output) const
{
  m_eol->dump(output);
}

Alternate_Parser_Node::Alternate_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Alternate_Parser_Node::Alternate_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Alternate_Parser_Node::Alternate_Parser_Node(Choice_3&& choice)
  : m_choice(std::move(choice))
{
}

Alternate_Parser_Node::Alternate_Parser_Node(Choice_4&& choice)
  : m_choice(std::move(choice))
{
}

Alternate_Parser_Node_Ptr
Alternate_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_separator_1 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword = check_and_remove(input, L"else");
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_2 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_statement = Statement_Parser_Node::parse(input);
  if (choice.m_statement == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Alternate_Parser_Node>(std::move(choice));
}

Alternate_Parser_Node_Ptr
Alternate_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_eols = parse_zero_or_more<Eol_Parser_Node>(input);
  if (choice.m_eols.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword = check_and_remove(input, L"else");
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator = Separator_Parser_Node::parse(input);
  if (choice.m_separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_statement = Statement_Parser_Node::parse(input);
  if (choice.m_statement == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Alternate_Parser_Node>(std::move(choice));
}

Alternate_Parser_Node_Ptr
Alternate_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_3 choice;

  choice.m_eols = parse_zero_or_more<Eol_Parser_Node>(input);
  if (choice.m_eols.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword = check_and_remove(input, L"else");
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_eol = Eol_Parser_Node::parse(input);
  if (choice.m_eol == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_block = Block_Parser_Node::parse(input);
  if (choice.m_block == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Alternate_Parser_Node>(std::move(choice));
}

Alternate_Parser_Node_Ptr
Alternate_Parser_Node::parse_choice_4(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_4 choice;

  choice.m_eol = Eol_Parser_Node::parse(input);
  if (choice.m_eol == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Alternate_Parser_Node>(std::move(choice));
}

Alternate_Parser_Node_Ptr
Alternate_Parser_Node::parse(Token_Stream& input)
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

void Alternate_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Alternate_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr scope) const
{
  m_statement->generate_cpp(scope);
}

void Alternate_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr scope) const
{
  m_statement->generate_cpp(scope);
}

void Alternate_Parser_Node::Choice_3::generate_cpp(Scope_State_Ptr scope) const
{
  m_block->generate_cpp(scope);
}

void Alternate_Parser_Node::Choice_4::generate_cpp(Scope_State_Ptr scope) const
{
  (void)scope;
}

void Alternate_Parser_Node::generate_cpp(Scope_State_Ptr scope) const
{
  std::visit([&scope](auto&& choice) { choice.generate_cpp(scope); }, m_choice);
}

void Alternate_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr scope) const
{
  m_statement->collect_variables(scope);
}

void Alternate_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr scope) const
{
  m_statement->collect_variables(scope);
}

void Alternate_Parser_Node::Choice_3::collect_variables(Scope_State_Ptr scope) const
{
  m_block->collect_variables(scope);
}

void Alternate_Parser_Node::Choice_4::collect_variables(Scope_State_Ptr scope) const
{
  (void)scope;
}

void Alternate_Parser_Node::collect_variables(Scope_State_Ptr scope) const
{
  std::visit([&scope](auto&& choice) { choice.collect_variables(scope); }, m_choice);
}
