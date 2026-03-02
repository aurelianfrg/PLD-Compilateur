// Generated from /mnt/c/Users/aurel/Desktop/Cours/4A/COMP/PLD-Compilateur/compiler/ifcc.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue", "this-escape"})
public class ifccLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.13.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, TYPE=8, RETURN=9, 
		CONST=10, VAR=11, COMMENT=12, DIRECTIVE=13, WS=14;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "TYPE", "RETURN", 
			"CONST", "VAR", "COMMENT", "DIRECTIVE", "WS"
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


	public ifccLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "ifcc.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\u0004\u0000\u000eb\u0006\uffff\uffff\u0002\u0000\u0007\u0000\u0002\u0001"+
		"\u0007\u0001\u0002\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004"+
		"\u0007\u0004\u0002\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007"+
		"\u0007\u0007\u0002\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b"+
		"\u0007\u000b\u0002\f\u0007\f\u0002\r\u0007\r\u0001\u0000\u0001\u0000\u0001"+
		"\u0000\u0001\u0000\u0001\u0000\u0001\u0001\u0001\u0001\u0001\u0002\u0001"+
		"\u0002\u0001\u0003\u0001\u0003\u0001\u0004\u0001\u0004\u0001\u0005\u0001"+
		"\u0005\u0001\u0006\u0001\u0006\u0001\u0007\u0001\u0007\u0001\u0007\u0001"+
		"\u0007\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\t"+
		"\u0004\t;\b\t\u000b\t\f\t<\u0001\n\u0001\n\u0005\nA\b\n\n\n\f\nD\t\n\u0001"+
		"\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0005\u000bJ\b\u000b\n\u000b"+
		"\f\u000bM\t\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0001\u000b\u0001"+
		"\u000b\u0001\f\u0001\f\u0005\fV\b\f\n\f\f\fY\t\f\u0001\f\u0001\f\u0001"+
		"\f\u0001\f\u0001\r\u0001\r\u0001\r\u0001\r\u0002KW\u0000\u000e\u0001\u0001"+
		"\u0003\u0002\u0005\u0003\u0007\u0004\t\u0005\u000b\u0006\r\u0007\u000f"+
		"\b\u0011\t\u0013\n\u0015\u000b\u0017\f\u0019\r\u001b\u000e\u0001\u0000"+
		"\u0004\u0001\u000009\u0003\u0000AZ__az\u0004\u000009AZ__az\u0003\u0000"+
		"\t\n\r\r  e\u0000\u0001\u0001\u0000\u0000\u0000\u0000\u0003\u0001\u0000"+
		"\u0000\u0000\u0000\u0005\u0001\u0000\u0000\u0000\u0000\u0007\u0001\u0000"+
		"\u0000\u0000\u0000\t\u0001\u0000\u0000\u0000\u0000\u000b\u0001\u0000\u0000"+
		"\u0000\u0000\r\u0001\u0000\u0000\u0000\u0000\u000f\u0001\u0000\u0000\u0000"+
		"\u0000\u0011\u0001\u0000\u0000\u0000\u0000\u0013\u0001\u0000\u0000\u0000"+
		"\u0000\u0015\u0001\u0000\u0000\u0000\u0000\u0017\u0001\u0000\u0000\u0000"+
		"\u0000\u0019\u0001\u0000\u0000\u0000\u0000\u001b\u0001\u0000\u0000\u0000"+
		"\u0001\u001d\u0001\u0000\u0000\u0000\u0003\"\u0001\u0000\u0000\u0000\u0005"+
		"$\u0001\u0000\u0000\u0000\u0007&\u0001\u0000\u0000\u0000\t(\u0001\u0000"+
		"\u0000\u0000\u000b*\u0001\u0000\u0000\u0000\r,\u0001\u0000\u0000\u0000"+
		"\u000f.\u0001\u0000\u0000\u0000\u00112\u0001\u0000\u0000\u0000\u0013:"+
		"\u0001\u0000\u0000\u0000\u0015>\u0001\u0000\u0000\u0000\u0017E\u0001\u0000"+
		"\u0000\u0000\u0019S\u0001\u0000\u0000\u0000\u001b^\u0001\u0000\u0000\u0000"+
		"\u001d\u001e\u0005m\u0000\u0000\u001e\u001f\u0005a\u0000\u0000\u001f "+
		"\u0005i\u0000\u0000 !\u0005n\u0000\u0000!\u0002\u0001\u0000\u0000\u0000"+
		"\"#\u0005(\u0000\u0000#\u0004\u0001\u0000\u0000\u0000$%\u0005)\u0000\u0000"+
		"%\u0006\u0001\u0000\u0000\u0000&\'\u0005{\u0000\u0000\'\b\u0001\u0000"+
		"\u0000\u0000()\u0005}\u0000\u0000)\n\u0001\u0000\u0000\u0000*+\u0005;"+
		"\u0000\u0000+\f\u0001\u0000\u0000\u0000,-\u0005=\u0000\u0000-\u000e\u0001"+
		"\u0000\u0000\u0000./\u0005i\u0000\u0000/0\u0005n\u0000\u000001\u0005t"+
		"\u0000\u00001\u0010\u0001\u0000\u0000\u000023\u0005r\u0000\u000034\u0005"+
		"e\u0000\u000045\u0005t\u0000\u000056\u0005u\u0000\u000067\u0005r\u0000"+
		"\u000078\u0005n\u0000\u00008\u0012\u0001\u0000\u0000\u00009;\u0007\u0000"+
		"\u0000\u0000:9\u0001\u0000\u0000\u0000;<\u0001\u0000\u0000\u0000<:\u0001"+
		"\u0000\u0000\u0000<=\u0001\u0000\u0000\u0000=\u0014\u0001\u0000\u0000"+
		"\u0000>B\u0007\u0001\u0000\u0000?A\u0007\u0002\u0000\u0000@?\u0001\u0000"+
		"\u0000\u0000AD\u0001\u0000\u0000\u0000B@\u0001\u0000\u0000\u0000BC\u0001"+
		"\u0000\u0000\u0000C\u0016\u0001\u0000\u0000\u0000DB\u0001\u0000\u0000"+
		"\u0000EF\u0005/\u0000\u0000FG\u0005*\u0000\u0000GK\u0001\u0000\u0000\u0000"+
		"HJ\t\u0000\u0000\u0000IH\u0001\u0000\u0000\u0000JM\u0001\u0000\u0000\u0000"+
		"KL\u0001\u0000\u0000\u0000KI\u0001\u0000\u0000\u0000LN\u0001\u0000\u0000"+
		"\u0000MK\u0001\u0000\u0000\u0000NO\u0005*\u0000\u0000OP\u0005/\u0000\u0000"+
		"PQ\u0001\u0000\u0000\u0000QR\u0006\u000b\u0000\u0000R\u0018\u0001\u0000"+
		"\u0000\u0000SW\u0005#\u0000\u0000TV\t\u0000\u0000\u0000UT\u0001\u0000"+
		"\u0000\u0000VY\u0001\u0000\u0000\u0000WX\u0001\u0000\u0000\u0000WU\u0001"+
		"\u0000\u0000\u0000XZ\u0001\u0000\u0000\u0000YW\u0001\u0000\u0000\u0000"+
		"Z[\u0005\n\u0000\u0000[\\\u0001\u0000\u0000\u0000\\]\u0006\f\u0000\u0000"+
		"]\u001a\u0001\u0000\u0000\u0000^_\u0007\u0003\u0000\u0000_`\u0001\u0000"+
		"\u0000\u0000`a\u0006\r\u0001\u0000a\u001c\u0001\u0000\u0000\u0000\u0005"+
		"\u0000<BKW\u0002\u0006\u0000\u0000\u0000\u0001\u0000";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}