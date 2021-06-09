```
topLevelObject
  : declaration semis?
  ;
  
declaration (used by topLevelObject, classMemberDeclaration, statement)
  : classDeclaration
  | functionDeclaration
  | propertyDeclaration
  ;
  ```

# Classes

```
classDeclaration (used by declaration)
  : 'class'
    simpleIdentifier
    classBody?
  ;

classBody (used by classDeclaration)
  : '{' (declaration semis?)* '}'
  ;
  ```

# Class members

```
functionValueParameters (used by functionDeclaration)
  : '(' (functionValueParameter (',' functionValueParameter)* ','?)? ')'
  ;

functionValueParameter (used by functionValueParameters)
  : parameter ('=' expression)?
  ;
  
parameter (used by functionValueParameter)
  : simpleIdentifier ':' type
  ;

functionDeclaration (used by declaration)
  : 'fun'
    simpleIdentifier functionValueParameters
    (':' type)?
    functionBody
  ;

functionBody (used by functionDeclaration)
  : block
  | '=' expression
  ;

variableDeclaration (used by propertyDeclaration, forStatement)
  : simpleIdentifier (':' type)?
  ;

propertyDeclaration (used by declaration)
  : ('val' | 'var') variableDeclaration
    ('=' expression)? ';'?
  ;
  ```

# Types

```
type (used by functionDeclaration, variableDeclaration, parameter, parenthesizedType)
  : (parenthesizedType | userType)
  ;

userType (used by type)
  : simpleIdentifier ('.' simpleIdentifier)*
  ;

parenthesizedType (used by type)
  : '(' type ')'
  ;
  ```

# Statements

```
statements (used by block)
  : (statement (semis statement)*)? semis?
  ;

statement (used by statements, controlStructureBody)
  : (declaration | assignment | loopStatement | expression)
  ;

controlStructureBody (used by forStatement, whileStatement, doWhileStatement, ifExpression)
  : block
  | statement
  ;

block (used by functionBody, controlStructureBody)
  : '{' statements '}'
  ;

loopStatement (used by statement)
  : forStatement
  | whileStatement
  | doWhileStatement
  ;

forStatement (used by loopStatement)
  : 'for'
    '(' variableDeclaration 'in' expression ')'
    controlStructureBody
  ;

whileStatement (used by loopStatement)
  : 'while' '(' expression ')' (controlStructureBody | ';')
  ;

doWhileStatement (used by loopStatement)
  : 'do' controlStructureBody 'while' '(' expression ')'
  ;

assignment (used by statement)
  : ((directlyAssignableExpression '=') | (assignableExpression assignmentAndOperator)) expression
  ;

semi
  : (';' | NL) NL*
  | EOF
  ;

semis
  : (';' | NL)+
  | EOF
  ;
```

# Expressions

```
expression (used by classParameter, functionValueParameter, functionBody, propertyDeclaration, statement, forStatement, whileStatement, doWhileStatement, assignment, indexingSuffix, valueArgument, parenthesizedExpression, lineStringExpression, multiLineStringExpression, ifExpression, rangeTest)
  : disjunction
  ;

disjunction (used by expression)
  : conjunction ('||' conjunction)*
  ;

conjunction (used by disjunction)
  : equality ('&&' equality)*
  ;

equality (used by conjunction)
  : comparison (equalityOperator comparison)*
  ;

comparison (used by equality)
  : genericCallLikeComparison (comparisonOperator genericCallLikeComparison)*
  ;

genericCallLikeComparison (used by comparison)
  : infixOperation callSuffix*
  ;

infixOperation (used by genericCallLikeComparison)
  : infixFunctionCall (inOperator infixFunctionCall)*
  ;

infixFunctionCall (used by infixOperation)
  : rangeExpression (simpleIdentifier rangeExpression)*
  ;

rangeExpression (used by infixFunctionCall)
  : additiveExpression ('..' additiveExpression)*
  ;
  
additiveExpression (used by rangeExpression)
  : multiplicativeExpression (additiveOperator multiplicativeExpression)*
  ;
  
multiplicativeExpression (used by additiveExpression)
  : prefixUnaryExpression (multiplicativeOperator prefixUnaryExpression)*
  ;

prefixUnaryExpression (used by assignableExpression)
  : prefixUnaryOperator* postfixUnaryExpression
  ;

postfixUnaryExpression (used by prefixUnaryExpression, directlyAssignableExpression)
  : primaryExpression postfixUnarySuffix*
  ;

postfixUnarySuffix (used by postfixUnaryExpression)
  : postfixUnaryOperator
  | callSuffix
  | indexingSuffix
  | navigationSuffix
  ;
  
directlyAssignableExpression (used by assignment, parenthesizedDirectlyAssignableExpression)
  : postfixUnaryExpression assignableSuffix
  | simpleIdentifier
  | parenthesizedDirectlyAssignableExpression
  ;

parenthesizedDirectlyAssignableExpression (used by directlyAssignableExpression)
  : '(' directlyAssignableExpression ')'
  ;

assignableExpression (used by assignment, parenthesizedAssignableExpression)
  : prefixUnaryExpression
  | parenthesizedAssignableExpression
  ;

parenthesizedAssignableExpression (used by assignableExpression)
  : '(' assignableExpression ')'
  ;

assignableSuffix (used by directlyAssignableExpression)
  : indexingSuffix
  | navigationSuffix
  ;

indexingSuffix (used by postfixUnarySuffix, assignableSuffix)
  : '[' expression (',' expression)* ','? ']'
  ;

navigationSuffix (used by postfixUnarySuffix, assignableSuffix)
  : '.' simpleIdentifier
  ;

callSuffix (used by genericCallLikeComparison, postfixUnarySuffix)
  : '(' (valueArgument (',' valueArgument)* ','?)? ')'
  ;

valueArgument (used by valueArguments)
  : (simpleIdentifier '=')? expression
  ;

primaryExpression (used by postfixUnaryExpression)
  : parenthesizedExpression
  | simpleIdentifier
  | literalConstant
  | stringLiteral
  | ifExpression
  ;

parenthesizedExpression (used by navigationSuffix, primaryExpression)
  : '(' expression ')'
  ;

literalConstant (used by primaryExpression)
  : BooleanLiteral
  | IntegerLiteral
  | HexLiteral
  | BinLiteral
  | CharacterLiteral
  | RealLiteral
  | LongLiteral
  | UnsignedLiteral
  ;

stringLiteral (used by primaryExpression)
  : LineStringLiteral
  | MultiLineStringLiteral
  ;

ifExpression (used by primaryExpression)
  : 'if' '(' expression ')'
    (controlStructureBody | (controlStructureBody? ';'? 'else' (controlStructureBody | ';')) | ';')
  ;

assignmentAndOperator (used by assignment)
  : '+='
  | '-='
  | '*='
  | '/='
  | '%='
  ;
equalityOperator (used by equality)
  : '!='
  | '!=='
  | '=='
  | '==='
  ;
comparisonOperator (used by comparison)
  : '<'
  | '>'
  | '<='
  | '>='
  ;
  
additiveOperator (used by additiveExpression)
  : '+'
  | '-'
  ;
multiplicativeOperator (used by multiplicativeExpression)
  : '*'
  | '/'
  | '%'
  ;

prefixUnaryOperator (used by unaryPrefix)
  : '++'
  | '--'
  | '-'
  | '+'
  | excl
  ;
postfixUnaryOperator (used by postfixUnarySuffix)
  : '++'
  | '--'
  ;
excl (used by prefixUnaryOperator, postfixUnaryOperator)
  : '!'
  | EXCL_WS
  ;
  ```

# Identifiers

```
simpleIdentifier (used by classDeclaration, functionDeclaration, variableDeclaration, parameter, userType, infixFunctionCall, directlyAssignableExpression, navigationSuffix, valueArgument, primaryExpression, identifier)
  : Identifier
  | 'abstract'
  | 'annotation'
  | 'by'
  | 'catch'
  | 'companion'
  | 'constructor'
  | 'crossinline'
  | 'data'
  | 'dynamic'
  | 'enum'
  | 'external'
  | 'final'
  | 'finally'
  | 'get'
  | 'import'
  | 'infix'
  | 'init'
  | 'inline'
  | 'inner'
  | 'internal'
  | 'lateinit'
  | 'noinline'
  | 'open'
  | 'operator'
  | 'out'
  | 'override'
  | 'private'
  | 'protected'
  | 'public'
  | 'reified'
  | 'sealed'
  | 'tailrec'
  | 'set'
  | 'vararg'
  | 'where'
  | 'field'
  | 'property'
  | 'receiver'
  | 'param'
  | 'setparam'
  | 'delegate'
  | 'file'
  | 'expect'
  | 'actual'
  | 'const'
  | 'suspend'
  | 'value'
  ;

identifier (used by packageHeader, importHeader)
  : simpleIdentifier ('.' simpleIdentifier)*
  ;
