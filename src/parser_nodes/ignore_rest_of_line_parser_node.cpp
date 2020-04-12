//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "ignore_rest_of_line_parser_node.hpp"
#include "separator_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>


Ignore_Rest_Of_Line_Parser_Node::Ignore_Rest_Of_Line_Parser_Node(Separator_Parser_Node_Ptr&& separator,
                                                                 std::wstring&& rest_of_line)
  : m_separator(std::move(separator))
  , m_rest_of_line(std::move(rest_of_line))
{
}

Ignore_Rest_Of_Line_Parser_Node_Ptr
Ignore_Rest_Of_Line_Parser_Node::parse(Token_Stream& input)
{
  std::wstring rest_of_line;

  auto separator = Separator_Parser_Node::parse(input);
  while (true) {
    auto c = static_cast<wchar_t>(input.peek());
    if (c == L'\n') {
      break;
    }

    rest_of_line += static_cast<wchar_t>(input.get());
  }

  return std::make_shared<Ignore_Rest_Of_Line_Parser_Node>(std::move(separator),
                                                           std::move(rest_of_line));
}

void Ignore_Rest_Of_Line_Parser_Node::dump(std::wostream& output) const
{
  if (m_separator) { m_separator->dump(output); }
  output << m_rest_of_line;
}
