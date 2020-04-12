//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_EOF_PARSER_NODE_HPP
#define NEDERROCK_SRC_EOF_PARSER_NODE_HPP

#include "eof_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>

class Eof_Parser_Node {
public:
  Eof_Parser_Node();

  static Eof_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
private:
};

#endif // NEDERROCK_SRC_EOF_PARSER_NODE_HPP
