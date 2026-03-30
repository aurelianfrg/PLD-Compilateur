/mnt/c/Users/aurel/.venv/bin/hpp2plantuml \
-i compiler/IR.h \
-i compiler/SymbolsTable.h \
-i compiler/Symbol.h \
-i compiler/Type.h \
-i compiler/Function.h \
-i compiler/FunctionsTable.h \
-o class_diagram.puml

plantuml -tpng class_diagram.puml
plantuml -tsvg class_diagram.puml