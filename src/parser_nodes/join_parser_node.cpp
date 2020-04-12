//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "join_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Join_Parser_Node::Join_Parser_Node(std::wstring&& keyword)
  : m_keyword(std::move(keyword))
{
}

Join_Parser_Node_Ptr
Join_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, { L"join", L"unite" });
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Join_Parser_Node>(std::move(keyword));
}

void Join_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}

void Join_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"rockstar::join");
}
