
// Generated from Xiang.g4 by ANTLR 4.9

#pragma once


#include "antlr4-runtime.h"
#include "XiangParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by XiangParser.
 */
class  XiangVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by XiangParser.
   */
    virtual antlrcpp::Any visitMain(XiangParser::MainContext *context) = 0;

    virtual antlrcpp::Any visitEvent(XiangParser::EventContext *context) = 0;

    virtual antlrcpp::Any visitExpression(XiangParser::ExpressionContext *context) = 0;

    virtual antlrcpp::Any visitSignal(XiangParser::SignalContext *context) = 0;

    virtual antlrcpp::Any visitInstance_path(XiangParser::Instance_pathContext *context) = 0;


};

