
#include <string>
#include <variant>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <locale>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace rockstar {

struct Variable;
using Variable_Ptr = std::shared_ptr<Variable>;

inline std::wstring to_string(double nr)
{
  std::wstringstream ss;
  ss << std::fixed << std::setprecision(10) << nr;
  auto res = ss.str();

  bool has_dot = false;
  bool has_other = false;

  for (const auto& c: res) {
    if (c == '.') {
      has_dot = true;
    } else if (!((c >= '0' && c <= '9') || (c == L'-') || (c == L'+'))) {
      has_other = true;
    }
  }

  if (has_other) {
    return res;
  }

  if (!has_dot) {
    return res;
  }

  size_t new_size = res.size();
  for (auto i = res.rbegin(); i != res.rend(); ++i) {
    if (*i != L'0') {
      break;
    }

    new_size--;
  }

  res.resize(new_size);

  if (res.back() == L'.') {
    res.pop_back();
  }

  return res;
}

struct Mysterious {
  std::wstring to_string() const { return L"mysterious"; }
  double to_number() const { return 0.0; }
  bool to_bool() const { return false; }
};

struct Bool {
  constexpr Bool(bool value) : m_value(value) { }

  std::wstring to_string() const { return m_value ? L"true" : L"false"; }
  double to_number() const { return m_value ? 1.0 :  0.0; }
  bool to_bool() const { return m_value; }

  Bool& operator--(int) { m_value = !m_value; return *this; }
  Bool& operator++(int) { m_value = !m_value; return *this; }

  bool m_value { false };
};

struct Number {
  Number(const wchar_t* str, double nr)
    : m_str(str) , m_nr(nr)
  {
    m_str = rockstar::to_string(m_nr);
  }

  Number(double nr)
    : m_nr(nr)
  {
    m_str = rockstar::to_string(m_nr);
  }

  std::wstring to_string() const { return m_str; }
  double to_number() const { return m_nr; }
  bool to_bool() const { return m_nr != 0.0; }

  Number& operator--(int)
  {
    m_nr--;

    m_str = rockstar::to_string(m_nr);

    return *this;
  }

  Number& operator++(int)
  {
    m_nr++;

    m_str = rockstar::to_string(m_nr);

    return *this;
  }

  std::wstring m_str;
  double m_nr{0.0};
};

struct String {
  String(const std::wstring& str) :  m_str(str) { }
  String(const wchar_t* str) :  m_str(str) { }

  std::wstring to_string() const { return m_str; }
  double to_number() const { return std::stod(m_str); }
  bool to_bool() const { return !m_str.empty(); }

  friend bool operator==(const String& lhs, const String& rhs) {
    return lhs.m_str == rhs.m_str;
  }

  friend bool operator!=(const String& lhs, const String& rhs) {
    return lhs.m_str != rhs.m_str;
  }

  friend bool operator<(const String& lhs, const String& rhs) {
    return lhs.m_str < rhs.m_str;
  }

  friend bool operator<=(const String& lhs, const String& rhs) {
    return lhs.m_str <= rhs.m_str;
  }

  friend bool operator>(const String& lhs, const String& rhs) {
    return lhs.m_str > rhs.m_str;
  }

  friend bool operator>=(const String& lhs, const String& rhs) {
    return lhs.m_str >= rhs.m_str;
  }

  Variable join(const std::wstring& k) const;

  std::wstring m_str;
};


struct Null {
  bool to_bool() const { return false; }
  double to_number() const { return 0.0; }
  std::wstring to_string() const { return L"null"; }
};

struct Array {
  bool to_bool() const { return false; }
  double to_number() const { return 0.0; }
  std::wstring to_string() const { return std::to_wstring(get_size()); }

  Variable& operator[](size_t index);
  Variable& operator[](const std::wstring& index);

  size_t get_size() const {
    if (m_vector.empty()) {
      return 0;
    }

    return m_vector.rbegin()->first + 1;
  }

  Variable join(const std::wstring& k) const;

  std::map<size_t, Variable_Ptr>        m_vector;
  std::map<std::wstring, Variable_Ptr>  m_map;
};

struct Variable {
  std::variant<Mysterious, Null, Bool, String, Number, Array> m_data;
  static Variable m_dummy;

  Variable() : m_data(Mysterious()) { }
  Variable(const wchar_t* str, double nr) : m_data(Number(str, nr)) { }
  Variable(const wchar_t* str) : m_data(String(str)) { }
  Variable(bool b) : m_data(Bool(b)) { }
  Variable(nullptr_t) : m_data(Null()) { }
  Variable(Number&& nr) : m_data(nr) { }
  Variable(String&& s) : m_data(s) { }
  Variable(Bool&& b) : m_data(b) { }
  Variable(Mysterious&& m) : m_data(m) { }
  Variable(Null&& n) : m_data(n) { }
  Variable(Array&& a) : m_data(a) { }

  Variable(Variable&& v) : m_data(std::move(v.m_data)) { }
  Variable(const Variable& v) : m_data(v.m_data) { }

  Variable& operator=(Variable&& v) { m_data = std::move(v.m_data); return *this; }
  Variable& operator=(const Variable& v) { m_data = v.m_data; return *this; }

  bool to_bool() const { return std::visit([](auto&& d) { return d.to_bool(); }, m_data); }
  double to_number() const { return std::visit([](auto&& d) { return d.to_number(); }, m_data); }
  std::wstring to_string() const { return std::visit([](auto&& d) { return d.to_string(); }, m_data); }

  explicit operator bool() const { return to_bool(); }

  Variable& operator[](const Variable& index)
  {
    try {
      if (std::holds_alternative<String>(m_data))
      {
        size_t i = static_cast<size_t>(index.to_number());
        const auto str = to_string();
        if (i >= str.size()) {
          m_dummy = Mysterious();
        } else {
          std::wstring tmp;
          tmp += str[i];
          m_dummy = String(tmp);
        }

        return m_dummy;
      }

      if (!std::holds_alternative<Array>(m_data)) {
        m_data = Array();
      }

      auto array = std::get_if<Array>(&m_data);

      if (std::holds_alternative<String>(index.m_data)) {
        return (*array)[index.to_string()];
      } else {
        return (*array)[static_cast<size_t>(index.to_number())];
      }
    }
    catch(...) {
      m_dummy = Mysterious();
    }

    return m_dummy;
  }

  friend Variable operator+(const Variable& lhs, const Variable& rhs)
  {
    if (std::holds_alternative<String>(lhs.m_data) ||
        std::holds_alternative<String>(rhs.m_data))
    {
      return String(lhs.to_string() + rhs.to_string());
    }

    try {
      if (auto ptr = std::get_if<Mysterious>(&lhs.m_data)) {
        return String(ptr->to_string() + lhs.to_string());
      }
      if (std::holds_alternative<Null>(lhs.m_data)) {
        return Number(rhs.to_number());
      }
      if (auto ptr = std::get_if<Number>(&lhs.m_data)) {
        return Number(ptr->to_number() + rhs.to_number() );
      }
      if (auto ptr = std::get_if<Bool>(&lhs.m_data)) {
        return Number(ptr->to_number() + rhs.to_number() );
      }
    }
    catch(...) {
      return Mysterious();
    }

    return Mysterious();
  }

  friend Variable operator-(const Variable& lhs, const Variable& rhs) {
    try {
      if (std::holds_alternative<Null>(lhs.m_data)) {
        return Number( 0.0 - rhs.to_number() );
      }
      if (auto ptr = std::get_if<Number>(&lhs.m_data)) {
        return Number( ptr->to_number() - rhs.to_number() );
      }
      if (auto ptr = std::get_if<Bool>(&lhs.m_data)) {
        return Number(ptr->to_number() + rhs.to_number() );
      }
    }
    catch(...) {
      return Mysterious();
    }
    return Mysterious();
  }

  friend Variable operator*(const Variable& lhs, const Variable& rhs)
  {
    try {
      if (std::holds_alternative<Null>(lhs.m_data)) {
        return Number(0.0);
      }
      if (auto ptr = std::get_if<Number>(&lhs.m_data)) {
        return Number( ptr->to_number() * rhs.to_number() );
      }

      if (auto ptr = std::get_if<String>(&lhs.m_data)) {
        const auto times = rhs.to_number();
        const auto s = ptr->to_string();
        std::wstring res;
        for (int i = 0; i < times; i++) {
          res += s;
        }

        return String(res);
      }
    }
    catch(...) {
      return Mysterious();
    }

    return Mysterious();
  }

  friend Variable operator/(const Variable& lhs, const Variable& rhs) {
    try {
      if (std::holds_alternative<Null>(lhs.m_data)) {
        return Number(0.0);
      }
      if (auto ptr = std::get_if<Number>(&lhs.m_data)) {
        const auto div = rhs.to_number();
        if (div == 0.0) {
          return Mysterious();
        } else {
          return Number( ptr->to_number() / div );
        }
      }
    }
    catch(...) {
      return Mysterious();
    }

    return Mysterious();
  }

  friend bool operator>(const Variable& lhs, const Variable& rhs)
  {
    if (std::holds_alternative<Bool>(lhs.m_data) ||
        std::holds_alternative<Bool>(rhs.m_data))
    {
      throw std::logic_error("Can't > compare Bool");
    }

    try {
      if (std::holds_alternative<Mysterious>(lhs.m_data)) {
        return (0.0 > rhs.to_number());
      }
      if (std::holds_alternative<Null>(lhs.m_data)) {
        return (0.0 > rhs.to_number());
      }
      if (auto ptr = std::get_if<Number>(&lhs.m_data)) {
        return ptr->to_number() > rhs.to_number();
      }
      if (auto ptr = std::get_if<String>(&lhs.m_data)) {
        return ptr->to_string() > rhs.to_string();
      }
    }
    catch(...) {
      return false;
    }

    return false;
  }

  friend bool operator>=(const Variable& lhs, const Variable& rhs)
  {
    if (std::holds_alternative<Bool>(lhs.m_data) ||
        std::holds_alternative<Bool>(rhs.m_data))
    {
      throw std::logic_error("Can't >= compare Bool");
    }

    try {
      if (std::holds_alternative<Mysterious>(lhs.m_data)) {
        return (0.0 >= rhs.to_number());
      }
      if (std::holds_alternative<Null>(lhs.m_data)) {
        return (0.0 >= rhs.to_number());
      }
      if (auto ptr = std::get_if<Null>(&lhs.m_data)) {
        return ptr->to_number() >= rhs.to_number();
      }
      if (auto ptr = std::get_if<Number>(&lhs.m_data)) {
        return ptr->to_number() >= rhs.to_number();
      }
      if (auto ptr = std::get_if<String>(&lhs.m_data)) {
        return ptr->to_string() >= rhs.to_string();
      }
    }
    catch(...) {
      return false;
    }

    return false;
  }

  friend bool operator<(const Variable& lhs, const Variable& rhs)
  {
    if (std::holds_alternative<Bool>(lhs.m_data) ||
        std::holds_alternative<Bool>(rhs.m_data))
    {
      throw std::logic_error("Can't < compare Bool");
    }

    try {
      if (std::holds_alternative<Mysterious>(lhs.m_data)) {
        return (0.0 < rhs.to_number());
      }
      if (std::holds_alternative<Null>(lhs.m_data)) {
        return (0.0 < rhs.to_number());
      }
      if (auto ptr = std::get_if<Null>(&lhs.m_data)) {
        return ptr->to_number() < rhs.to_number();
      }
      if (auto ptr = std::get_if<Number>(&lhs.m_data)) {
        return ptr->to_number() < rhs.to_number();
      }
      if (auto ptr = std::get_if<String>(&lhs.m_data)) {
        return ptr->to_string() < rhs.to_string();
      }
    }
    catch(...) {
      return false;
    }

    return false;
  }

  friend bool operator<=(const Variable& lhs, const Variable& rhs)
  {
    if (std::holds_alternative<Bool>(lhs.m_data) ||
        std::holds_alternative<Bool>(rhs.m_data))
    {
      throw std::logic_error("Can't <= compare Bool");
    }

    try {
      if (std::holds_alternative<Mysterious>(lhs.m_data)) {
        return (0.0 <= rhs.to_number());
      }
      if (std::holds_alternative<Null>(lhs.m_data)) {
        return (0.0 <= rhs.to_number());
      }
      if (auto ptr = std::get_if<Number>(&lhs.m_data)) {
        return ptr->to_number() <= rhs.to_number();
      }
      if (auto ptr = std::get_if<String>(&lhs.m_data)) {
        return ptr->to_string() <= rhs.to_string();
      }
    }
    catch(...) {
      return false;
    }

    return false;
  }

  friend bool operator==(const Variable& lhs, const Variable& rhs)
  {
    try {
      if (std::holds_alternative<Bool>(lhs.m_data) ||
          std::holds_alternative<Bool>(rhs.m_data))
      {
        return lhs.to_bool() == rhs.to_bool();
      }

      if (std::holds_alternative<Null>(lhs.m_data) ||
          std::holds_alternative<Null>(rhs.m_data))
      {
        return lhs.to_bool() == rhs.to_bool();
      }

      if (std::holds_alternative<Mysterious>(lhs.m_data) ||
          std::holds_alternative<Mysterious>(rhs.m_data))
      {
        return lhs.to_bool() == rhs.to_bool();
      }

      if (auto ptr = std::get_if<Number>(&lhs.m_data)) {
        return ptr->to_number() == rhs.to_number();
      }
      if (auto ptr = std::get_if<String>(&lhs.m_data)) {
        return ptr->to_string() == rhs.to_string();
      }
    }
    catch(...) {
      return false;
    }

    return false;
  }

  friend bool operator!=(const Variable& lhs, const Variable& rhs)
  {
    try {
      if (std::holds_alternative<Bool>(lhs.m_data) ||
          std::holds_alternative<Bool>(rhs.m_data))
      {
        return lhs.to_bool() != rhs.to_bool();
      }

      if (std::holds_alternative<Null>(lhs.m_data) ||
          std::holds_alternative<Null>(rhs.m_data))
      {
        return lhs.to_bool() != rhs.to_bool();
      }

      if (std::holds_alternative<Mysterious>(lhs.m_data) ||
          std::holds_alternative<Mysterious>(rhs.m_data))
      {
        return lhs.to_bool() != rhs.to_bool();
      }

      if (auto ptr = std::get_if<Number>(&lhs.m_data)) {
        return ptr->to_number() != rhs.to_number();
      }
      if (auto ptr = std::get_if<String>(&lhs.m_data)) {
        return ptr->to_string() != rhs.to_string();
      }
    }
    catch(...) {
      return true;
    }

    return true;
  }

  Variable& operator++(int)
  {
    if (std::holds_alternative<Null>(m_data)) {
      m_data = Number(0.0);
    }

    if (auto ptr = std::get_if<Number>(&m_data)) {
      (*ptr)++;
    }

    if (auto ptr = std::get_if<Bool>(&m_data)) {
      (*ptr)++;
    }

    return *this;
  }

  Variable& operator--(int)
  {
    if (std::holds_alternative<Null>(m_data)) {
      m_data = Number(0.0);
    }

    if (auto ptr = std::get_if<Number>(&m_data)) {
      (*ptr)--;
    }

    if (auto ptr = std::get_if<Bool>(&m_data)) {
      (*ptr)--;
    }

    return *this;
  }

  void ceil() {
    m_data = Number(std::ceil(to_number()));
  }

  void floor() {
    m_data = Number(std::floor(to_number()));
  }

  void math_round() {
    m_data = Number(std::round(to_number()));
  }

  Variable join(const Variable& k) const
  {
    if (auto ptr = std::get_if<Array>(&m_data)) {
      return ptr->join(k.to_string());
    }

    if (auto ptr = std::get_if<String>(&m_data)) {
      return ptr->join(k.to_string());
    }

    return *this;
  }

  Variable join() const
  {
    return this->join(String(L""));
  }

  Variable cast(const Variable& k) const
  {
    try {
      if (std::holds_alternative<Null>(m_data)) {
        return String(L"0");
      }
      if (auto ptr = std::get_if<Number>(&m_data)) {
        std::wstring str;
        str += static_cast<wchar_t>(ptr->to_number());
        return String(str);
      }
      if (auto ptr = std::get_if<String>(&m_data)) {
        const auto n = static_cast<int>(k.to_number());
        if (n == 10) {
          return Number( ptr->to_number() );
        } else {
          const auto l = std::stol(ptr->to_string(), nullptr, n);
          return Number( static_cast<double>(l) );
        }
      }
    }
    catch(...) {
      return Mysterious();
    }

    return *this;
  }

  Variable cast() const
  {
    return this->cast(Number(10));
  }

  Variable split(const Variable& k) const
  {
    const auto str = this->to_string();
    const auto delim_str = k.to_string();
    wchar_t delim;

    if (delim_str.empty()) {
      delim = L' ';
    } else {
      delim = delim_str[0];
    }

    std::wstring token;
    std::wstringstream ss(str);

    Array res;

    size_t index = 0;
    while (std::getline(ss, token, delim)) {
        res[index++] = String(token);
    }

    return res;
  }

  Variable split() const
  {
    const auto str = this->to_string();
    Array res;
    size_t index = 0;
    for (const auto& c: str) {
      std::wstring tmp;
      tmp += c;
      res[index++] = String(tmp);
    }

    return res;
  }
};

Variable Variable::m_dummy;

Variable& Array::operator[](const std::wstring& index)
{
  auto i = m_map.find(index);
  if (i == m_map.end()) {
    auto v = std::make_shared<Variable>();
    auto res = m_map.insert(std::make_pair(index, v));
    i = res.first;
  }

  return *(i->second.get());
}

Variable& Array::operator[](size_t index)
{
  auto i = m_vector.find(index);
  if (i == m_vector.end()) {
    auto v = std::make_shared<Variable>();
    auto res = m_vector.insert(std::make_pair(index, v));
    i = res.first;
  }

  return *(i->second.get());
}

std::wostream* output_stream{nullptr};
std::wstringstream test_stream;

inline void print(const Variable& arg)
{
  const auto res = arg.to_string();

  (*output_stream) << res << std::endl;

  test_stream << res << std::endl;
}

inline Variable readline() {
  std::wstring res;
  std::getline(std::wcin, res, L'\n');
  return String(res);
}

inline void ceil(Variable& v) { v.ceil(); }
inline void floor(Variable& v) { v.floor(); }
inline void math_round(Variable& v) { v.math_round(); }

inline Variable join(const Variable& array, const Variable& k) { return array.join(k); }
inline Variable join(const Variable& array) { return array.join(); }
inline Variable cast(const Variable& v, const Variable& k) { return v.cast(k); }
inline Variable cast(const Variable& v) { return v.cast(); }
inline Variable split(const Variable& v, const Variable& k) { return v.split(k); }
inline Variable split(const Variable& v) { return v.split(); }

Variable String::join(const std::wstring& k) const
{
  std::wstring res;

  for (const auto& c: m_str) {
    res += c;
    res += k;
  }

  return String(res);
}

Variable Array::join(const std::wstring& k) const
{
  std::wstring res;

  for (const auto& c: m_vector) {
    res += c.second->to_string();
    res += k;
  }

  for (const auto& c: m_map) {
    res += c.second->to_string();
    res += k;
  }

  res.resize(res.size() - k.size());

  return String(res);
}

} // namespace rockstar

void rockstar_main();

int main(int argc, char* argv[])
{
  std::locale::global(std::locale(std::locale(), std::locale("en_US.UTF-8"), std::locale::ctype));

  std::wcout.imbue(std::locale());
  std::wcerr.imbue(std::locale());

  int i{0};
  std::string expected_output_file;
  std::string output_file;

  while (i < argc) {
    std::string s(argv[i]);

    if (s == "-e") {
      if (i >= argc) {
        return -1;
      }

      expected_output_file = argv[++i];
    }

    if (s == "-o") {
      if (i >= argc) {
        return -1;
      }

      output_file = argv[++i];
    }

    i++;
  }

  std::wofstream output_file_stream;
  if (output_file.empty()) {
    rockstar::output_stream = &std::wcout;
  } else {
    output_file_stream.open(output_file);
    rockstar::output_stream = &output_file_stream;
  }

  rockstar_main();

  if (!expected_output_file.empty()) {
    std::wifstream expected_output_stream(expected_output_file, std::ios::binary);
    expected_output_stream.imbue(std::locale("en_US.UTF-8"));

    int line_nr{0};

    for (std::wstring expected_line; std::getline(expected_output_stream, expected_line);) {
      line_nr++;

      if (std::wstring real_line; std::getline(rockstar::test_stream, real_line)) {
        if (expected_line != real_line) {
          std::wcerr << L"Error: output different than expected\n";
          std::wcerr << L"Line: " << line_nr << "|" << real_line << "|" << expected_line << "|" << std::endl;
          return -1;
        }
      } else {
        std::wcerr << L"Error: output different than expected\n";
        return -1;
      }
    }
  }

  return 0;
}
