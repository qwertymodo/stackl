#pragma once

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <unordered_map>
using std::unordered_map;
#include <time.h>

#include "rapidxml.hpp"
using rapidxml::xml_document;
using rapidxml::xml_node;
using rapidxml::xml_attribute;
#include "rapidxml_utils.hpp"

#include "struct_decl.h"
#include "function.h"

class abstract_syntax_tree
{
public:
    abstract_syntax_tree() {}
    abstract_syntax_tree( const string& filename, uint32_t max_ip );
    string all_funcs();
    string all_locals( const string& func_name );
    string all_globals();

    inline unordered_map<string, variable>& globals() { return _globals; }
    inline unordered_map<string, function>& functions() { return _functions; }
    inline unordered_map<string, struct_decl>& struct_decls() { return _struct_decls; }

    inline time_t compile_time() const { return _compile_time; }

    variable* var( const string& func_name, const string& var_name );

private:

    void load( xml_document<char>& doc, uint32_t max_ip );

    //key = symbol name, value = object with that name
    unordered_map<string, variable> _globals;
    unordered_map<string, function> _functions;
    unordered_map<string, struct_decl> _struct_decls;

    time_t _compile_time;

};