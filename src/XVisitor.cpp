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
        std::cout << "[cycle = " << (int)(time/2) << "] " << ctx->StringLiteral()->getText() << std::endl;
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
    } else if(ctx->children.size() == 3){
        auto *lexp = dynamic_cast<XiangParser::ExpressionContext *>(ctx->children[0]);
        auto *rexp = dynamic_cast<XiangParser::ExpressionContext *>(ctx->children[2]);
        VCDValue* lv = visitExpression(lexp);
        assert(lv);
        VCDValue* rv = visitExpression(rexp);
        assert(rv);
        VCDValue* res;
        
        switch (ctx->op->getType()) {
            case XiangParser::Mul:
                res = (*lv) * (*rv);
                break;
            case XiangParser::Div:
                break;
            case XiangParser::Add:
                res = *lv + *rv;
                break;
            case XiangParser::Minus:
                res = *lv - *rv;
                break;
            case XiangParser::Equal:
                res = *lv == *rv;
                break;
            case XiangParser::NotEqual:
                res = *lv != *rv;
                break;
            case XiangParser::And:
                res = *lv && *rv;
                break;
            case XiangParser::Or:
                res = *lv || *rv;
                break;
            case XiangParser::BitOr:
                res = *lv | *rv;
                break;
            case XiangParser::BitAnd:
                res = *lv & *rv;
                break;
            default:
                assert(false && "Unknown Op type");
        }
        if(res){
            tmp_results.push_back(res);
        }
        return res;
    } else
    {
        // TODO, support unary op
        assert(ctx->children.size() == 2);
        //for now , we just support '!' and '~' operator
        auto *pre       = ctx->op;
        auto *exp_child = dynamic_cast<XiangParser::ExpressionContext*>(ctx->children[1]);
        VCDValue* exp_res = visitExpression(exp_child);
        if(pre->getType() == XiangParser::Not)
        {
            if(exp_res->asBool())
            {
                return new VCDValue(VCD_0);
            }else
            {
                return new VCDValue(VCD_1);
            }
        }else if(pre->getType() == XiangParser::Reverse)
        {
            if(exp_res->get_type() == VCD_SCALAR)
            {
                if(exp_res->asBool())
                {
                    return new VCDValue(VCD_0);
                }else
                {
                    return new VCDValue(VCD_1);
                }
                
            }else
            {
                return new VCDValue(VCDValue::Reverse(exp_res->get_value_vector()));
            }
            
        }else
        {
            assert(false && "unimplememted unary op");
        }
        
        

    }
    
}

const static std::vector<std::string> HexMap = {
    "0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"
    };
const static std::vector<std::string> OctMap = {
    "000","001","010","011","100","101","110","111"
    };

antlrcpp::Any XVisitor::visitSignal(XiangParser::SignalContext *ctx) {
    StrPath signalPath = ctx->getText();
    //handle constants
    if(signalPath[0] == '0' && signalPath[1] == 'x')
    {
        VCDValue* v = new VCDValue(new VCDBitVector());
        for(int i=2;i<signalPath.length();i++)
        {
            if(signalPath[i] >= '0' && signalPath[i] <= '9')
            {
                //0x0123456789
                for(int j=0;j<4;j++)
                {
                    if(HexMap[signalPath[i] - '0'][j] == '0')
                    {
                        v->get_value_vector()->push_back(VCD_0);
                    }else
                    {
                        v->get_value_vector()->push_back(VCD_1);
                    }
                    
                }

            }else
            {
                //0xabcdef
                for(int j=0;j<4;j++)
                {
                    if(HexMap[(signalPath[i] - 'a') + 10][j] == '0')
                    {
                        v->get_value_vector()->push_back(VCD_0);
                    }else
                    {
                        v->get_value_vector()->push_back(VCD_1);
                    }
                }
            }
        
        }
        return v;

    }else if(signalPath[0] == '0' && signalPath[1] == 'X')
    {
        VCDValue* v = new VCDValue(new VCDBitVector());
        for(int i=2;i<signalPath.length();i++)
        {
            if(signalPath[i] >= '0' && signalPath[i] <= '9')
            {
                //0x0123456789
                for(int j=0;j<4;j++)
                {
                    if(HexMap[signalPath[i] - '0'][j] == '0')
                    {
                        v->get_value_vector()->push_back(VCD_0);
                    }else
                    {
                        v->get_value_vector()->push_back(VCD_1);
                    }
                    
                }

            }else
            {
                //0xABCDEF
                for(int j=0;j<4;j++)
                {
                    if(HexMap[(signalPath[i] - 'A') + 10][j] == '0')
                    {
                        v->get_value_vector()->push_back(VCD_0);
                    }else
                    {
                        v->get_value_vector()->push_back(VCD_1);
                    }
                }
            }
        
        }
        return v;
    }else if(signalPath[0] == '0' && signalPath[1] == 'b')
    {
        //binary
        VCDValue* v = new VCDValue(new VCDBitVector());
        for(int i=2;i<signalPath.length();i++)
        {
            if(signalPath[i] == '0')
            {
                v->get_value_vector()->push_back(VCD_0);
            }else
            {
                v->get_value_vector()->push_back(VCD_1);
            }
            
        
        }
        return v;

    }else if(signalPath[0] == '0' && signalPath[1] == 'o')
    {
        //octonary
        VCDValue* v = new VCDValue(new VCDBitVector());
        for(int i=2;i<signalPath.length();i++)
        {
            for(int j=0;j<3;j++)
            {
                if(OctMap[(signalPath[i] - '0')][j] == '0')
                {
                    v->get_value_vector()->push_back(VCD_0);
                }else
                {
                    v->get_value_vector()->push_back(VCD_1);
                }
                
            }
            
        }
        return v;
    }else if(signalPath[0] >= '0' && signalPath[0] <= '9')
    {
        VCDValue* v = new VCDValue(new VCDBitVector());
        int value = stoi(signalPath);
        std::vector<int> temp_res;

        while(value)
        {
            if(value % 2 == 1)
            {
                temp_res.push_back(1);
            }else
            {
                temp_res.push_back(0);
            }
            
            value = value / 2;
        }
        std::reverse(temp_res.begin(),temp_res.end());
        for(int i=0;i<temp_res.size();i++)
        {
            if(temp_res[i] == 1)
            {
                v->get_value_vector()->push_back(VCD_1);
            }else
            {
                v->get_value_vector()->push_back(VCD_0);
            }
            
        }
        return v;

    }else
    {
        //handle identifier
        VCDSignal* sig = trace->get_signal_by_path(signalPath);
        assert(sig && "Signal can't be found!");
        VCDValue* v = trace->get_signal_value_at(sig->hash, time);
        assert(v && "Value is null!");
        return v;
    }
}

void XVisitor::set_time(VCDTime t) {
    this->time = t;
}