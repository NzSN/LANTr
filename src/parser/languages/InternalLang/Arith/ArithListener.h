
// Generated from ./Arith.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "ArithParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by ArithParser.
 */
class  ArithListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProg(ArithParser::ProgContext *ctx) = 0;
  virtual void exitProg(ArithParser::ProgContext *ctx) = 0;

  virtual void enterExpr(ArithParser::ExprContext *ctx) = 0;
  virtual void exitExpr(ArithParser::ExprContext *ctx) = 0;


};

