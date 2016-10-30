grammar scs;

options 
{
    k = 3;
    language = C;
    output = AST;
    ASTLabelType=pANTLR3_BASE_TREE;
}

@lexer::includes {
    #include "../parseutils.h"
}

@parser::includes {
    #include "../parseutils.h"
}

@parser::apifuncs {
    RECOGNIZER->displayRecognitionError = displayRecognitionError;
}

@lexer::apifuncs {
    LEXER->rec->displayRecognitionError = displayLexerError;
}

@rulecatch
{
    if (HASEXCEPTION())
    {
        parseError((const char*)EXCEPTION->name, EXCEPTION->line);
    }
}

/* Parser rules */
syntax
    : (sentence ';;'!)* EOF
    ;
       
sentence
	// level 1
    : sentence_lvl1
	| sentence_lvl_common
    ;
    
idtf_lvl1
	:	
	    (('sc_node' |
		  'sc_link' |
		  'sc_arc_common' |
		  'sc_edge' |
		  'sc_arc_main' |
		  'sc_arc_access') '#')? ID_SYSTEM
	;

idtf_edge
	: '(' ID_SYSTEM CONNECTORS ID_SYSTEM ')'
	;
	
idtf_set
	: '{' attr_list? idtf_common (';' attr_list? idtf_common)* '}'
	;

idtf_common
	: ID_SYSTEM
	| idtf_edge
	| idtf_set
	| CONTENT
	| LINK
	;

idtf_list
	: idtf_common internal_sentece_list? (';' idtf_common internal_sentece_list?)*
	;
	
internal_sentence
	: CONNECTORS attr_list? idtf_list
	;
	
internal_sentece_list
	: '(*' (internal_sentence ';;')+ '*)'
	;

sentence_lvl1
 	: idtf_lvl1 '|' idtf_lvl1 '|' idtf_lvl1
 	;


sentence_lvl_common
	: idtf_common CONNECTORS attr_list? idtf_list
	;

attr_list
	: (ID_SYSTEM (':'|'::'))+
	;

// --------------- separators -----------------


ID_SYSTEM
    :   ('a'..'z'|'A'..'Z'|'_'|'.'|'0'..'9')+
    ;

COMMENT
    :   '//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    |   ('/*' | '/!*') ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
    ;


LINK
     :  '"' (   ~('"')  | '\\"'  )* '"'
     ;
CONTENT
    @init{int count = 1;}
  	: '_'? '['
  	  (
  	  { count > 0 }? =>
  	   	 (
  		  ~ ('[' | ']')
	  	  | '[' { count++; } 
  		  | ']' { count--; }
	  	  )
	  )*
  	;
    
CONNECTORS
	 :  ( 
         '<>'  | '>'   | '<'   |
         '..>' | '<..' | '->'  |
         '<-'  | '<=>' | '=>'  | '<=' |
         '-|>' | '<|-' | '-/>' | '</-' |
         '~>'  | '<~'  | '~|>' | '<|~' |
         '~/>' | '</~' | '_<>' | '_>'  |
         '_<'  | '_..>'| '_<..'| '_->' |
         '_<-' | '_<=>'| '_=>' | '_<=' |
         '_-|>'| '_<|-'| '_-/>'| '_</-'|
         '_~>' | '_<~' | '_~|>'| '_<|~'|
         '_~/>'| '_</~'
         );

fragment
HEX_DIGIT : ('0'..'9'|'a'..'f'|'A'..'F') ;

WS  :   ( ' '
        | '\t'
        | '\r'
        | '\n'
        ) {$channel=HIDDEN;}
    ;

fragment
OCTAL_ESC
    :   '\\' ('0'..'3') ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7')
    ;

fragment
UNICODE_ESC
    :   '\\' 'u' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT
    ;

