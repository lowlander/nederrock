#include "token_stream.hpp"
#include "parser_nodes/program_parser_node.hpp"

#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <string>

int main(int argc, const char* argv[])
{
  std::locale::global(std::locale(std::locale(), std::locale("en_US.UTF-8"), std::locale::ctype));
  std::wcout.imbue(std::locale());
  std::wcerr.imbue(std::locale());

  std::string input_file;
  std::string output_file;
  int i = 0;

  while (i < (argc-1))
  {
    const std::string s(argv[i]);

    if (s == "-i") {
      input_file = argv[++i];
    } else if (s == "-o") {
      output_file = argv[++i];
    }

    i++;
  }

  std::wofstream output_file_stream;
  std::wostream* output_stream;

  if (output_file.empty()) {
    output_stream = &std::wcout;
  } else {
    output_file_stream.open(output_file);
    output_stream = &output_file_stream;
  }

  Token_Stream input(input_file);

  try {
    auto p = Program_Parser_Node::parse(input);
    if (p == nullptr) {
      std::cerr << "Failed to parse" << std::endl;
      return -1;
    }

    (*output_stream) << L"/*\n";
    p->dump((*output_stream));
    (*output_stream) << L"\n*/\n";

    (*output_stream) << L"\n#include \"rockstar.hpp\"\n";

    p->generate_cpp((*output_stream));
  }
  catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return -1;
  }
  catch(...) {
    std::cerr << "Unkown Exception" << std::endl;
    return -1;
  }

  return 0;
}
