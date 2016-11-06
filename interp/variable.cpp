#include "variable.h"
#include "struct_decl.h" //forward declaration found in variable.h
//since a variable needs to know its type context but also a struct_decl holds variables

set<string> variable::builtins( 
{
	"char",
	"int",
	"float"
} );

variable::variable( xml_node<char>* var_node, unordered_map<string, struct_decl>& global_type_context, unordered_map<string, struct_decl>* local_type_context )
{
	_offset = atoi( var_node->first_attribute( "offset" )->value() );
	_name = var_node->first_node( "Symbol" )->first_attribute( "name" )->value();

	parse_node_type( var_node->first_node(), global_type_context, local_type_context );
}

void variable::parse_base_decl( xml_node<char>* node )
{
	_size = atoi( node->first_attribute( "size" )->value() );
	_type = node->first_node( "Symbol" )->first_attribute( "name" )->value();
}

void variable::parse_pointer_type( xml_node<char>* node, unordered_map<string, struct_decl>& global_type_context, unordered_map<string, struct_decl>* local_type_context )
{
	_size = 4; //I think?

	string pointer_type = node->first_node( "Symbol" )->first_attribute( "name" )->value();

	//count the number of asterisks, that's our indirection level
	_indirection = count( pointer_type.begin(), pointer_type.end(), '*' );
	//then remove them
	pointer_type.erase( std::remove( pointer_type.begin(), pointer_type.end(), '*' ), pointer_type.end() );
	//and that's the type
	_type = pointer_type;

	if( builtins.find( _type ) == builtins.end() ) //if the type is not a builtin type
	{
		auto global_type = global_type_context.find( _type );
		if( global_type != global_type_context.end() )
		{
			_struct_decl = &global_type->second; //'_struct_decl' now points to the decl object that declares the type of this var
		}
		else if( local_type_context != nullptr ) //it's a local type
		{
			_struct_decl = &local_type_context->find( _type )->second;
		}
		else
		{
			//we don't know what type it is?
			//kernel_panic();
			//throw "a flotation device";
			//exit( EXIT_FAILURE );
			//assert( false );
			//new uint64_t[SIZE_MAX]; -> does the user have 34.35 gigs of ram quick and easy test
		}
		_size = _struct_decl->size();
	}
}

void variable::parse_struct_type( xml_node<char>* node, unordered_map<string, struct_decl>& global_type_context, unordered_map<string, struct_decl>* local_type_context )
{
	_type = node->first_node( "Symbol" )->first_attribute( "name" )->value();
	_struct_decl = &global_type_context[_type];
	_size = _struct_decl->size();
}

void variable::parse_array_type( xml_node<char>* node, unordered_map<string, struct_decl>& global_type_context, unordered_map<string, struct_decl>* local_type_context )
{
	//every time we come into this function we add a 'size' index to our vector.
	_array_ranges.push_back( atoi( node->first_attribute( "Size" )->value() ) );

	for( xml_node<char>* inner_node : node->all_nodes() )
	{
		if( strcmp( inner_node->name(), "Symbol" ) != 0 ) //NOT our 'Symbol' node
		{
			parse_node_type( inner_node, global_type_context, local_type_context );
			break;  //there's only going to be two nodes. A Symbol node and a node of the other type, which is the one we care about
		}
	}
}

void variable::parse_node_type( xml_node<char>* node, unordered_map<string, struct_decl>& global_type_context, unordered_map<string, struct_decl>* local_type_context )
{
	if( strcmp( node->name(), "BaseDecl" ) == 0 ) // (int, float, etc.)
	{
		parse_base_decl( node );
	}
	else if( strcmp( node->name(), "PointerType" ) == 0 )
	{
		parse_pointer_type( node, global_type_context, local_type_context );
	}
	else if( strcmp( node->name(), "StructType" ) == 0 )
	{
		parse_struct_type( node, global_type_context, local_type_context );
	}
	else if( strcmp( node->name(), "ArrayType" ) == 0 )
	{
		parse_array_type( node, global_type_context, local_type_context );
	}
	else
	{
		//??
	}
}

string variable::to_string( uint32_t fp ) const
{
	if( _struct_decl == nullptr )
	{
            if( _type == "int" )
            {
                return "nope :/"; //to_string( Abs_Get_Word( fp + _offset ) );
            }
            else return _type + " is unsupported.";
		//return string( "variable address:" ) + std::to_string( _offset + fp ) + '\n';
	}
	else
	{
		return _struct_decl->to_string( fp );
	}
}
