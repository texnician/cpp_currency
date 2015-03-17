#pragma once

#include <iostream>
#include <boost/type_index.hpp>

using boost::typeindex::type_id_with_cvr;

#define PRINT_TYPE(expr) std::cout << #expr << " => [" \
  << type_id_with_cvr<decltype(expr)>().pretty_name() \
  << "]\n";

class Creative
{};
