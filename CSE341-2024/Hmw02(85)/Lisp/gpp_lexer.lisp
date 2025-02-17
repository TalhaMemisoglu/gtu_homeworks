;;It finds patterns for our syntax
(defun patternFinder (word)
    (cond ((string= word "if") (format t "KW_IF~%"))
        ((string= word "and") (format t "KW_AND"))
        ((string= word "or") (format t "KW_OR"))
        ((string= word "not") (format t "KW_NOT~%"))
        ((string= word "equal") (format t "KW_EQUAL~%"))
        ((string= word "less") (format t "KW_LESS~%"))
        ((string= word "nil") (format t "KW_NIL~%"))
        ((string= word "list") (format t "KW_LIST~%"))
        ((string= word "append") (format t "KW_APPEND~%"))
        ((string= word "concat") (format t "KW_CONCAT~%"))
        ((string= word "set") (format t "KW_SET~%"))
        ((string= word "deffun") (format t "KW_DEFFUN~%"))
        ((string= word "for") (format t "KW_FOR~%"))
        ((string= word "exit") (format t "KW_EXIT~%"))
        ((string= word "load") (format t "KW_LOAD~%"))
        ((string= word "disp") (format t "KW_DISP~%"))
        ((string= word "true") (format t "KW_TRUE~%"))
        ((string= word "false") (format t "KW_FALSE~%"))
        ((some #'digit-char-p word) (format t "SYNTAX_ERROR it is not either number or variable~%"))
        (t (format t "IDENTIFIER~%"))
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
    (cond ((search ":" word) (format t "VALUEF~%"))
          (t (format t "VALUEI~%"))
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
    (cond ((char= character (code-char 40)) (format t "OP_OP~%"))
          ((char= character (code-char 41)) (format t "OP_CP~%"))
          ((char= character #\-) (format t "OP_MINUS~%"))
          ((char= character #\+) (format t "OP_PLUS~%"))
          ((char= character #\/) (format t "OP_DIV~%"))
          ((char= character #\*) (format t "OP_MULT~%"))
          ((char= character #\,) (format t "OP_COMMA~%"))
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
                        (format t "COMMENT~%") ;;it is comment if it includes ";"
                        (skipline stream)
                        (lexer stream) ;;call recursively
                  ))
                  (t (lookop char))
            )
            (lexer stream)
        )
    )
)

;;main function that starts interpretion
(defun gppinterpreter (&optional line)
    (cond ((search ".lisp" line) (with-open-file (stream line);;file
            (lexer stream)))
            (t (with-input-from-string (stream line);;line
            (lexer stream)
        ))
    )
)

;;for testing
(defun test ()
    (gppinterpreter "Lisp//text.lisp") ;file (If errors occurs, the reason why is that file path)
    (format t "~%")
    (gppinterpreter "12") ;line of code
)
(test)