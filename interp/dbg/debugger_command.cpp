#include "debugger_command.h"

debugger_command::debugger_command( stackl_debugger& dbg, op func, const set<string>& names, const string& help ): _dbg(dbg), _func(func), _names(names), _help(help) {}

bool debugger_command::called_by( const string& cmd ) const
{
    return _names.find( cmd ) != _names.end();
}

bool debugger_command::run( string& params, Machine_State* cpu ) const
{
    return (_dbg.*_func)( params, cpu );
}

string debugger_command::to_string() const
{
    string ret = "[";
    for( const string& str : _names )
        ret += str + "|";
    ret.pop_back(); //remove last bar
    ret += "] " + _help;
    return ret;
}