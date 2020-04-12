//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "split_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Split_Parser_Node::Split_Parser_Node(std::wstring&& keyword)
  : m_keyword(std::move(keyword))
{
}

Split_Parser_Node_Ptr
Split_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, { L"cut", L"split", L"shatter" });
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Split_Parser_Node>(std::move(keyword));
}

void Split_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}

void Split_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"rockstar::split");
}
