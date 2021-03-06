#pragma once
//*******************************************************
// Purpose: Base class for list of statements (incl decls)
//
// Author: Philip Howard
// Email:  phil.howard@oit.edu
//
// Date: 2/20/2015
//
//*******************************************************

#include <string>
#include <list>
using std::list;

#include "cStmt.h"
#include "cDecl.h"

using std::list;

class cStmtsList : public cStmt
{
  public:
    // constructor places first decl in list
    cStmtsList(cStmt *stmt) : cStmt()
    {
        AddChild(stmt);
    }

    // add a declaration to the list
    virtual void AddNode(cStmt *stmt) 
    {
        AddChild(stmt);
    }

    // add a list of stmts to this list
    void AddList(cStmtsList *stmts)
    {
        for (int ii=0; ii<stmts->NumChildren(); ii++)
        {
            AddChild( stmts->GetChild(ii) );
        }
    }

    virtual int Size()
    {
        int size = 0;
        for (int ii=0; ii<NumChildren(); ii++)
        {
            size += GetStmt(ii)->Size();
        }

        return size;
    }

    cStmt *GetStmt(int index) { return (cStmt*)GetChild(index); }

    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};

