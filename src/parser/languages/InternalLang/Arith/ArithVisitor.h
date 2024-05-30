
// Generated from ./Arith.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "ArithParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by ArithParser.
 */
class  ArithVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by ArithParser.
   */
    virtual std::any visitProg(ArithParser::ProgContext *context) = 0;

    virtual std::any visitExpr(ArithParser::ExprContext *context) = 0;


};

