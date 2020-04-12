//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "greater_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Greater_Parser_Node::Greater_Parser_Node(std::wstring&& keyword)
  : m_keyword(keyword)
{
}

Greater_Parser_Node_Ptr
Greater_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, { L"higher", L"greater", L"bigger", L"stronger" });
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Greater_Parser_Node>(std::move(keyword));
}

void Greater_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}
