//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "break_parser_node.hpp"
#include "ignore_rest_of_line_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Break_Parser_Node::Break_Parser_Node(std::wstring&& keyword,
                                     Ignore_Rest_Of_Line_Parser_Node_Ptr&& rest_of_line)
  : m_keyword(std::move(keyword))
  , m_rest_of_line(std::move(rest_of_line))
{
}

Break_Parser_Node_Ptr
Break_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, L"break");
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  auto rest_of_line = Ignore_Rest_Of_Line_Parser_Node::parse(input);
  if (rest_of_line == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Break_Parser_Node>(std::move(keyword), std::move(rest_of_line));
}

void Break_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
  m_rest_of_line->dump(output);
}

void Break_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code( L"break" );
}
