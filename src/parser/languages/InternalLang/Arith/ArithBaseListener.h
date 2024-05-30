
// Generated from ./Arith.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "ArithListener.h"


/**
 * This class provides an empty implementation of ArithListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  ArithBaseListener : public ArithListener {
public:

  virtual void enterProg(ArithParser::ProgContext * /*ctx*/) override { }
  virtual void exitProg(ArithParser::ProgContext * /*ctx*/) override { }

  virtual void enterExpr(ArithParser::ExprContext * /*ctx*/) override { }
  virtual void exitExpr(ArithParser::ExprContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

