// Generated from /mnt/c/Users/aurel/Desktop/Cours/4A/COMP/PLD-Compilateur/compiler/ifcc.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link ifccParser}.
 */
public interface ifccListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link ifccParser#axiom}.
	 * @param ctx the parse tree
	 */
	void enterAxiom(ifccParser.AxiomContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifccParser#axiom}.
	 * @param ctx the parse tree
	 */
	void exitAxiom(ifccParser.AxiomContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifccParser#prog}.
	 * @param ctx the parse tree
	 */
	void enterProg(ifccParser.ProgContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifccParser#prog}.
	 * @param ctx the parse tree
	 */
	void exitProg(ifccParser.ProgContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifccParser#bloc}.
	 * @param ctx the parse tree
	 */
	void enterBloc(ifccParser.BlocContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifccParser#bloc}.
	 * @param ctx the parse tree
	 */
	void exitBloc(ifccParser.BlocContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifccParser#ins_set}.
	 * @param ctx the parse tree
	 */
	void enterIns_set(ifccParser.Ins_setContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifccParser#ins_set}.
	 * @param ctx the parse tree
	 */
	void exitIns_set(ifccParser.Ins_setContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifccParser#ins}.
	 * @param ctx the parse tree
	 */
	void enterIns(ifccParser.InsContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifccParser#ins}.
	 * @param ctx the parse tree
	 */
	void exitIns(ifccParser.InsContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifccParser#return_stmt}.
	 * @param ctx the parse tree
	 */
	void enterReturn_stmt(ifccParser.Return_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifccParser#return_stmt}.
	 * @param ctx the parse tree
	 */
	void exitReturn_stmt(ifccParser.Return_stmtContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifccParser#aff_stmt}.
	 * @param ctx the parse tree
	 */
	void enterAff_stmt(ifccParser.Aff_stmtContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifccParser#aff_stmt}.
	 * @param ctx the parse tree
	 */
	void exitAff_stmt(ifccParser.Aff_stmtContext ctx);
}