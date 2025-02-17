(defvar *nextToken*)
(defvar *syntax*)
;;It finds patterns for our syntax
(defun patternFinder (word)
    (cond ((string= word "if") (setf *nextToken* "KW_IF"))
        ((string= word "and") (setf *nextToken* "KW_AND"))
        ((string= word "or") (setf *nextToken* "KW_OR"))
        ((string= word "not") (setf *nextToken* "KW_NOT"))
        ((string= word "equal") (setf *nextToken* "KW_EQUAL"))
        ((string= word "less") (setf *nextToken* "KW_LESS"))
        ((string= word "nil") (setf *nextToken* "KW_NIL"))
        ((string= word "list") (setf *nextToken* "KW_LIST"))
        ((string= word "append") (setf *nextToken* "KW_APPEND"))
        ((string= word "concat") (setf *nextToken* "KW_CONCAT"))
        ((string= word "set") (setf *nextToken* "KW_SET"))
        ((string= word "deffun") (setf *nextToken* "KW_DEFFUN"))
        ((string= word "for") (setf *nextToken* "KW_FOR"))
        ((string= word "exit") (setf *nextToken* "KW_EXIT"))
        ((string= word "load") (setf *nextToken* "KW_LAOD"))
        ((string= word "disp") (setf *nextToken* "KW_DISP"))
        ((string= word "true") (setf *nextToken* "KW_TRUE"))
        ((string= word "false") (setf *nextToken* "KW_FALSE"))
        ((some #'digit-char-p word) (format t "SYNTAX_ERROR it is not either number or variable~%"))
        (t (setf *nextToken* "IDENTIFIER"))
    )
)

;;It takes back one character
(defun back (char stream)
    (if char (if (char= char (code-char 41)) (unread-char char stream)))
)

;;for those which contains character and digit
(defun letter (stream word)
    (let ((char (read-char stream nil)))
        (if char
            (cond ((or (alpha-char-p char) (digit-char-p char))
                (letter stream (concatenate 'string word (string char)))) ;;call recursively
                (t (patternfinder word));;finished, proceed with patternfinder
            )
            (patternfinder word);;finished, proceed with patternfinder
        )
        (back char stream)
    )
)

;;It determines that it is integer or float
(defun valuefinder (word)
    (cond ((search ":" word) (setf *nextToken* "VALUEF"))
          (t (setf *nextToken* "VALUEI"))
    )
)

;;for those which contains digit
(defun digit (stream word)
    (let ((char (read-char stream nil nil)))
        (if char
            (cond ((or (digit-char-p char) (char= char #\:))
                    (digit stream (concatenate 'string word (string char)))) ;;call recursively
                ((alpha-char-p char) (letter stream word))
                (t (valuefinder word));;finished, proceed with valuefinder
            )
            (valuefinder word);;finished, proceed with valuefinder
        )
        (back char stream)
    )
)

;;for those which contains operator
(defun lookop (character)
    (cond ((char= character (code-char 40)) (setf *nextToken* "OP_OP"))
          ((char= character (code-char 41)) (setf *nextToken* "OP_CP"))
          ((char= character #\-) (setf *nextToken* "OP_MINUS"))
          ((char= character #\+) (setf *nextToken* "OP_PLUS"))
          ((char= character #\/) (setf *nextToken* "OP_DIV"))
          ((char= character #\*) (setf *nextToken* "OP_MULT"))
          ((char= character #\,) (setf *nextToken* "OP_COMMA"))
    )
)

;;for skipping line in stream
(defun skipLine (stream)
    (read-line stream)
)

;;It skips all whitespace in stream
(defun skip-whitespaces (stream)
    (let ((char (peek-char nil stream nil nil)))  ; Peek at the next character
        (when (and char (char= #\Space char))   ; Check if it's whitespace
            (read-char stream)                  ; Consume the whitespace character
            (skip-whitespaces stream)
        )
    )
)

;;It initializes recursive functions
(defun lexer (stream)
    (let ((char (read-char stream nil)))
        (when char
            (if (char= char #\Space) (progn (skip-whitespaces stream) (lexer stream))) ;;if it is a space char, skip whitespaces
            (cond ((alpha-char-p char) (letter stream (string char)));;if it is alphabetic
                  ((digit-char-p char) (digit stream (string char)));;if it is digit
                  ((char= char #\;) (progn
                        (setf *nextToken* "COMMENT");;it is comment if it includes ";"
                        (skipline stream)
                  ))
                  (t (lookop char))
            )
        )
    )
)

(declaim (ftype (function (stream) ) exprB))
(declaim (ftype (function (stream) ) identifierlist))

(defvar *syntax1*)
(defvar *syntax2*)

;;starting point
(defun input (stream)
    (progn
        (lexer stream)
        (exprI stream)
        (setf *syntax1* *syntax*)
        (exprB stream)
        (setf *syntax2* *syntax*)
    )
    (setf *syntax* (or *syntax1* *syntax2*))
)

(defun expr (stream)
    (progn
        (exprI stream)
        (setf *syntax1* *syntax*)
        (exprB stream)
        (setf *syntax2* *syntax*)
    )
    (setf *syntax* (or *syntax1* *syntax2*))
)

;;It is for integer expression
(defun exprI (stream)
    (cond ((string= *nextToken* "OP_OP")
            (progn
                (lexer stream)
                (cond   ((string= *nextToken* "OP_PLUS")
                            (progn
                                (lexer stream)
                                (exprI stream)
                                (lexer stream) 
                                (exprI stream)
                                (lexer stream)
                            )
                            (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                        )
                        ((string= *nexttoken* "OP_MINUS")
                            (progn
                                (lexer stream)
                                (exprI stream)
                                (lexer stream) 
                                (exprI stream)
                                (lexer stream)
                            )
                            (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        ((string= *nextToken* "OP_MULT")
                            (progn
                                (lexer stream)
                                (exprI stream)
                                (lexer stream)
                                (exprI stream)
                                (lexer stream)
                            )
                            (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        ((string= *nextToken* "OP_DIV")
                            (progn
                                (lexer stream)
                                (exprI stream)
                                (lexer stream)
                                (exprI stream)
                                (lexer stream)
                            )
                            (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        ((string= *nextToken* "OP_POW")
                            (progn
                                (lexer stream)
                                (exprI stream)
                                (lexer stream)
                                (exprI stream)
                                (lexer stream)
                            )
                            (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        ((string= *nextToken* "KW_SET")
                                (if (string= *nexttoken* "IDENTIFIER")
                                    (progn
                                        (lexer stream)
                                        (exprI stream)
                                        (lexer stream)
                                    )
                                    (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                                )
                            
                        )
                        ((string= *nextToken* "KW_DEFFUN")
                            
                                (if (string= *nexttoken* "IDENTIFIER")
                                    (progn
                                        (lexer stream)
                                        (identifierlist stream)
                                        (lexer stream)
                                        (expr stream)
                                        (lexer stream)
                                    )
                                    (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                                
                                )
                            
                        )
                        ((string= *nextToken* "IDENTIFIER")
                            (progn
                                (lexer stream)
                                (exprI stream)
                                (lexer stream)
                                (exprI stream)
                                (lexer stream)
                            )
                            (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        ((string= *nexttoken* "KW_DISP")
                            (progn
                                (lexer stream)
                                (exprI stream)
                                (lexer stream)
                            )
                            (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        ((string= *nexttoken* "KW_IF")
                            (progn
                                (lexer stream)
                                (exprB stream)
                                (lexer stream)
                                (expr stream)
                                (lexer stream)
                            )
                            (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        ((string= *nexttoken* "KW_WHILE")
                            (progn
                                (lexer stream)
                                (exprB stream)
                                (lexer stream)
                                (expr stream)
                                (lexer stream)
                            )
                            (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        ((string= *nexttoken* "KW_FOR")
                            
                                (if (string= *nexttoken* "OP_OP")
                                    
                                        (cond ((string= *nexttoken* "IDENTIFIER")
                                            (progn
                                                    (lexer stream)
                                                    (exprI stream)
                                                    (lexer stream)
                                                    (exprI stream)
                                                    (lexer stream)
                                            ))
                                        )
                                        (cond ((string= *nextToken* "OP_CP")
                                                (progn
                                                    (lexer stream)
                                                    (expr stream)
                                                    (lexer stream)
                                                )
                                                (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                                            )
                                            (t (setf *syntax* "FALSE")) 
                                        )
                                )
                            
                        )
                        ((string= *nextToken* "KW_DEFVAR")
                    
                                (if (string= *nexttoken* "IDENTIFIER")
                                    (progn
                                        (lexer stream)
                                        (exprI stream)
                                        (lexer stream)
                                    )
                                    (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                                    
                                )
                            
                        )
                        ((string= *nexttoken* "KW_EXIT")
                            (if (string= *nextToken* "OP_CP") (format t "Exit <exprI>~%"))
                        )
                        (t (setf *syntax* "FALSE"))
                )
            )
        )
        ((string= *nextToken* "IDENTIFIER")
            
        )
        ((string= *nextToken* "VALUEF")
            
        )
        ((string= *nextToken* "VALUEI")
            
        )
        (t (setf *syntax* "FALSE"))
    )
)

;;It is for binary expression
(defun exprB (stream)
    (cond ((string= *nextToken* "OP_OP")
            (progn
                (lexer stream)
                (cond   ((string= *nextToken* "KW_AND")
                            (progn
                                (lexer stream)
                                (exprB stream)
                                (lexer stream) 
                                (exprB stream)
                                (lexer stream)
                            )
                            (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        ((string= *nexttoken* "KW_OR")
                            (progn
                                (lexer stream)
                                (exprB stream)
                                (lexer stream) 
                                (exprB stream)
                                (lexer stream)
                            )
                                (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        ((string= *nextToken* "KW_NOT")
                            (progn
                                (lexer stream)
                                (exprB stream)
                                (lexer stream)
                            )
                                (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        ((string= *nextToken* "KW_LESS")
                            (progn
                                (lexer stream)
                                (exprI stream)
                                (lexer stream)
                                (exprI stream)
                                (lexer stream)
                            )
                                (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        ((string= *nextToken* "KW_EQUAL")
                            (progn
                                (lexer stream)
                                (exprB stream)
                                (lexer stream)
                                (exprB stream)
                                (lexer stream)
                            )
                                (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        ((string= *nextToken* "KW_EQUAL")
                            (progn
                                (lexer stream)
                                (exprI stream)
                                (lexer stream)
                                (exprI stream)
                                (lexer stream)
                            )
                                (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
                            
                        )
                        (t (setf *syntax* "FALSE"))
                )
            )
        )
        ((string= *nextToken* "KW_TRUE")
            ;;do nothing
        )
        ((string= *nextToken* "KW_FALSE")
            ;;do nothing
        )
        (t (setf *syntax* "FALSE"))
    )
)

(defun idlist (stream)
    (cond ((string= *nextToken* "IDENTIFIER")
            
        )
        ((string= *nextToken* "IDENTIFIER")
            (progn
                (lexer stream)
                (idlist stream)
            )
        )
        (t (setf *syntax* "FALSE"))
    )
)

(defun identifierlist (stream)
    (cond ((string= *nextToken* "OP_OP")
            (progn
                (lexer stream)
                (idlist stream)
                (lexer stream)
            )
                (if (not (string= *nextToken* "OP_CP")) (setf *syntax* "FALSE"))
        )
        (t (setf *syntax* "FALSE"))
    )
)
      
;;main function that starts interpretion
(defun gppinterpreter (&optional line)
    (cond ((search ".lisp" line) (with-open-file (stream line);;file
                (input stream)
            ))
            (t (with-input-from-string (stream line);;line
            (lexer stream)
        ))
    )
)

;;for testing
(defun test ()
    (setf *syntax* "TRUE")
    (gppinterpreter "Homework3\\lisp\\text.lisp") ;file (If errors occurs, the reason why is that file path)
    (format t "SYNTAX: ~a" *syntax*)
)
(test)

