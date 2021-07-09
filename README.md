# KotlinCompiler
Compiler of simplified version of Kotlin language

## Implemented:
<ul>
  <li> Lexer </li>
  <li> Parser </li>
  <li> Semantic analyzer </li>
  <li> Interpreter </li>
</ul>

You can check grammar [on a special page](GRAMMAR.md)

## Notable differences from original Kotlin:
<ul>
	<li> Every entity should be declared before the first usage; </li>
	<li> Function return type should be explicit, if it is not 'Unit'; </li>
	<li> No 'this' keyword, no visibility / abstract modifiers, no constructors and init block, no nested classes; </li>
	<li> No lambdas, object literals; </li>
	<li> No Boolean ranges (yes they are exist) and progressions (i.e. 'step' and 'downTo' keyword are not supported) </li>
	<li> Jump expressions are now statements. </li>
</ul>

## Tests:
Tests are implemented with Catch2. For interpreter tests download official Kotlin compiler (tested with native version) and build Compiler on Release configuration first. Only then launch tests.

## Author
Yuriy Mikhalev - 3rd year student of Applied Mathematics and Informatics (Far Eastern Federal University, Б8118-01.03.02систпро)
