(defvar *subst* nil) ; Global variable to store substitutions

(defun prolog_prove (axioms query)
    (setq *subst* '()) ; Initialize *subst* as an empty list
    (resolve query axioms)
    (format t "~a~%" *subst*) ; Print the final substitutions
)

(defun resolve (query axioms)
  ;(format t "~a~a~%" query subst)
  (cond ((null query)) ; If there are no more goals, return the current substitutions
        (t 
          (let* ((current-goal (first query))  ; Take the first goal from the query
                 (remaining-goals (rest query)) ; Remaining goals to resolve after the current one
                 (matching-axioms (find-matching-axioms current-goal axioms))) ; Find matching axioms
                 ;(format t "prove:s~as~as~as~%" matching-axioms current-goal remaining-goals)
                (mapcar (lambda (axiom)
                          (unify current-goal (head axiom)) ; Attempt unification
                          ;(format t "after unify:~a head:~a body:~a unified:~a~%" current-goal (head axiom) (body axiom)*subst*)
                          (resolve
                            (append (body axiom) remaining-goals)  ; Add body to remaining goals
                            axioms
                          )
                        ); Continue resolution with updated substitutions
                        matching-axioms
                )
          )
        )
  )
)

;I tried so much to implement the following functions but I couldn't make it work

#|(defun unify-arguments (args1 args2 subst)
  "Unify two lists of arguments under the given substitutions."
  (cond
    ;; Both argument lists are empty, unification succeeded
    ((and (null args1) (null args2)) subst)

    ;; One of the argument lists is empty, unification fails
    ((or (null args1) (null args2)) nil)

    ;; Unify the heads of the argument lists
    (t
     (let ((unified-head (unify (first args1) (first args2) subst)))
       (if unified-head
           ;; Recursively unify the rest of the arguments
           (unify-arguments (rest args1) (rest args2) unified-head)
           ;; If unification of heads fails, the whole process fails
           nil)))))


(defun unify-second (goal axiom subst)
  (let ((goal_term2 (caddr goal))
        (axiom_term2 (caddr axiom)))
        ;(format t "terms1:~a:~a ~a:~a ~%" goal_term2 (variable-p goal_term2) axiom_term2 (variable-p axiom_term2))
    (cond
      ((equal goal_term2 axiom_term2) subst)                        ; Terms are already identical
      ((variable-p goal_term2) (unify-variable goal_term2 axiom_term2 subst)) ; First term is a variable
      ((variable-p axiom_term2) (unify-variable axiom_term2 goal_term2 subst)) ; Second term is a variable
      (t nil)
    )
  )
)


(defun unify (goal axiom subst)
  (format t "inside unify:~a ~a ~a~%" goal axiom subst)
  (let ((goal_term1 (car (cdr goal)))
        (axiom_term1 (car (cdr axiom))))
        ;(format t "terms1:~a:~a ~a:~a ~%" goal_term1 (variable-p goal_term1) axiom_term1 (variable-p axiom_term1))
    (cond
      ((equal goal_term1 axiom_term1) (unify-second goal axiom subst))                        ; Terms are already identical
      ((variable-p goal_term1) (unify-variable goal_term1 axiom_term1 (unify-second goal axiom subst))) ; First term is a variable
      ((variable-p axiom_term1) (unify-variable axiom_term1 goal_term1 (unify-second goal axiom subst))) ; Second term is a variable
      (t nil)
    )
  )
)


(defun give-Val (var subs)
    (if (listp subs)
        (if (equal var (car (car subs)))
            (return-from give-Val (cdr (car subs)))
            (if (cdr subs)
                (give-val var (cdr subs))
                (return-from give-val nil)
            )
        )
  )
)

(defun memberSubvar (var subs)
  (if (listp subs)
        (if (and (equal var (car (car subs))))
            (return-from memberSubvar (cdr (car subs)))
            (if (cdr subs)
                (memberSubst var (cdr subs))
                (return-from memberSubvar nil)
            )
        )
  )
)

(defun substitute-var (body)
  (format t "here:~a~%" body)
  (if (listp body)
    (mapcar (lambda (term)
              (if (variable-p (car (cdr term)))
                (if (memberSubvar (car (cdr term)) *subst*)
                    (substitute (car (cdr term)) (memberSubvar (car (cdr term)) *subst*))
                    term
                )
                term)
              (if (variable-p (caddr term))
                (if (memberSubvar (car (cdr term)) *subst*)
                    (substitute (car (cdr term)) (memberSubvar (car (cdr term)) *subst*))
                    term
                )
                term)
            )
            body)
    nil)
)
|#

(defun unify (goal axiom)
  (if (and (listp goal) (listp axiom))
    (progn
      "Unify two terms under the given substitutions."
      (let ((goal_term1 (car (cdr goal)))
            (axiom_term1 (car (cdr axiom))))
            ;(format t "terms1:~a:~a ~a:~a ~%" goal_term1 (variable-p goal_term1) axiom_term1 (variable-p axiom_term1))
        (cond
          ((equal goal_term1 axiom_term1) ())                        ; Terms are already identical
          ((variable-p goal_term1) (unify-variable goal_term1 axiom_term1)) ; First term is a variable
          ((variable-p axiom_term1) (unify-variable axiom_term1 goal_term1)) ; Second term is a variable
          (t nil)
        )
      )

      (let ((goal_term2 (caddr goal))
            (axiom_term2 (caddr axiom)))
            ;(format t "terms1:~a:~a ~a:~a ~%" goal_term2 (variable-p goal_term2) axiom_term2 (variable-p axiom_term2))
        (cond
          ((equal goal_term2 axiom_term2) ())                        ; Terms are already identical
          ((variable-p goal_term2) (unify-variable goal_term2 axiom_term2)) ; First term is a variable
          ((variable-p axiom_term2) (unify-variable axiom_term2 goal_term2)) ; Second term is a variable
          (t nil)
        )
      )
    )
  )
)                                        ; Unification fails

(defun memberSubst (var value subs)
  (if (listp subs)
        (if (and (equal var (car (car subs))) (equal value (cdr (car subs))))
            (return-from memberSubst t)
            (if (cdr subs)
                (memberSubst var value (cdr subs))
                (return-from memberSubst nil)
            )
        )
  )
)


(defun unify-variable (var value)
  ;(format t "unify-variable:~a ~a~%" (cons var value) *subst*)
  "Unify a variable with a value under the current substitutions."
  (let ((existing (memberSubst var value *subst*)))
    ;(format t "here:~a~%" existing)
    (cond
      (existing ())  ; Variable is already bound
      ((member value *subst* :key #'cdr) nil)  ; Avoid circular references
      (t (setq *subst* (cons (cons var value) *subst*)))
    )
  )
)

(defun variable-p (string)
    ;; Check if a string represents a variable (uppercase letter)
    (let ((ch (char-code (char string 0))))
        (and (>= ch 65) (<= ch 90))
    )
)

(defun find-matching-axioms (goal axioms)
  "Find axioms whose head matches the goal."
  (remove-if-not (lambda (axiom)
                   (predicate-match-p (head axiom) goal))
                 axioms))

(defun predicate-match-p (predicate1 predicate2)
  "Check if two predicates can potentially unify."
  (and (equal (first predicate1) (first predicate2))   ; Compare predicate names
       (= (length (rest predicate1)) (length (rest predicate2))))) ; Same arity

(defun head (axiom)
  "Return the head of the axiom (the first predicate)."
  (first axiom))

(defun body (axiom)
  "Return the body of the axiom (the predicates after the '<')."
  (if (string= "<" (second axiom))
      (cddr axiom) ; Extract predicates after "<"
      nil))        ; No body if axiom is a fact

(let (
  (axioms '(
      ( ("father" "jim" "jill") )
      ( ("mother" "mary" "jill") )
      ( ("father" "samm" "jim") )
      ( ("ancestor" "X" "Y") "<" ("parent" "X" "Y") )
      ;;( ("ancestor" "X" "Y") "<" ("ancestor" "X" "Z") ("ancestor" "Z" "Y") ) it causes infinite loop
      ( ("parent" "X" "Y") "<" ("mother" "X" "Y") )
      ( ("parent" "X" "Y") "<" ("father" "X" "Y") ) ) )
  (query1 '( ("ancestor" "X" "jill") ) )
  (query2 '( ("ancestor" "X" "jill") ("mother" "X" "bob") ) ) ) ;;Query2 is not working properly.
  (prolog_prove axioms query1)
)