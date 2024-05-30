
// Generated from ./Arith.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "ArithVisitor.h"


/**
 * This class provides an empty implementation of ArithVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  ArithBaseVisitor : public ArithVisitor {
public:

  virtual std::any visitProg(ArithParser::ProgContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpr(ArithParser::ExprContext *ctx) override {
    return visitChildren(ctx);
  }


};

