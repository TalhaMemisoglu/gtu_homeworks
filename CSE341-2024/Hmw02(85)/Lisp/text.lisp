;; hello.g++
(deffun sumup (x)
;; function body
 (if (equal x 0)
 12
 (+ x (sumup (- x 1)))
 )
 )