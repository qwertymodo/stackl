#include "debugger_command.h"
#include <stdexcept>
using std::runtime_error;

debugger_command::debugger_command( stackl_debugger& dbg, op func, const set<string>& names, const string& help, bool op_dbg )
    : _dbg( dbg ), _func( func ), _names( names ), _help( help ), _op_dbg( op_dbg )
{
    for( const string& name : _names )
        if( name.find( ' ' ) != string::npos )
            throw runtime_error( string( "Command names must be one word: \"" ) + name + "\"" );
}

bool debugger_command::called_by( const string& cmd ) const
{
    return _names.find( cmd ) != _names.end();
}

void debugger_command::run( string& params, Machine_State* cpu ) const
{
    (_dbg.*_func)( params, cpu );
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

bool debugger_command::has_same_name( const debugger_command& cmd ) const
{
    for( const string& name : _names )
        if( cmd.called_by( name ) )
            return true;
    return false;
}