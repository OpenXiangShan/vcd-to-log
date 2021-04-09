#include "antlr4-runtime.h"
#include "XiangBaseVisitor.h"
#include "Trace.h"

#ifndef __XVISITOR__
#define __XVISITOR__


class XVisitor : public XiangBaseVisitor {

private:

    Trace* trace;
    VCDTime time;
    VCDValue* v_true;
    VCDValue* v_false;
    std::vector<VCDValue*> tmp_results;

public:;
    XVisitor(Trace *t);
    ~XVisitor();

    void set_time(VCDTime time);

    antlrcpp::Any visitEvent(XiangParser::EventContext *ctx) override;

    antlrcpp::Any visitSignal(XiangParser::SignalContext *ctx) override;

    antlrcpp::Any visitExpression(XiangParser::ExpressionContext *ctx) override;

};

#endif
