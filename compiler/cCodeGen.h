#pragma once

#include <fstream>

#include "cVisitor.h"

class cGenAddr;
class cFixupTable;

static const int STACK_FRAME_SIZE = 8;

class cCodeGen : public cVisitor
{
    public:
        cCodeGen(std::string filename);
        ~cCodeGen();

        virtual void VisitAllNodes(cAstNode *node);

        virtual void Visit(cAddressExpr *node);
        virtual void Visit(cArrayRef *node);
        virtual void Visit(cArrayType *node);
        virtual void Visit(cAsmNode *node);
        virtual void Visit(cAssignExpr *node);
        //virtual void Visit(cAstNode *node);
        //virtual void Visit(cBaseDeclNode *node);
        virtual void Visit(cBinaryExpr *node);
        //virtual void Visit(cDecl *node);
        //virtual void Visit(cDeclsList *node);
        //virtual void Visit(cExpr *node);
        virtual void Visit(cExprStmt *node);
        virtual void Visit(cForStmt *node);
        virtual void Visit(cFuncCall *node);
        virtual void Visit(cFuncDecl *node);
        virtual void Visit(cIfStmt *node);
        virtual void Visit(cIntExpr *node);
        //virtual void Visit(cNopStmt *node);
        virtual void Visit(cParams *node);
        virtual void Visit(cPlainVarRef *node);
        virtual void Visit(cPointerDeref *node);
        //virtual void Visit(cPointerType *node);
        virtual void Visit(cPostfixExpr *node);
        virtual void Visit(cPrefixExpr *node);
        virtual void Visit(cReturnStmt *node);
        virtual void Visit(cShortCircuitExpr *node);
        virtual void Visit(cSizeofExpr *node);
        //virtual void Visit(cStmt *node);
        //virtual void Visit(cStmtsList *node);
        virtual void Visit(cStringLit *node);
        virtual void Visit(cStructDeref *node);
        virtual void Visit(cStructRef *node);
        //virtual void Visit(cStructType *node);
        //virtual void Visit(cSymbol *node);
        //virtual void Visit(cTypeDecl *node);
        virtual void Visit(cUnaryExpr *node);
        virtual void Visit(cVarDecl *node);
        //virtual void Visit(cVarRef *node);
        virtual void Visit(cWhileStmt *node);


        void EmitInt(int val);
        void EmitString(std::string str);
        void EmitActualString(std::string str);
        void EmitGlobalDef(std::string str, int size);
        void EmitGlobalRef(std::string str);
        void EmitActualGlobal(std::string str, int size);
        void EmitComment(std::string str);
        void EmitFixup(int loc, int dest);
        void SetLabelRef(std::string label);
        void SetLabelValue(std::string label);
        std::string GenerateLabel();

        static const int STACKL_WORD_SIZE;
    protected:
        std::ofstream m_Output;
        std::string m_Filename;
        int m_Next_Label = 1;
        int m_Location = 0;
        cFixupTable *m_FixupTable;
        cGenAddr *m_GenAddr;

        static const int STACK_FRAME_SIZE = 8;
};