#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define T_NUMBER 	1001
#define T_OPERATOR	1002		

int ParseExpression () ;		// Prototype for forward reference
int ParseParameter (int op) ;		// Prototype for forward reference

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
#define ParseEqual() 	MatchSymbol (T_OPERATOR) ; 
					// This is only useful for matching Literals

int SearchIndex()
{
	/*Para devolver el index que le corresponde a la variable en el array*/
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
		/*Letras minusculas*/
		int posicion = ascii - 97 + 26;
		rd_lex () ;
		return posicion;
	}
}

int ParseVariable ()
{
	/*Esta funcion devuelve el valor de una variable en el array*/
	int pos = SearchIndex(); //Primero se obtiene el index de la variable
	int val = array[pos];	//Una vez obetenido el index se obtiene el valor de esa variable en el array
	return val;
}

int ParseNumber()
{
	/*Funcion que devuelve el valor del numero leido*/
	int val;
	val = number ;	    // store number value to avoid losing it when reading
	MatchSymbol (T_NUMBER) ;
	return val;

}

int ParseTerm ()
{
	int val;
	
	if (token == T_NUMBER) {	
		/*Si el valor del token leido es un numero, se ejecuta el ParseNumber() para obtener el valor de dicho numero*/
		val = ParseNumber () ;
		return val;

	} else {
		/*En cualquier otro caso, se tratara de una variable, por lo que se ejecuta ParseVariable() para obtener su valor*/
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

int ParseOpParameter (int op)
{
	int val;
	int val2;
	val = ParseType () ;
	if (token == '\n' || token == ')'){

		return val;

	} else {
		val2 = ParseParameter (op) ;
		switch (op){			
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
	}
	return val;

}

int ParseParameter (int op)
{
	int val;
	int val2;
	val = ParseType () ;
	if (token == '\n' || token == ')'){

		return val;

	} else {
		val2 = ParseOpParameter (op) ;
		switch (op){			
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
	}
	return val;

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
		val2 = ParseParameter (operator) ;	
	
	} else {
		
		ParseLParen () ;
		val = ParseExpression () ;
		ParseRParen () ;
		val2 = ParseParameter (operator) ;
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

	return val;

}

int ParseAsignation()
{
	int val ;
	ParseEqual () ;
	int index = SearchIndex () ;
	val = ParseType ();
	array[index] = val;
	return val;
}

int ParseExpression () 		
{				
	int val ;
	int operator ;
	if(token == T_OPERATOR && token_val == '!'){
		val = ParseAsignation();
		return val;
	}
	else{
		operator = ParseOperator () ;
		val=ParseOpExpression () ;
		return val;
	}
}


int ParseAxiom () 		
{
	int val ;
	int index;

	if(token == T_OPERATOR && token_val == '!'){
		val = ParseAsignation();
		return val;

	} else {

		val = ParseType ();
		return val ;

	}
}


int main (int argc, char *argv[])
{
	array=(int*)malloc(52 * sizeof(int)); /*52 porque tenemos 52 posibles variables*/
	while(1) {
		rd_lex();

		if(token == -1) {
			break;
		}

		printf("Valor %d\n", ParseAxiom());
	}

	// system ("PAUSE") ;
	free(array);
}