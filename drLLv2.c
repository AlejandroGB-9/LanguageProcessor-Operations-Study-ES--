#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define T_NUMBER 	1001
#define T_OPERATOR	1002		

int ParseExpression () ;		// Prototype for forward reference

int token ;		// Here we store the current token/literal 
int number ;		// the value of the number 
int token_val ;		// or the arithmetic operator
			// TO DO: Pack these variables in a struct

int line_counter = 1 ;
int *array;

int rd_lex ()
{
	int c ;
	
	do {
		c = getchar () ;
		printf("%d\n", c);
		if (c == '\n')
			line_counter++ ;	// info for rd_syntax_error()
	} while (c == ' ' || c == '\t' || c == '\r') ;

	if (isdigit (c)) {
		ungetc (c, stdin) ;
		scanf ("%d", &number) ;
		token = T_NUMBER ;
		return (token) ;	// returns the Token for Number
	}

	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '!') {
		token_val = c ;
		token = T_OPERATOR ;
		return (token) ;
	}				// returns the Token for Arithmetic Operators
	
	token = c ;
	return (token) ;		// returns a literal
}


void rd_syntax_error (int expected, int token, char *output) 
{
	fprintf (stderr, "ERROR in line %d ", line_counter) ;
	fprintf (stderr, output, expected, token) ;

	exit (0) ;
}


void MatchSymbol (int expected_token)
{
	if (token != expected_token) {
		rd_syntax_error (expected_token, token, "token %d expected, but %d was read") ;
	}
	rd_lex () ;
}


#define ParseLParen() 	MatchSymbol ('(') ; // More concise and efficient definitions
#define ParseRParen() 	MatchSymbol (')') ; // rather than using functions
					// This is only useful for matching Literals

int ParseIndex()
{
	/*Para devolver el index de la variable en el array*/
	int ascii = token;
	if (ascii>=65 || ascii <= 90)
	{
		/*Letras mayusculas*/
		int posicion = ascii - 65;
		rd_lex () ;
		return posicion;
	}
	else if (ascii>=97 || ascii <= 122)
	{
		/*Letras mayusculas*/
		int posicion = ascii - 97 + 26;
		rd_lex () ;
		return posicion;
	}
}

int ParseVariable ()
{
	/*Para devolver el valor de una variable*/
	int pos = ParseIndex();
	int val = array[pos];
	return val;
}

int ParseNumber()
{

	int val;
	val = number ;	    // store number value to avoid losing it when reading
	MatchSymbol (T_NUMBER) ;
	return val;

}

int ParseTerm ()
{
	int val;
	
	if (token == T_NUMBER) {	
		
		val = ParseNumber () ;
		return val;

	} else {

		val = ParseVariable () ;
		return val;

	}

}

int ParseType ()
{
	int val;
	if (token == '(') {

		ParseLParen () ;
		val = ParseExpression () ;
		ParseRParen () ;
		return val;

	} else if (token == T_NUMBER || ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z'))){

		val = ParseTerm () ;
		return val;

	}
}

int ParseCollection (int op)
{

	int val;
	int val2;

	if (token == T_NUMBER || ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z'))){

		val = ParseTerm () ;

	} else {

		ParseLParen () ;
		val = ParseExpression () ;
		ParseRParen () ;

	}

	while(1){
		if (token == '\n' || token == ')'){
			printf("%d FIN \n", val) ;
			return val;

		} else if (token == T_NUMBER || ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z'))){

			val2 = ParseTerm () ;

		} else {

			ParseLParen () ;
			val2 = ParseExpression () ;
			ParseRParen () ;

		}

		switch (op){			// This part is for the Semantic actions
		case '+' :  val += val2 ;
			    	break ;
		case '-' :  val += val2 ;
			    	break ;
		case '*' :  val *= val2 ;
			    	break ;
		case '/' :  val *= val2 ;
			    	break ;
		default :   rd_syntax_error (op, 0, "Unexpected error in ParseExpressionRest for operator %c\n") ;
			    	break ;
		}

		printf("%d\n", val) ;
	}
}

int ParseOperator () 
{
	int val = token_val ;
	MatchSymbol (T_OPERATOR) ;
	return val ;
}


int ParseOpExpression ()
{
	int val ;
	int val2 ;
	int operator ;

	if (token == T_NUMBER || ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z'))){	
		
		val = ParseTerm () ;
		val2 = ParseCollection (operator) ;	
		printf("%d\n", val) ;
		printf("%d\n", val2) ;

	} else {
		
		ParseLParen () ;
		val = ParseExpression () ;
		ParseRParen () ;
		val2 = ParseCollection (operator) ;


	}

	switch (operator){			// This part is for the Semantic actions
		case '+' :  val += val2 ;
			    	break ;
		case '-' :  val -= val2 ;
			    	break ;
		case '*' :  val *= val2 ;
			    	break ;
		case '/' :  val /= val2 ;
			    	break ;
		default :   rd_syntax_error (operator, 0, "Unexpected error in ParseExpressionRest for operator %c\n") ;
			    	break ;
	}

	printf("%d\n", val) ;
	return val;

}

int ParseExpression () 		
{							
	int val ;
	int operator ;
	operator = ParseOperator () ;
	val=ParseOpExpression () ;
	return val;
}

int ParseAxiom () 		
{
	int val ;
	int index;

	if(token == T_OPERATOR && token_val == '!'){
		ParseOperator () ;
		index = ParseIndex () ;
		val = ParseType ();
		array[index] = val;
		return val;

	} else {

		val = ParseType ();
		return val ;

	}
}


int main (int argc, char *argv[])
{
	array=(int*)malloc(52 * sizeof(int)); /*52 porque tenemos 52 posibles variables*/
	if (argc == 2){
		
		freopen(argv[1], "r", stdin);
		while (1) {
			rd_lex () ;
			printf ("Valor %d\n", ParseAxiom ()) ;
		}

	} else {

		while (1) {
			rd_lex () ;
			printf ("Valor %d\n", ParseAxiom ()) ;
		}

	}	
	
	system ("PAUSE") ;
	free(array);
}