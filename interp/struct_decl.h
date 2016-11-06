#pragma once

#include <vector>
using std::vector;
#include <unordered_map>
using std::unordered_map;
#include <string>
using std::string;
#include <string.h>

#include "rapidxml.hpp"
using rapidxml::xml_node;

#include "variable.h"

class struct_decl
{
public:
	struct_decl( xml_node<char>* decl_node, unordered_map<string, struct_decl>& live_type_context );
	struct_decl() {}

	string to_string( uint32_t fp ) const;
	inline int size() const { return _size; }
	inline string name() const { return _name; }

private:
	vector<variable> _fields; //key = field name, value = variable obj
	int _size = 0;
	string _name;
};

