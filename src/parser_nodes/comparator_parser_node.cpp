//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "comparator_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "greater_parser_node.hpp"
#include "great_parser_node.hpp"
#include "small_parser_node.hpp"
#include "smaller_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>
#include <variant>

void Comparator_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  output << m_keyword_1;
  m_separator_1->dump(output);
  m_greater->dump(output);
  m_separator_2->dump(output);
  output << m_keyword_2;
}

void Comparator_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  output << m_keyword_1;
  m_separator_1->dump(output);
  m_smaller->dump(output);
  m_separator_2->dump(output);
  output << m_keyword_2;
}

void Comparator_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  output << m_keyword_1;
  m_separator_1->dump(output);
  output << m_keyword_2;
  m_separator_2->dump(output);
  m_great->dump(output);
  m_separator_3->dump(output);
  output << m_keyword_3;
}

void Comparator_Parser_Node::Choice_4::dump(std::wostream& output) const
{
  output << m_keyword_1;
  m_separator_1->dump(output);
  output << m_keyword_2;
  m_separator_2->dump(output);
  m_small->dump(output);
  m_separator_3->dump(output);
  output << m_keyword_3;
}

Comparator_Parser_Node::Comparator_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Comparator_Parser_Node::Comparator_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Comparator_Parser_Node:: Comparator_Parser_Node(Choice_3&& choice)
  : m_choice(std::move(choice))
{
}

Comparator_Parser_Node::Comparator_Parser_Node(Choice_4&& choice)
  : m_choice(std::move(choice))
{
}

Comparator_Parser_Node_Ptr
Comparator_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_keyword_1 = check_and_remove(input, L"is");
  if (choice.m_keyword_1.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_1 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_greater = Greater_Parser_Node::parse(input);
  if (choice.m_greater == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_2 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword_2 = check_and_remove(input, L"than");
  if (choice.m_keyword_2.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Comparator_Parser_Node>(std::move(choice));
}

Comparator_Parser_Node_Ptr
Comparator_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_keyword_1 = check_and_remove(input, L"is");
  if (choice.m_keyword_1.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_1 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_smaller = Smaller_Parser_Node::parse(input);
  if (choice.m_smaller == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_2 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword_2 = check_and_remove(input, L"than");
  if (choice.m_keyword_2.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Comparator_Parser_Node>(std::move(choice));
}

Comparator_Parser_Node_Ptr
Comparator_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_3 choice;

  choice.m_keyword_1 = check_and_remove(input, L"is");
  if (choice.m_keyword_1.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_1 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword_2 = check_and_remove(input, L"as");
  if (choice.m_keyword_2.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_2 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_great = Great_Parser_Node::parse(input);
  if (choice.m_great == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_3 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_3 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword_3 = check_and_remove(input, L"as");
  if (choice.m_keyword_3.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Comparator_Parser_Node>(std::move(choice));
}

Comparator_Parser_Node_Ptr
Comparator_Parser_Node::parse_choice_4(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_4 choice;

  choice.m_keyword_1 = check_and_remove(input, L"is");
  if (choice.m_keyword_1.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_1 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_1 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword_2 = check_and_remove(input, L"as");
  if (choice.m_keyword_2.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_2 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_2 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_small = Small_Parser_Node::parse(input);
  if (choice.m_small == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_separator_3 = Separator_Parser_Node::parse(input);
  if (choice.m_separator_3 == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_keyword_3 = check_and_remove(input, L"as");
  if (choice.m_keyword_3.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Comparator_Parser_Node>(std::move(choice));
}

Comparator_Parser_Node_Ptr
Comparator_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto node = parse_choice_1(input);
  if (node) {
    return node;
  }

  // reset file pointer and try next choice
  input.seekg(offset);
  node = parse_choice_2(input);
  if (node) {
    return node;
  }

  // reset file pointer and try next choice
  input.seekg(offset);
  node = parse_choice_3(input);
  if (node) {
    return node;
  }

  // reset file pointer and try next choice
  input.seekg(offset);
  node = parse_choice_4(input);
  if (node) {
    return node;
  }

  input.seekg(offset);

  return nullptr;
}

void Comparator_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Comparator_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code( L">" );
}

void Comparator_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code( L"<" );
}

void Comparator_Parser_Node::Choice_3::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code( L">=" );
}

void Comparator_Parser_Node::Choice_4::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code( L"<=" );
}

void Comparator_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);
}
