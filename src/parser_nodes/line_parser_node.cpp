//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "line_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "statement_parser_node.hpp"
#include "eol_parser_node.hpp"
#include "eof_parser_node.hpp"

#include "parser_node.hpp"

#include <istream>
#include <vector>
#include <variant>
#include <memory>

void Line_Parser_Node::Choice_1_2::dump(std::wostream& output) const
{
  for (const auto& e: m_eols) { e->dump(output); }
}

void Line_Parser_Node::Choice_1_1::dump(std::wostream& output) const
{
  m_eof->dump(output);
}

void Line_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  for (const auto& s: m_separators) { s->dump(output); }
  m_statement->dump(output);
  std::visit( [&output](auto&& arg){ arg.dump(output); }, m_choice);
}

void Line_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_eol->dump(output);
}

Line_Parser_Node::Line_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Line_Parser_Node::Line_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Line_Parser_Node_Ptr
Line_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);

  choice.m_statement = Statement_Parser_Node::parse(input);
  if (choice.m_statement == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto tmp_offset = input.tellg();

  auto eof = Eof_Parser_Node::parse(input);
  if (eof) {
    choice.m_choice = Choice_1_1{ std::move(eof) };
    return std::make_shared<Line_Parser_Node>(std::move(choice));
  }

  input.seekg(tmp_offset);

  Eol_Parser_Node_Ptr tmp;
  Eol_Parser_Node_Vector eols;

  while ((tmp = Eol_Parser_Node::parse(input)) != nullptr) {
    if (input.peek() == WEOF) {
      break;
    }

    eols.push_back(tmp);
  }

  if (!eols.empty()) {
    choice.m_choice = Choice_1_2{ std::move(eols) };
    return std::make_shared<Line_Parser_Node>(std::move(choice));
  }

  input.seekg(offset);
  return nullptr;
}

Line_Parser_Node_Ptr
Line_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_eol = Eol_Parser_Node::parse(input);
  if (choice.m_eol == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Line_Parser_Node>(std::move(choice));
}

Line_Parser_Node_Ptr
Line_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  if (input.eof()) {
    return nullptr;
  }

  auto node = parse_choice_1(input);
  if (node) {
    return node;
  }

  input.seekg(offset);

  if (input.eof()) {
    return nullptr;
  }

  node = parse_choice_2(input);
  if (node) {
    return node;
  }

  input.seekg(offset);
  return nullptr;
}

void Line_Parser_Node::dump(std::wostream& output) const
{
  std::visit( [&output](auto&& choice){ choice.dump(output); }, m_choice);
}

void Line_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_statement->generate_cpp(state);
}

void Line_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  (void)state;
}

void Line_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit( [&state](auto&& arg){ arg.generate_cpp(state); }, m_choice);
}

void Line_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_statement->collect_variables(state);
}

void Line_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  (void)state;
}

void Line_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit( [&state](auto&& arg){ arg.collect_variables(state); }, m_choice);
}
