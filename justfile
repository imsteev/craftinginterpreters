gen:
    javac com/craftinginterpreters/lox/tool/GenerateAst.java
    java com.craftinginterpreters.lox.tool.GenerateAst com/craftinginterpreters/lox

print:
    javac com/craftinginterpreters/lox/AstPrinter.java
    java com.craftinginterpreters.lox.AstPrinter com/craftinginterpreters/lox

repl:
    javac com/craftinginterpreters/lox/Lox.java
    java com.craftinginterpreters.lox.Lox
