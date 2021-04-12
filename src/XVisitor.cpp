#include <iostream>
#include "XVisitor.h"

XVisitor::XVisitor(Trace* t):time(0),trace(t) {
    tmp_results = std::vector<VCDValue *>(100);
    v_true = new VCDValue(VCDBit(VCD_1));
    v_false = new VCDValue(VCDBit(VCD_0));
}

XVisitor::~XVisitor(){
    for(auto v : tmp_results){
        delete v;
    }
    delete v_true;
    delete v_false;
    delete trace;
}

antlrcpp::Any XVisitor::visitEvent(XiangParser::EventContext *ctx) {
    VCDValue* result = visitExpression(ctx->expression());
    if(result->asBool()){
        std::cout << "[time = " << time << "] " << ctx->StringLiteral()->getText() << std::endl;
    }
    return nullptr;
}

antlrcpp::Any XVisitor::visitExpression(XiangParser::ExpressionContext *ctx) {
    if(ctx->children.size() == 1 || ctx->inner){
        if(ctx->inner) { // parentheses
            return visitExpression(dynamic_cast<XiangParser::ExpressionContext *>(ctx->children[1]));
        } else {
            // this is a signal
            return visitSignal(dynamic_cast<XiangParser::SignalContext *>(ctx->children.front()));
        }
    } else {
        // for now, we don't support unary op
        assert(ctx->children.size() == 3);
        auto *lexp = dynamic_cast<XiangParser::ExpressionContext *>(ctx->children[0]);
        auto *rexp = dynamic_cast<XiangParser::ExpressionContext *>(ctx->children[2]);
        VCDValue* lv = visitExpression(lexp);
        VCDValue* rv = visitExpression(rexp);
        VCDValue* res;

        switch (ctx->op->getType()) {
            case XiangParser::Mul:
                break;
            case XiangParser::Div:
                break;
            case XiangParser::Add:
                break;
            case XiangParser::Minus:
                break;
            case XiangParser::Equal:
                break;
            case XiangParser::NotEqual:
                break;
            case XiangParser::And:
                res = *lv && *rv;
                break;
            case XiangParser::Or:
                break;
            default:
                assert(false && "Unknown Op type");
        }
        if(res){
            tmp_results.push_back(res);
        }
        return res;
    }
}

antlrcpp::Any XVisitor::visitSignal(XiangParser::SignalContext *ctx) {
    StrPath signalPath = ctx->getText();
    VCDSignal* sig = trace->get_signal_by_path(signalPath);
    assert(sig && "Signal can't be found!");
    VCDValue* v = trace->get_signal_value_at(sig->hash, time);
    return v;
}

void XVisitor::set_time(VCDTime t) {
    this->time = t;
}