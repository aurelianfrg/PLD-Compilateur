// Generated from /mnt/c/Users/aurel/Desktop/Cours/4A/COMP/PLD-Compilateur/compiler/ifcc.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue"})
public class ifccParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.13.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, TYPE=8, RETURN=9, 
		CONST=10, VAR=11, COMMENT=12, DIRECTIVE=13, WS=14;
	public static final int
		RULE_axiom = 0, RULE_prog = 1, RULE_bloc = 2, RULE_instruction = 3, RULE_return_stmt = 4, 
		RULE_aff_stmt = 5, RULE_def_stmt = 6, RULE_def_aff_stmt = 7;
	private static String[] makeRuleNames() {
		return new String[] {
			"axiom", "prog", "bloc", "instruction", "return_stmt", "aff_stmt", "def_stmt", 
			"def_aff_stmt"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'main'", "'('", "')'", "'{'", "'}'", "';'", "'='", "'int'", "'return'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, "TYPE", "RETURN", "CONST", 
			"VAR", "COMMENT", "DIRECTIVE", "WS"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "ifcc.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public ifccParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@SuppressWarnings("CheckReturnValue")
	public static class AxiomContext extends ParserRuleContext {
		public ProgContext prog() {
			return getRuleContext(ProgContext.class,0);
		}
		public TerminalNode EOF() { return getToken(ifccParser.EOF, 0); }
		public AxiomContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_axiom; }
	}

	public final AxiomContext axiom() throws RecognitionException {
		AxiomContext _localctx = new AxiomContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_axiom);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(16);
			prog();
			setState(17);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ProgContext extends ParserRuleContext {
		public TerminalNode TYPE() { return getToken(ifccParser.TYPE, 0); }
		public BlocContext bloc() {
			return getRuleContext(BlocContext.class,0);
		}
		public ProgContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_prog; }
	}

	public final ProgContext prog() throws RecognitionException {
		ProgContext _localctx = new ProgContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_prog);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(19);
			match(TYPE);
			setState(20);
			match(T__0);
			setState(21);
			match(T__1);
			setState(22);
			match(T__2);
			setState(23);
			bloc();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class BlocContext extends ParserRuleContext {
		public List<InstructionContext> instruction() {
			return getRuleContexts(InstructionContext.class);
		}
		public InstructionContext instruction(int i) {
			return getRuleContext(InstructionContext.class,i);
		}
		public BlocContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_bloc; }
	}

	public final BlocContext bloc() throws RecognitionException {
		BlocContext _localctx = new BlocContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_bloc);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(25);
			match(T__3);
			setState(29);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 2816L) != 0)) {
				{
				{
				setState(26);
				instruction();
				}
				}
				setState(31);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(32);
			match(T__4);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class InstructionContext extends ParserRuleContext {
		public InstructionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_instruction; }
	 
		public InstructionContext() { }
		public void copyFrom(InstructionContext ctx) {
			super.copyFrom(ctx);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Definition_affectation_statementContext extends InstructionContext {
		public Def_aff_stmtContext def_aff_stmt() {
			return getRuleContext(Def_aff_stmtContext.class,0);
		}
		public Definition_affectation_statementContext(InstructionContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Definition_statementContext extends InstructionContext {
		public Def_stmtContext def_stmt() {
			return getRuleContext(Def_stmtContext.class,0);
		}
		public Definition_statementContext(InstructionContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Affectation_statementContext extends InstructionContext {
		public Aff_stmtContext aff_stmt() {
			return getRuleContext(Aff_stmtContext.class,0);
		}
		public Affectation_statementContext(InstructionContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Return_statementContext extends InstructionContext {
		public Return_stmtContext return_stmt() {
			return getRuleContext(Return_stmtContext.class,0);
		}
		public Return_statementContext(InstructionContext ctx) { copyFrom(ctx); }
	}

	public final InstructionContext instruction() throws RecognitionException {
		InstructionContext _localctx = new InstructionContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_instruction);
		try {
			setState(38);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,1,_ctx) ) {
			case 1:
				_localctx = new Affectation_statementContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(34);
				aff_stmt();
				}
				break;
			case 2:
				_localctx = new Definition_statementContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(35);
				def_stmt();
				}
				break;
			case 3:
				_localctx = new Definition_affectation_statementContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(36);
				def_aff_stmt();
				}
				break;
			case 4:
				_localctx = new Return_statementContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(37);
				return_stmt();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class Return_stmtContext extends ParserRuleContext {
		public Return_stmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_return_stmt; }
	 
		public Return_stmtContext() { }
		public void copyFrom(Return_stmtContext ctx) {
			super.copyFrom(ctx);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Return_varContext extends Return_stmtContext {
		public TerminalNode RETURN() { return getToken(ifccParser.RETURN, 0); }
		public TerminalNode VAR() { return getToken(ifccParser.VAR, 0); }
		public Return_varContext(Return_stmtContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Return_constContext extends Return_stmtContext {
		public TerminalNode RETURN() { return getToken(ifccParser.RETURN, 0); }
		public TerminalNode CONST() { return getToken(ifccParser.CONST, 0); }
		public Return_constContext(Return_stmtContext ctx) { copyFrom(ctx); }
	}

	public final Return_stmtContext return_stmt() throws RecognitionException {
		Return_stmtContext _localctx = new Return_stmtContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_return_stmt);
		try {
			setState(46);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,2,_ctx) ) {
			case 1:
				_localctx = new Return_constContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(40);
				match(RETURN);
				setState(41);
				match(CONST);
				setState(42);
				match(T__5);
				}
				break;
			case 2:
				_localctx = new Return_varContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(43);
				match(RETURN);
				setState(44);
				match(VAR);
				setState(45);
				match(T__5);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class Aff_stmtContext extends ParserRuleContext {
		public Aff_stmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_aff_stmt; }
	 
		public Aff_stmtContext() { }
		public void copyFrom(Aff_stmtContext ctx) {
			super.copyFrom(ctx);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Affectation_constContext extends Aff_stmtContext {
		public TerminalNode VAR() { return getToken(ifccParser.VAR, 0); }
		public TerminalNode CONST() { return getToken(ifccParser.CONST, 0); }
		public Affectation_constContext(Aff_stmtContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Affectation_varContext extends Aff_stmtContext {
		public List<TerminalNode> VAR() { return getTokens(ifccParser.VAR); }
		public TerminalNode VAR(int i) {
			return getToken(ifccParser.VAR, i);
		}
		public Affectation_varContext(Aff_stmtContext ctx) { copyFrom(ctx); }
	}

	public final Aff_stmtContext aff_stmt() throws RecognitionException {
		Aff_stmtContext _localctx = new Aff_stmtContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_aff_stmt);
		try {
			setState(56);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,3,_ctx) ) {
			case 1:
				_localctx = new Affectation_varContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(48);
				match(VAR);
				setState(49);
				match(T__6);
				setState(50);
				match(VAR);
				setState(51);
				match(T__5);
				}
				break;
			case 2:
				_localctx = new Affectation_constContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(52);
				match(VAR);
				setState(53);
				match(T__6);
				setState(54);
				match(CONST);
				setState(55);
				match(T__5);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class Def_stmtContext extends ParserRuleContext {
		public TerminalNode TYPE() { return getToken(ifccParser.TYPE, 0); }
		public TerminalNode VAR() { return getToken(ifccParser.VAR, 0); }
		public Def_stmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_def_stmt; }
	}

	public final Def_stmtContext def_stmt() throws RecognitionException {
		Def_stmtContext _localctx = new Def_stmtContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_def_stmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(58);
			match(TYPE);
			setState(59);
			match(VAR);
			setState(60);
			match(T__5);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class Def_aff_stmtContext extends ParserRuleContext {
		public Def_aff_stmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_def_aff_stmt; }
	 
		public Def_aff_stmtContext() { }
		public void copyFrom(Def_aff_stmtContext ctx) {
			super.copyFrom(ctx);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Definition_affectation_constContext extends Def_aff_stmtContext {
		public TerminalNode TYPE() { return getToken(ifccParser.TYPE, 0); }
		public TerminalNode VAR() { return getToken(ifccParser.VAR, 0); }
		public TerminalNode CONST() { return getToken(ifccParser.CONST, 0); }
		public Definition_affectation_constContext(Def_aff_stmtContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class Definition_affectation_varContext extends Def_aff_stmtContext {
		public TerminalNode TYPE() { return getToken(ifccParser.TYPE, 0); }
		public List<TerminalNode> VAR() { return getTokens(ifccParser.VAR); }
		public TerminalNode VAR(int i) {
			return getToken(ifccParser.VAR, i);
		}
		public Definition_affectation_varContext(Def_aff_stmtContext ctx) { copyFrom(ctx); }
	}

	public final Def_aff_stmtContext def_aff_stmt() throws RecognitionException {
		Def_aff_stmtContext _localctx = new Def_aff_stmtContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_def_aff_stmt);
		try {
			setState(72);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,4,_ctx) ) {
			case 1:
				_localctx = new Definition_affectation_constContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(62);
				match(TYPE);
				setState(63);
				match(VAR);
				setState(64);
				match(T__6);
				setState(65);
				match(CONST);
				setState(66);
				match(T__5);
				}
				break;
			case 2:
				_localctx = new Definition_affectation_varContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(67);
				match(TYPE);
				setState(68);
				match(VAR);
				setState(69);
				match(T__6);
				setState(70);
				match(VAR);
				setState(71);
				match(T__5);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\u0004\u0001\u000eK\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001\u0002"+
		"\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004\u0007\u0004\u0002"+
		"\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007\u0007\u0007\u0001"+
		"\u0000\u0001\u0000\u0001\u0000\u0001\u0001\u0001\u0001\u0001\u0001\u0001"+
		"\u0001\u0001\u0001\u0001\u0001\u0001\u0002\u0001\u0002\u0005\u0002\u001c"+
		"\b\u0002\n\u0002\f\u0002\u001f\t\u0002\u0001\u0002\u0001\u0002\u0001\u0003"+
		"\u0001\u0003\u0001\u0003\u0001\u0003\u0003\u0003\'\b\u0003\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0003\u0004"+
		"/\b\u0004\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005"+
		"\u0001\u0005\u0001\u0005\u0001\u0005\u0003\u00059\b\u0005\u0001\u0006"+
		"\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0007\u0001\u0007\u0001\u0007"+
		"\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007"+
		"\u0001\u0007\u0003\u0007I\b\u0007\u0001\u0007\u0000\u0000\b\u0000\u0002"+
		"\u0004\u0006\b\n\f\u000e\u0000\u0000I\u0000\u0010\u0001\u0000\u0000\u0000"+
		"\u0002\u0013\u0001\u0000\u0000\u0000\u0004\u0019\u0001\u0000\u0000\u0000"+
		"\u0006&\u0001\u0000\u0000\u0000\b.\u0001\u0000\u0000\u0000\n8\u0001\u0000"+
		"\u0000\u0000\f:\u0001\u0000\u0000\u0000\u000eH\u0001\u0000\u0000\u0000"+
		"\u0010\u0011\u0003\u0002\u0001\u0000\u0011\u0012\u0005\u0000\u0000\u0001"+
		"\u0012\u0001\u0001\u0000\u0000\u0000\u0013\u0014\u0005\b\u0000\u0000\u0014"+
		"\u0015\u0005\u0001\u0000\u0000\u0015\u0016\u0005\u0002\u0000\u0000\u0016"+
		"\u0017\u0005\u0003\u0000\u0000\u0017\u0018\u0003\u0004\u0002\u0000\u0018"+
		"\u0003\u0001\u0000\u0000\u0000\u0019\u001d\u0005\u0004\u0000\u0000\u001a"+
		"\u001c\u0003\u0006\u0003\u0000\u001b\u001a\u0001\u0000\u0000\u0000\u001c"+
		"\u001f\u0001\u0000\u0000\u0000\u001d\u001b\u0001\u0000\u0000\u0000\u001d"+
		"\u001e\u0001\u0000\u0000\u0000\u001e \u0001\u0000\u0000\u0000\u001f\u001d"+
		"\u0001\u0000\u0000\u0000 !\u0005\u0005\u0000\u0000!\u0005\u0001\u0000"+
		"\u0000\u0000\"\'\u0003\n\u0005\u0000#\'\u0003\f\u0006\u0000$\'\u0003\u000e"+
		"\u0007\u0000%\'\u0003\b\u0004\u0000&\"\u0001\u0000\u0000\u0000&#\u0001"+
		"\u0000\u0000\u0000&$\u0001\u0000\u0000\u0000&%\u0001\u0000\u0000\u0000"+
		"\'\u0007\u0001\u0000\u0000\u0000()\u0005\t\u0000\u0000)*\u0005\n\u0000"+
		"\u0000*/\u0005\u0006\u0000\u0000+,\u0005\t\u0000\u0000,-\u0005\u000b\u0000"+
		"\u0000-/\u0005\u0006\u0000\u0000.(\u0001\u0000\u0000\u0000.+\u0001\u0000"+
		"\u0000\u0000/\t\u0001\u0000\u0000\u000001\u0005\u000b\u0000\u000012\u0005"+
		"\u0007\u0000\u000023\u0005\u000b\u0000\u000039\u0005\u0006\u0000\u0000"+
		"45\u0005\u000b\u0000\u000056\u0005\u0007\u0000\u000067\u0005\n\u0000\u0000"+
		"79\u0005\u0006\u0000\u000080\u0001\u0000\u0000\u000084\u0001\u0000\u0000"+
		"\u00009\u000b\u0001\u0000\u0000\u0000:;\u0005\b\u0000\u0000;<\u0005\u000b"+
		"\u0000\u0000<=\u0005\u0006\u0000\u0000=\r\u0001\u0000\u0000\u0000>?\u0005"+
		"\b\u0000\u0000?@\u0005\u000b\u0000\u0000@A\u0005\u0007\u0000\u0000AB\u0005"+
		"\n\u0000\u0000BI\u0005\u0006\u0000\u0000CD\u0005\b\u0000\u0000DE\u0005"+
		"\u000b\u0000\u0000EF\u0005\u0007\u0000\u0000FG\u0005\u000b\u0000\u0000"+
		"GI\u0005\u0006\u0000\u0000H>\u0001\u0000\u0000\u0000HC\u0001\u0000\u0000"+
		"\u0000I\u000f\u0001\u0000\u0000\u0000\u0005\u001d&.8H";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}