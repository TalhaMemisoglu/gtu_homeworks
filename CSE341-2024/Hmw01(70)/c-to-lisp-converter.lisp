;Determine the type of code line based on keywords or patterns.
;Takes a single argument, `line`, which is a string representing a line of code,
;and returns a symbol that indicates the type of code line.
(defun line-type (line)
  (cond ((search "if" line) 'if)
        ((search "for" line) 'for)
        ((search "}" line) 'paranthesis)
        ((search "main" line) 'main-f)
        ((search "printf" line) 'print)
        ((and (search "int" line) (search "(" line) (search ")" line) (search "=" line)) 'assign-func)
        ((and (search "(" line) (search ")" line) (search ";" line)) 'func-def)
        ((and (search "(" line) (search ")" line)) 'func)
        ((and (search "return" line) (search "0" line)) 'return-0)
        ((search "return" line) 'func-return)
        ((and (search "=" line) ( not (search "int" line))) 'assign)
        ((and (search "int" line) (search "=" line)) 'assign-top)
        (t 'unknown))
)

;Determine the appropriate conversion function based on the type of line.
;Takes `line-type` as input, which is a symbol representing the line type, 
;and returns a function symbol for conversion, or a no-op function if unknown.
(defun conversion-foo (line-type)
    (case line-type
        ('if #'convert-if)
        ('for #'convert-for)
        ('paranthesis #'convert-parant)
        ('func-def #'convert-func-def)
        ('func #'convert-func)
        ('return-0 #'convert-return0)
        ('func-return #'convert-funcreturn)
        ('main-f #'convert-main)
        ('assign #'convert-assign)
        ('assign-top #'convert-assigntop)
        ('assign-func #'convert-assign-func)
        ('print #'convert-print)
        ('unknown #'do-nothing)
    )
)

(defun do-nothing (line)
    (format nil "")
)

;Extract the variable name on the left side of an operator (>, <, or =).
;Takes `input-string`, which is a line of code, and returns the substring 
;representing the variable name to the left of the specified operator.
(defun extract-left-variable (input-string)
    (cond ((search ">" input-string) 
        (let ((start (position (code-char 40) input-string))
            (end (position #\> input-string)))
        (subseq input-string (+ start 1) (- end 1))
        ))
        ((search "<" input-string) 
            (let ((start (position (code-char 40) input-string))
                (end (position #\< input-string)))
            (subseq input-string (+ start 1) (- end 1))
            ))
        ((search "=" input-string) 
            (let ((start (position (code-char 40) input-string))
                (end (position #\= input-string)))
            (subseq input-string (+ start 1) (- end 1))
            ))
    )
)

;Extract the variable name on the right side of an operator (>, <, or =).
;Takes `input-string`, which is a line of code, and returns the substring 
;representing the variable name to the right of the specified operator."
(defun extract-right-variable (input-string)
    (cond ((search ">" input-string) 
        (let ((start (position #\> input-string))
            (end (position (code-char 41) input-string)))
        (subseq input-string (+ start 2) end)
        ))
        ((search "<" input-string) 
            (let ((start (position #\< input-string))
                (end (position (code-char 41) input-string)))
            (subseq input-string (+ start 2) end)
            ))
        ((search "=" input-string) 
            (let ((start (position #\= input-string))
                (end (position (code-char 41) input-string)))
            (subseq input-string (+ start 2) end)
            ))
    )
)

;Extracts the operator (>, <, or =) from `input-string`.
;Returns the operator as a string, or nil if no operator is found.
(defun extract-operator (input-string)
  (cond ((search ">" input-string) ">")
        ((search "<" input-string) "<")
        ((search "=" input-string) "="))
)

;Converts a conditional `line` to Lisp-style syntax using `if`.
;Constructs an if expression with the operator, left variable, and right variable.
(defun convert-if (line)
    (format nil "(if (~a ~a ~a)~%(progn" (extract-operator line) (extract-left-variable line) (extract-right-variable line))
)

;Extracts the loop index variable in a for-loop expression.
;Assumes that the index variable is located between the first ';' and the '<' in the `input-string`.
(defun extract-index-variable (input-string)
    (let ((start (position #\; input-string))
                (end (position #\< input-string)))
            (subseq input-string (+ start 1) (- end 1))
            )
)

;Extracts the start position of the loop index in a for-loop expression.
;Assumes the start position is located between '=' and the first ';' in `input-string`.
(defun extract-start-position (input-string)
    (let ((start (position #\= input-string))
                (end (position #\; input-string)))
            (subseq input-string (+ start 1) end)
            )
)

;Extracts the end position of the loop index in a for-loop expression.
;Assumes the end position is located between the '<' operator and the second ';' in `input-string`.
(defun extract-end-position (input-string)
    (let* ((start (position #\< input-string))
                (first-semicolon (position #\; input-string))
                (end (position #\; input-string :start (1+ first-semicolon))))
            (subseq input-string (+ start 1) end)
    )
)

;Converts a for-loop expression `line` to Lisp-style syntax using `loop`.
;Constructs a loop expression with the index variable, start, and end positions.
(defun convert-for (line)
    (format nil "(loop for~a from~a below~a do" (extract-index-variable line) (extract-start-position line) (extract-end-position line))
)


;Extracts the function name from a function definition line.
;Assumes the function name appears between the first space and the '(' character.
(defun extract-function-name (input-string)
  (let ((start (position #\Space input-string :start 0))
        (end (position (code-char 40) input-string )))
    (subseq input-string (+ 1 start) end)
  )
)

;Converts a function definition `line` to Lisp-style syntax using `declaim`.
;Declares a function type with two integer arguments and an integer return type.
(defun convert-func-def (line)
  (let ((function-name (extract-function-name line)))
    (format nil "(declaim (ftype (function (integer integer) integer) ~a))" function-name))
)

;Extracts the first parameter variable name in a function definition.
;Assumes it appears between the second space and the first comma in `input-string`.
(defun extract-first-var (input-string)
    (let* ((first-space (position #\Space input-string))
                (start (position #\Space input-string :start (+ 1 first-space)))
                (end (position #\, input-string)))
            (subseq input-string (+ start 1) end)
    )
)

;Extracts the second parameter variable name in a function definition.
;Assumes it appears between the last comma and the ')' character in `input-string`.
(defun extract-second-var (input-string)
    (let ((end (position (code-char 41) input-string )))
        (subseq input-string (- end 1) end)
    )
)

;Converts a function definition `line` to Lisp-style syntax using `defun`.
;Assumes the function has two parameters.
(defun convert-func (line)
    (format nil "(defun ~a(~a, ~a)" (extract-function-name line) (extract-first-var line) (extract-second-var line))
)

(defun convert-return0 (line)
    (format nil "0") ;; Simply returns "0" as the converted expression
)

;Extracts the first variable in an arithmetic operation.
;Identifies the variable based on the operator position (+, -, *, or /).
(defun take-first-var (input-string)
    (cond ((search "+" input-string) 
            (let ((end (position #\+ input-string)))
            (subseq input-string (- end 2) end)
        ))
        ((search "-" input-string) 
            (let ((end (position #\- input-string)))
            (subseq input-string (- end 2) end)
        ))
        ((search "*" input-string) 
            (let ((end (position #\* input-string)))
            (subseq input-string (- end 2) end)
        ))
        ((search "/" input-string) 
            (let ((end (position (code-char 47) input-string)))
            (subseq input-string (- end 2) end)
        ))
    )
)

;Extracts the second variable in an arithmetic operation.
;Assumes the second variable is located after the operator up to the semicolon.
(defun take-second-var (input-string)
    (cond ((search "+" input-string) 
            (let ((start (position #\+ input-string))
                (end (position #\; input-string)))
            (subseq input-string (+ start 2) end)
            ))
        ((search "-" input-string) 
            (let ((start (position #\- input-string))
                (end (position #\; input-string)))
            (subseq input-string (+ start 2) end)
            ))
        ((search "*" input-string) 
            (let ((start (position #\* input-string))
                (end (position #\; input-string)))
            (subseq input-string (+ start 2) end)
            ))
        ((search "/" input-string) 
            (let ((start (position (code-char 47) input-string))
                (end (position #\; input-string)))
            (subseq input-string (+ start 2) end)
            ))
    )
)

;Extracts the arithmetic operator (+, -, *, or /) from the `input-string`.
(defun take-operator (input-string)
  (cond ((search "+" input-string) "+")
        ((search "-" input-string) "-")
        ((search "*" input-string) "*")
        ((search "/" input-string) "*"))
)

;Converts a return statement with an arithmetic operation to Lisp-style syntax.
;Assumes the line includes an operation with two variables.
(defun convert-funcreturn (line)
    (format nil " (~a ~a ~a) " (take-operator line) (take-first-var line) (take-second-var line))
)

;Extracts the left-side variable in an assignment expression.
;Assumes the variable is located just before the '=' character in `input-string`.
(defun extract-left (input-string)
    (let ((end (position #\= input-string )))
        (subseq input-string (- end 2) end)
    )
)

;Extracts the right-side expression in an assignment expression.
;Assumes the expression is located after the '=' character up to the ';' character.
(defun extract-right (input-string)
    (let ((start (position #\= input-string :start 0))
            (end (position #\; input-string )))
        (subseq input-string (+ 1 start) end)
    )
)

;Converts a standard assignment expression to Lisp-style syntax using `setf`.
(defun convert-assign (line)
    (format nil "(setf ~a ~a)))" (extract-left line) (extract-right line))
)

;Extracts the variable name in a top-level assignment expression.
;Assumes the variable name starts after 'int' keyword and ends before '='.
(defun left (input-string)
    (let ((start (position #\t input-string :start 0))
            (end (position #\= input-string )))
        (subseq input-string (+ 2 start) end)
    )
)

;Extracts the value in a top-level assignment expression.
;Assumes the value is located after '=' up to the ';' character.
(defun right (input-string)
    (let ((start (position #\= input-string :start 0))
            (end (position #\; input-string )))
        (subseq input-string (+ 2 start) end)
    )
)

;Converts a top-level assignment to Lisp-style syntax.
(defun convert-assigntop (line)
    (format nil "(~a~a)" (left line) (right line))
)

;Extracts the function name from a function assignment line.
;Assumes function name is located between '=' and '('.
(defun func-name (input-string)
    (let ((start (position #\= input-string))
            (end (position (code-char 40) input-string )))
        (subseq input-string (+ 2 start) end)
    )
)

;Extracts the first argument in a function call.
;Assumes the argument is located between '(' and ',' characters.
(defun left-var (input-string)
    (let ((start (position (code-char 40) input-string))
            (end (position #\, input-string )))
        (subseq input-string (+ 1 start) end)
    )
)

;Extracts the second argument in a function call.
;Assumes the argument is located between ',' and ')' characters.
(defun right-var (input-string)
    (let ((start (position #\, input-string))
            (end (position (code-char 41) input-string )))
        (subseq input-string (+ 2 start) end)
    )
)

;Converts a function assignment to Lisp-style syntax.
(defun convert-assign-func (line)
    (format nil "(~a (~a ~a ~a)))" (left line) (func-name line) (left-var line) (right-var line))
)

;Converts the main function definition to Lisp-style syntax.
(defun convert-main (line)
    (format nil "(defun main ()~%(let (")
)

;Returns the closing parenthesis in Lisp syntax.
(defun convert-parant (line)
    (format nil ")")
)

;Extracts text within quotation marks for printing.
;Assumes text is located between double quotes and before any escape character.
(defun extract-text (input-string)
    (let ((start (position (code-char 34) input-string))
            (end (position (code-char 92) input-string)))
            (subseq input-string (+ 1 start) end)
    )
)

;Extracts the variable to be printed in a `printf` statement.
;Assumes the variable is located between ',' and ')' characters.
(defun extract-var-print (input-string)
    (let ((start (position #\, input-string))
            (end (position (code-char 41) input-string)))
            (subseq input-string (+ 1 start) end)
    )
)

;Converts a `printf` statement to Lisp-style syntax using `format`.
;Checks if the line includes a `d` (indicating integer printing), otherwise assumes a string.
(defun convert-print (line)
    (cond ((search "d" line) 
            (format nil "(format t \"~~d~~%\"~a)" (extract-var-print line))    
          )
        (t 
            (format nil "(format t \"~a~~%\")" (extract-text line))
        )
    )
)

;Reads a file line by line, returning a list of lines.
;Opens `filename` for input, reads each line, and collects them into a list.
(defun read-file (filename)
    (with-open-file (stream filename :direction :input :if-does-not-exist :error)
        (loop for line = (read-line stream nil nil)
            while line
            collect line)
    )
)

;Appends a single line to a file.
;Opens `filename` for output, creating the file if it doesn't exist, and appends `line`.
(defun write-file (filename line)
    (with-open-file (out filename :direction :output :if-exists :append :if-does-not-exist :create)
        (format out "~a~%" line)
    )
)

;Applies a given `conversion-function` to a line.
;Calls `conversion-function` with `line` as an argument to get the converted line.
(defun convert (conversion-function line)
    (funcall conversion-function line)
)

;Reads lines from `input.c`, converts each line, and writes the results to `output.lisp`.
(defun main ()
    (let ((lines (read-file "Homework1\\input.c")));That function and usage of that is only codes which violates functional programming principles.
        (dolist (line lines)
            (write-file "Homework1\\output.lisp" (convert (conversion-foo (line-type line)) line))
        )
    )
)

(main)