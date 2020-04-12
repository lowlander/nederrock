//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "null_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Null_Parser_Node::Null_Parser_Node(std::wstring&& keyword)
  : m_keyword(keyword)
{
}

Null_Parser_Node_Ptr
Null_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();
  auto keyword = check_and_remove(input, { L"null", L"nothing", L"nowhere", L"nobody", L"empty", L"gone" });
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Null_Parser_Node>(std::move(keyword));
}

void Null_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}

void Null_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"rockstar::Variable(nullptr)");
}
