gen:
    javac com/craftinginterpreters/lox/tool/GenerateAst.java
    java com.craftinginterpreters.lox.tool.GenerateAst com/craftinginterpreters/lox

print:
    javac com/craftinginterpreters/lox/AstPrinter.java
    java com.craftinginterpreters.lox.AstPrinter com/craftinginterpreters/lox

repl:
    javac com/craftinginterpreters/lox/*.java
    java com.craftinginterpreters.lox.Lox

run file:
    javac com/craftinginterpreters/lox/*.java
    java com.craftinginterpreters.lox.Lox {{file}}

samples:
    javac com/craftinginterpreters/lox/*.java
    @echo "Running all sample lox files..."
    @for script in samples/basics/*.lox samples/functions/*.lox samples/classes/*.lox; do \
        echo ""; \
        echo "=== Running $script ==="; \
        java com.craftinginterpreters.lox.Lox "$script" || echo "Failed: $script"; \
    done
