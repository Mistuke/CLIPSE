(clear)                   ; Memory Leak #1
(progn (release-mem) TRUE)
(mem-used)
(defclass SOURCE (is-a USER))

(deffunction foo()
   (do-for-all-instances ((?x SOURCE)) TRUE
      (bind ?y 0)
      (bogus)))
(clear)                   ; Memory Leak #2
(progn (release-mem) TRUE)
(mem-used)
(defclass SOURCE (is-a USER))

(deffunction foo()
   (do-for-all-instances ((?x SOURCE)) (progn (bind ?y 3) (bogus) TRUE)
      (+ 3 4)))
(clear)                   ; Memory Leak #3
(progn (release-mem) TRUE)
(mem-used)
(deftemplate SOURCE)

(deffunction foo()
   (do-for-all-facts ((?x SOURCE)) TRUE
      (bind ?y 0)
      (bogus)))
(clear)                   ; Memory Leak #41
(progn (release-mem) TRUE)
(mem-used)
(deftemplate SOURCE)

(deffunction foo()
   (do-for-all-facts ((?x SOURCE)) (progn (bind ?y 3) (bogus) TRUE)
      (+ 3 4)))
(clear)                   ; Memory Leak #5
(progn (release-mem) TRUE)
(mem-used)

(defclass FOO (is-a USER)
   (slot value1))

(deffunction foo ()
   (make-instance of FOO
      (value1 (bogus))))
(clear)                   ; Memory Leak #6
(progn (release-mem) TRUE)
(mem-used)

(deftemplate FOO
   (slot value1 (type SYMBOL)))

(defrule foo
   (FOO (value1 ?x))
   =>
   (+ ?x 1)
   (printout t ?x))
(clear)
(progn (release-mem) TRUE)
(mem-used)
(clear)

(deftemplate nar 
   (slot bc))

(defrule migrant 
   (test (eq 1 1))
   (nar (bc ?bc))
   =>
   (printout t ?bc crlf))

(deffacts stuff
   (nar  (bc "US")))
(reset)
(run)
(clear)                   ; SourceForge Bug #12
(defclass Test (is-a USER) (multislot Contents))
(make-instance of Test (Contents a b c d e f g h))

(defrule BrokenPatternMatchBehavior-Object
   (object (is-a Test) 
           (Contents $?first ?second ?third $?fourth ?fifth))
   =>
   (printout t ?first " " ?second " " ?third " " ?fourth " " ?fifth crlf))
(run)
(clear) ;; CLIPSESG Bug

(defclass A (is-a USER)
  (role concrete)
  (slot foo)
  (slot bar))
(make-instance a of A)
(watch all)
(modify-instance a (foo 0))
(unwatch all)
(clear) ;; CLIPSESG Bug

(defclass A
  (is-a USER)
  (role concrete)
  (slot one (type STRING))
  (slot two (type SYMBOL) (allowed-values TRUE FALSE) (default TRUE)))

(definstances TEST (a1 of A) (a2 of A) (a3 of A))

(defrule rule1
  ?obj <- (object (is-a A) (name [a1]))
  =>
  (message-modify-instance ?obj (one "a") (two FALSE))
  (send ?obj print))

(defrule rule2
  ?obj <- (object (is-a A) (name [a2]))
  =>
  (message-modify-instance ?obj (two FALSE) (one "a"))
  (send ?obj print))

(defrule rule3
  ?obj <- (object (is-a A) (name [a3]))
  =>
  (modify-instance ?obj (two FALSE) (one "a"))
  (send ?obj print))
(reset)
(run)
(clear) ;; CLIPSESG Crash Bug

(defrule bug
   (dummy)
   (foo ?x)
   (not (and (huge ?x)
             (not (and (test (eq ?x 1))
                       (bar ?x)))))
   =>)
(reset)
(assert (bar 1))
(assert (huge 1))
(clear) ; SourceForge Bug

(defclass FOO
   (is-a USER)
   (slot _foo (default ?NONE)))
(make-instance foo1 of FOO)
(make-instance foo2 of FOO (_foo))
(clear) ; SourceForge Assert/Clear Bug
(defrule foo (a ?x&:(progn (clear) TRUE)) =>)
(assert (a 1))
(send [initial-object] delete)
(assert (a 2))
(clear)
(assert (a (clear)))
(clear)
(deffacts FOO (foo bar) (foo (clear)))
(reset)
(clear)
(assert-string "(a (clear))")
(clear)
(deftemplate foo (multislot x))
(assert (foo (x (clear) 1)))
(facts)
(modify 1 (x (clear) 2))
(facts)
(clear) ; SourceForge Bug

(defmodule FOO
   (export ?ALL))
   
(defclass FOO::DUMMY 
   (is-a USER)
   (slot foo))
   
(defmodule BAR
   (import FOO ?ALL))

(defclass BAR::BAR 
   (is-a USER)
   (slot bar (allowed-classes DUMMY)))
(set-dynamic-constraint-checking TRUE)

(make-instance b of BAR
   (bar (make-instance f of DUMMY)))
(set-dynamic-constraint-checking FALSE)   
(clear)
