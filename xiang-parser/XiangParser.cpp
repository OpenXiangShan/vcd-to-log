
// Generated from Xiang.g4 by ANTLR 4.9


#include "XiangVisitor.h"

#include "XiangParser.h"


using namespace antlrcpp;
using namespace antlr4;

XiangParser::XiangParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

XiangParser::~XiangParser() {
  delete _interpreter;
}

std::string XiangParser::getGrammarFileName() const {
  return "Xiang.g4";
}

const std::vector<std::string>& XiangParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& XiangParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- MainContext ------------------------------------------------------------------

XiangParser::MainContext::MainContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<XiangParser::EventContext *> XiangParser::MainContext::event() {
  return getRuleContexts<XiangParser::EventContext>();
}

XiangParser::EventContext* XiangParser::MainContext::event(size_t i) {
  return getRuleContext<XiangParser::EventContext>(i);
}


size_t XiangParser::MainContext::getRuleIndex() const {
  return XiangParser::RuleMain;
}


antlrcpp::Any XiangParser::MainContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<XiangVisitor*>(visitor))
    return parserVisitor->visitMain(this);
  else
    return visitor->visitChildren(this);
}

XiangParser::MainContext* XiangParser::main() {
  MainContext *_localctx = _tracker.createInstance<MainContext>(_ctx, getState());
  enterRule(_localctx, 0, XiangParser::RuleMain);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(13);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == XiangParser::StringLiteral) {
      setState(10);
      event();
      setState(15);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EventContext ------------------------------------------------------------------

XiangParser::EventContext::EventContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* XiangParser::EventContext::StringLiteral() {
  return getToken(XiangParser::StringLiteral, 0);
}

XiangParser::ExpressionContext* XiangParser::EventContext::expression() {
  return getRuleContext<XiangParser::ExpressionContext>(0);
}


size_t XiangParser::EventContext::getRuleIndex() const {
  return XiangParser::RuleEvent;
}


antlrcpp::Any XiangParser::EventContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<XiangVisitor*>(visitor))
    return parserVisitor->visitEvent(this);
  else
    return visitor->visitChildren(this);
}

XiangParser::EventContext* XiangParser::event() {
  EventContext *_localctx = _tracker.createInstance<EventContext>(_ctx, getState());
  enterRule(_localctx, 2, XiangParser::RuleEvent);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(16);
    match(XiangParser::StringLiteral);
    setState(17);
    match(XiangParser::T__0);
    setState(18);
    match(XiangParser::T__1);
    setState(19);
    expression(0);
    setState(20);
    match(XiangParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

XiangParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<XiangParser::ExpressionContext *> XiangParser::ExpressionContext::expression() {
  return getRuleContexts<XiangParser::ExpressionContext>();
}

XiangParser::ExpressionContext* XiangParser::ExpressionContext::expression(size_t i) {
  return getRuleContext<XiangParser::ExpressionContext>(i);
}

XiangParser::SignalContext* XiangParser::ExpressionContext::signal() {
  return getRuleContext<XiangParser::SignalContext>(0);
}

tree::TerminalNode* XiangParser::ExpressionContext::Mul() {
  return getToken(XiangParser::Mul, 0);
}

tree::TerminalNode* XiangParser::ExpressionContext::Div() {
  return getToken(XiangParser::Div, 0);
}

tree::TerminalNode* XiangParser::ExpressionContext::Add() {
  return getToken(XiangParser::Add, 0);
}

tree::TerminalNode* XiangParser::ExpressionContext::Minus() {
  return getToken(XiangParser::Minus, 0);
}

tree::TerminalNode* XiangParser::ExpressionContext::Equal() {
  return getToken(XiangParser::Equal, 0);
}

tree::TerminalNode* XiangParser::ExpressionContext::NotEqual() {
  return getToken(XiangParser::NotEqual, 0);
}

tree::TerminalNode* XiangParser::ExpressionContext::And() {
  return getToken(XiangParser::And, 0);
}

tree::TerminalNode* XiangParser::ExpressionContext::Or() {
  return getToken(XiangParser::Or, 0);
}


size_t XiangParser::ExpressionContext::getRuleIndex() const {
  return XiangParser::RuleExpression;
}


antlrcpp::Any XiangParser::ExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<XiangVisitor*>(visitor))
    return parserVisitor->visitExpression(this);
  else
    return visitor->visitChildren(this);
}


XiangParser::ExpressionContext* XiangParser::expression() {
   return expression(0);
}

XiangParser::ExpressionContext* XiangParser::expression(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  XiangParser::ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, parentState);
  XiangParser::ExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 4;
  enterRecursionRule(_localctx, 4, XiangParser::RuleExpression, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(28);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case XiangParser::T__3: {
        setState(23);
        match(XiangParser::T__3);
        setState(24);
        dynamic_cast<ExpressionContext *>(_localctx)->inner = expression(0);
        setState(25);
        match(XiangParser::T__4);
        break;
      }

      case XiangParser::Identifier: {
        setState(27);
        signal();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(47);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(45);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(30);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(31);
          dynamic_cast<ExpressionContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == XiangParser::Mul

          || _la == XiangParser::Div)) {
            dynamic_cast<ExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(32);
          expression(7);
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(33);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(34);
          dynamic_cast<ExpressionContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == XiangParser::Add

          || _la == XiangParser::Minus)) {
            dynamic_cast<ExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(35);
          expression(6);
          break;
        }

        case 3: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(36);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(37);
          dynamic_cast<ExpressionContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == XiangParser::Equal

          || _la == XiangParser::NotEqual)) {
            dynamic_cast<ExpressionContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(38);
          expression(5);
          break;
        }

        case 4: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(39);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(40);
          dynamic_cast<ExpressionContext *>(_localctx)->op = match(XiangParser::And);
          setState(41);
          expression(4);
          break;
        }

        case 5: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(42);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(43);
          dynamic_cast<ExpressionContext *>(_localctx)->op = match(XiangParser::Or);
          setState(44);
          expression(3);
          break;
        }

        default:
          break;
        } 
      }
      setState(49);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- SignalContext ------------------------------------------------------------------

XiangParser::SignalContext::SignalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

XiangParser::Instance_pathContext* XiangParser::SignalContext::instance_path() {
  return getRuleContext<XiangParser::Instance_pathContext>(0);
}

tree::TerminalNode* XiangParser::SignalContext::Identifier() {
  return getToken(XiangParser::Identifier, 0);
}


size_t XiangParser::SignalContext::getRuleIndex() const {
  return XiangParser::RuleSignal;
}


antlrcpp::Any XiangParser::SignalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<XiangVisitor*>(visitor))
    return parserVisitor->visitSignal(this);
  else
    return visitor->visitChildren(this);
}

XiangParser::SignalContext* XiangParser::signal() {
  SignalContext *_localctx = _tracker.createInstance<SignalContext>(_ctx, getState());
  enterRule(_localctx, 6, XiangParser::RuleSignal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(50);
    instance_path();
    setState(51);
    match(XiangParser::T__5);
    setState(52);
    match(XiangParser::Identifier);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Instance_pathContext ------------------------------------------------------------------

XiangParser::Instance_pathContext::Instance_pathContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> XiangParser::Instance_pathContext::Identifier() {
  return getTokens(XiangParser::Identifier);
}

tree::TerminalNode* XiangParser::Instance_pathContext::Identifier(size_t i) {
  return getToken(XiangParser::Identifier, i);
}


size_t XiangParser::Instance_pathContext::getRuleIndex() const {
  return XiangParser::RuleInstance_path;
}


antlrcpp::Any XiangParser::Instance_pathContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<XiangVisitor*>(visitor))
    return parserVisitor->visitInstance_path(this);
  else
    return visitor->visitChildren(this);
}

XiangParser::Instance_pathContext* XiangParser::instance_path() {
  Instance_pathContext *_localctx = _tracker.createInstance<Instance_pathContext>(_ctx, getState());
  enterRule(_localctx, 8, XiangParser::RuleInstance_path);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(54);
    match(XiangParser::Identifier);
    setState(59);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(55);
        match(XiangParser::T__5);
        setState(56);
        match(XiangParser::Identifier); 
      }
      setState(61);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool XiangParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 2: return expressionSempred(dynamic_cast<ExpressionContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool XiangParser::expressionSempred(ExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 6);
    case 1: return precpred(_ctx, 5);
    case 2: return precpred(_ctx, 4);
    case 3: return precpred(_ctx, 3);
    case 4: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> XiangParser::_decisionToDFA;
atn::PredictionContextCache XiangParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN XiangParser::_atn;
std::vector<uint16_t> XiangParser::_serializedATN;

std::vector<std::string> XiangParser::_ruleNames = {
  "main", "event", "expression", "signal", "instance_path"
};

std::vector<std::string> XiangParser::_literalNames = {
  "", "':'", "'{'", "'}'", "'('", "')'", "'.'", "", "", "'*'", "'/'", "'+'", 
  "'-'", "'&&'", "'||'", "'=='", "'!='"
};

std::vector<std::string> XiangParser::_symbolicNames = {
  "", "", "", "", "", "", "", "Identifier", "StringLiteral", "Mul", "Div", 
  "Add", "Minus", "And", "Or", "Equal", "NotEqual", "WS"
};

dfa::Vocabulary XiangParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> XiangParser::_tokenNames;

XiangParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x13, 0x41, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x3, 0x2, 0x7, 0x2, 0xe, 
    0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x11, 0xb, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x1f, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 
    0x30, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x33, 0xb, 0x4, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x7, 0x6, 0x3c, 0xa, 
    0x6, 0xc, 0x6, 0xe, 0x6, 0x3f, 0xb, 0x6, 0x3, 0x6, 0x2, 0x3, 0x6, 0x7, 
    0x2, 0x4, 0x6, 0x8, 0xa, 0x2, 0x5, 0x3, 0x2, 0xb, 0xc, 0x3, 0x2, 0xd, 
    0xe, 0x3, 0x2, 0x11, 0x12, 0x2, 0x43, 0x2, 0xf, 0x3, 0x2, 0x2, 0x2, 
    0x4, 0x12, 0x3, 0x2, 0x2, 0x2, 0x6, 0x1e, 0x3, 0x2, 0x2, 0x2, 0x8, 0x34, 
    0x3, 0x2, 0x2, 0x2, 0xa, 0x38, 0x3, 0x2, 0x2, 0x2, 0xc, 0xe, 0x5, 0x4, 
    0x3, 0x2, 0xd, 0xc, 0x3, 0x2, 0x2, 0x2, 0xe, 0x11, 0x3, 0x2, 0x2, 0x2, 
    0xf, 0xd, 0x3, 0x2, 0x2, 0x2, 0xf, 0x10, 0x3, 0x2, 0x2, 0x2, 0x10, 0x3, 
    0x3, 0x2, 0x2, 0x2, 0x11, 0xf, 0x3, 0x2, 0x2, 0x2, 0x12, 0x13, 0x7, 
    0xa, 0x2, 0x2, 0x13, 0x14, 0x7, 0x3, 0x2, 0x2, 0x14, 0x15, 0x7, 0x4, 
    0x2, 0x2, 0x15, 0x16, 0x5, 0x6, 0x4, 0x2, 0x16, 0x17, 0x7, 0x5, 0x2, 
    0x2, 0x17, 0x5, 0x3, 0x2, 0x2, 0x2, 0x18, 0x19, 0x8, 0x4, 0x1, 0x2, 
    0x19, 0x1a, 0x7, 0x6, 0x2, 0x2, 0x1a, 0x1b, 0x5, 0x6, 0x4, 0x2, 0x1b, 
    0x1c, 0x7, 0x7, 0x2, 0x2, 0x1c, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x1f, 
    0x5, 0x8, 0x5, 0x2, 0x1e, 0x18, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x1d, 0x3, 
    0x2, 0x2, 0x2, 0x1f, 0x31, 0x3, 0x2, 0x2, 0x2, 0x20, 0x21, 0xc, 0x8, 
    0x2, 0x2, 0x21, 0x22, 0x9, 0x2, 0x2, 0x2, 0x22, 0x30, 0x5, 0x6, 0x4, 
    0x9, 0x23, 0x24, 0xc, 0x7, 0x2, 0x2, 0x24, 0x25, 0x9, 0x3, 0x2, 0x2, 
    0x25, 0x30, 0x5, 0x6, 0x4, 0x8, 0x26, 0x27, 0xc, 0x6, 0x2, 0x2, 0x27, 
    0x28, 0x9, 0x4, 0x2, 0x2, 0x28, 0x30, 0x5, 0x6, 0x4, 0x7, 0x29, 0x2a, 
    0xc, 0x5, 0x2, 0x2, 0x2a, 0x2b, 0x7, 0xf, 0x2, 0x2, 0x2b, 0x30, 0x5, 
    0x6, 0x4, 0x6, 0x2c, 0x2d, 0xc, 0x4, 0x2, 0x2, 0x2d, 0x2e, 0x7, 0x10, 
    0x2, 0x2, 0x2e, 0x30, 0x5, 0x6, 0x4, 0x5, 0x2f, 0x20, 0x3, 0x2, 0x2, 
    0x2, 0x2f, 0x23, 0x3, 0x2, 0x2, 0x2, 0x2f, 0x26, 0x3, 0x2, 0x2, 0x2, 
    0x2f, 0x29, 0x3, 0x2, 0x2, 0x2, 0x2f, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x30, 
    0x33, 0x3, 0x2, 0x2, 0x2, 0x31, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x31, 0x32, 
    0x3, 0x2, 0x2, 0x2, 0x32, 0x7, 0x3, 0x2, 0x2, 0x2, 0x33, 0x31, 0x3, 
    0x2, 0x2, 0x2, 0x34, 0x35, 0x5, 0xa, 0x6, 0x2, 0x35, 0x36, 0x7, 0x8, 
    0x2, 0x2, 0x36, 0x37, 0x7, 0x9, 0x2, 0x2, 0x37, 0x9, 0x3, 0x2, 0x2, 
    0x2, 0x38, 0x3d, 0x7, 0x9, 0x2, 0x2, 0x39, 0x3a, 0x7, 0x8, 0x2, 0x2, 
    0x3a, 0x3c, 0x7, 0x9, 0x2, 0x2, 0x3b, 0x39, 0x3, 0x2, 0x2, 0x2, 0x3c, 
    0x3f, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3e, 
    0x3, 0x2, 0x2, 0x2, 0x3e, 0xb, 0x3, 0x2, 0x2, 0x2, 0x3f, 0x3d, 0x3, 
    0x2, 0x2, 0x2, 0x7, 0xf, 0x1e, 0x2f, 0x31, 0x3d, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

XiangParser::Initializer XiangParser::_init;
