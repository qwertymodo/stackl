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
    abstract_syntax_tree( const string& filename );
    void add_ast( const string& filename );
    void fixup_globals( unordered_map<string, int32_t>& global_offsets );

    string all_funcs();
    string all_locals( const string& func_name );
    string all_globals();
    string all_statics();
    string statics_in( const string& filename );

    inline unordered_map<string, variable>& globals() { return _globals; }
    inline unordered_map<string, unordered_map<string, variable>>& statics() { return _statics; }
    inline unordered_map<string, function>& functions() { return _functions; }
    inline unordered_map<string, unordered_map<string, struct_decl>>& struct_decls() { return _struct_decls; }

    inline time_t compile_time( const string& filename ) const { return _compile_times.at( filename ); }

    variable* var( const string& cur_file, const string& func_name, const string& var_name );
    function* func( const string& func_name );

private:

    void load( const string& filename, xml_document<char>& doc );

    //key = symbol name, value = object with that name
    unordered_map<string, variable> _globals;
    unordered_map<string, unordered_map<string, variable>> _statics; //per file
    unordered_map<string, function> _functions;
    unordered_map<string, unordered_map<string, struct_decl>> _struct_decls; //per file
    unordered_map<string, time_t> _compile_times;

};
