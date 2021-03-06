#pragma once

#include <string>

#include "cExpr.h"
#include "cVarRef.h"
#include "cSymbol.h"
#include "cPointerType.h"
#include "cCodeGen.h"

class cAddressExpr : public cExpr
{
  public:
    cAddressExpr(cExpr *expr) : cExpr() 
    {
        cVarRef *var = dynamic_cast<cVarRef*>(expr);
        if (var == NULL)
            ThrowSemanticError("Took the address of something that isn't a var");
        AddChild(expr);
        cPointerType *type = cPointerType::PointerType(GetVar()->GetType());
        AddChild(type);

    }

    virtual cTypeDecl *GetType() 
    { return static_cast<cTypeDecl*>(GetChild(1)); }

    cVarRef* GetVar() { return (cVarRef*)GetChild(0); }
    virtual bool IsPointer() { return true; }

    virtual int Size() { return cCodeGen::STACKL_WORD_SIZE; }

    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};

