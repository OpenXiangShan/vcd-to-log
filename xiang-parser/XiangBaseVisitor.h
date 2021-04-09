
// Generated from Xiang.g4 by ANTLR 4.9

#pragma once


#include "antlr4-runtime.h"
#include "XiangVisitor.h"


/**
 * This class provides an empty implementation of XiangVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  XiangBaseVisitor : public XiangVisitor {
public:

  virtual antlrcpp::Any visitMain(XiangParser::MainContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEvent(XiangParser::EventContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExpression(XiangParser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSignal(XiangParser::SignalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInstance_path(XiangParser::Instance_pathContext *ctx) override {
    return visitChildren(ctx);
  }


};

