Blip is a small programming language that supports binary and unary operations. The operations must be written in polish notation. Not only that, but Blip also supports conditional statements and loops. The conditional statements and loops can also have nested statements within them. In order to write a Blip program save some blip code in a text file with the file extension .blip. In the input .cpp file, type in the titile of the text file with the .blip extension.
There are a few keywords for Blip. 
•Variables
	o var <variable_name> <expression> - the var keyword is used to create a new variable and set it to the given expression. If variable was created before, then displays a warning and sets previously declared variable.

•Set
	o set <variable> <expression> - set changes the value of the specified variable, if variable was not 	created before, it displays a warning then creates a variable.

•Output
	o output <expression> - displays the result of unary or binary expression
	o output <variable> - displays the value of specified variable.

•Conditional statements
	o conditional statement starts with if <expression> and ends with fi.
		If the expression is true it evaluates the statement until the keyword fi is read
		if statements can also have the else keyword.
		The else keyword executes if the if statement expression return a false.

•Loops
	o loops start with do <expression> where the next statements are read if the expression results in 		true. The statements execute until the od keyword is read. If the expression is false then 			statements between do <expression> and od are ignored.
