//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "pronoun_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Pronoun_Parser_Node::Pronoun_Parser_Node(std::wstring&& keyword)
  : m_keyword(std::move(keyword))
{
}

Pronoun_Parser_Node_Ptr
Pronoun_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, { L"they", L"them" });
  if (!keyword.empty()) {
    return std::make_shared<Pronoun_Parser_Node>(std::move(keyword));
  }

  input.seekg(offset);

  keyword = check_and_remove(input, { L"she", L"him", L"her", L"hir", L"zie", L"zir", L"xem", L"ver" });
  if (!keyword.empty()) {
    return std::make_shared<Pronoun_Parser_Node>(std::move(keyword));
  }

  input.seekg(offset);

  keyword = check_and_remove(input, { L"ze", L"ve", L"xe", L"it", L"he"});
  if (!keyword.empty()) {
    return std::make_shared<Pronoun_Parser_Node>(std::move(keyword));
  }

  input.seekg(offset);

  return nullptr;
}

void Pronoun_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}

std::wstring Pronoun_Parser_Node::to_string()
{
  return m_keyword;
}

void Pronoun_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_pronoun_code();
}
