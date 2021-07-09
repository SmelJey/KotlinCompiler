# KotlinCompiler
Compiler for simplified subset of [Kotlin language](https://kotlinlang.org/spec/introduction.html)

## Implemented:
<ul>
  <li> Lexer, </li>
  <li> Parser, </li>
  <li> Semantic analyzer, </li>
  <li> Interpreter. </li>
</ul>

You can check grammar [on the special page](GRAMMAR.md)

## Notable differences from original Kotlin:
<ul>
	<li> Only next built-in types are supported: 'Int', 'Double', 'Boolean', 'String', 'Array' and 'ClosedRange'. You can declare your own types via 'class' keyword; </li>
	<li> Built-in types have only casts (toInt(), toString(), etc) and basic operators (+, -, &&, index operator for arrays, etc) as only available members; </li>
	<li> String index operator is not supported; </li>
	<li> Operator overloading is not supported; </li>
	<li> 'in' and '!in' are only supported infix functions (for example, bitwise operators ('and', 'or', 'shl', etc) are not supported); </li>
	<li> Every entity should be declared before the first usage; </li>
	<li> Return type of every function should be explicit if it is other than 'Unit'; </li>
	<li> Visibility / abstract modifier, 'this' keyword, primary and secondary constructors, init block, nested classes are not supported; </li>
	<li> Lambdas, function types, object literals and generics (other than ClosedRange and Array) are not supported; </li>
	<li> Boolean ranges (yes, they are exist) and progressions (i.e. range with 'step' or 'downTo' keyword) are not supported; </li>
	<li> Jumps (e.g. 'break', 'continue', 'return') can not be used as expressions. </li>
</ul>

## Installation
Open .sln file in Visual Studio and build entire solution. The compiler executable (KotlinCompiler.exe) with Core.dll would lie in Debug / Release folder in the solution directory.

## Usage
```
KotlinCompiler.exe [flags] <source file>
```
Next flags are supported:
<ul>
	<li> '-l' or '--lexer-debug' -- show lexer's output (stream of tokens); </li>
	<li> '-p' or '--parser-debug' -- show parser's output (syntax tree) </li>
	<li> '-s' or '--semantics-debug' -- show semantics analyzer's output (semantics annotations on syntax tree and symbol table) </li>
</ul>

## Tests:
Tests are implemented with [Catch2](https://github.com/catchorg/Catch2). In order to launch interpreter tests, install [the official Kotlin compiler](https://kotlinlang.org/docs/command-line.html#compile-a-library) (tested with native version) and build Compiler on Release configuration first. Then you are ready to go.

## Author
Yuriy Mikhalev - 3rd year student of Applied Mathematics and Informatics (Far Eastern Federal University, Б8118-01.03.02систпро)
