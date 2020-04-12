//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "common_prefix_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Common_Prefix_Parser_Node::Common_Prefix_Parser_Node(std::wstring&& keyword)
  : m_keyword(std::move(keyword))
{
}

Common_Prefix_Parser_Node_Ptr
Common_Prefix_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, { L"an", L"a", L"the", L"my", L"your" } );
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Common_Prefix_Parser_Node>(std::move(keyword));
}

void Common_Prefix_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}

std::wstring Common_Prefix_Parser_Node::to_string()
{
  return m_keyword;
}
