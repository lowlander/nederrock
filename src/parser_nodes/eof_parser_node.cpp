//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "eof_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Eof_Parser_Node::Eof_Parser_Node()
{
}

Eof_Parser_Node_Ptr
Eof_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  if (input.eof()) {
    return std::make_shared<Eof_Parser_Node>();
  } else {
    input.seekg(offset);
    return nullptr;
  }
}

void Eof_Parser_Node::dump(std::wostream& output) const
{
  (void)output;
}
