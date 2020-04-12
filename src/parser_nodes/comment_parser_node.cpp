//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "comment_parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Comment_Parser_Node::Comment_Parser_Node(std::wstring&& text)
  : m_text(std::move(text))
{
}

Comment_Parser_Node_Ptr
Comment_Parser_Node::parse(Token_Stream& input)
{
  wchar_t c = input.peek();

  if (c == L'(') {
    input.get(); // get the ( and throw it away, we don't need it

    std::wstring text;
    while ((c = input.get()) != L')') {
      text += c;
    }

    return std::make_shared<Comment_Parser_Node>(std::move(text));
  } else {
    return nullptr;
  }
}

void
Comment_Parser_Node::dump(std::wostream& output) const
{
  output << L'(' << m_text << L')';
}
