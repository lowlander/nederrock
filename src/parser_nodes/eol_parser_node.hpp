//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_EOL_PARSER_NODE_HPP
#define NEDERROCK_SRC_EOL_PARSER_NODE_HPP

#include "eol_parser_node_pre.hpp"
#include "noise_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>

class Eol_Parser_Node {
public:
  Eol_Parser_Node(Noise_Parser_Node_Vector&& noise);

  static Eol_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
private:
  Noise_Parser_Node_Vector m_noise;
};

#endif // NEDERROCK_SRC_EOL_PARSER_NODE_HPP
