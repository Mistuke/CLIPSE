(clear)
(defrule joins =>)
(join-activity joins)
(clear)
(defrule joins (a) =>)
(join-activity joins)
(assert (a))
(join-activity joins)
(retract 1)
(join-activity joins)
(clear)

(defrule joins 
   (a ?x) 
   (b ?x) 
   (c ?x) 
   =>)
(join-activity joins)
(assert (a 1) (b 1) (c 1))
(join-activity joins)
(assert (c 2) (b 2) (a 2))
(join-activity joins)
(assert (a 3) (b 4) (c 5))
(join-activity joins)
(join-activity joins verbose)
(join-activity joins succinct)
(join-activity joins terse)
(join-activity joins bogus)
(join-activity bogus)
(clear)

(defrule joins 
   (a) 
   (not (b ?)) 
   =>)
(join-activity joins)
(assert (a))
(join-activity joins)
(assert (b 1))
(join-activity joins)
(assert (b 2) (b 3))
(join-activity joins)
(retract 4)
(join-activity joins)
(retract 2)
(join-activity joins)
(retract 3)
(join-activity joins)
(clear)
(defrule joins 
   (a ?x) 
   (not (and (b ?x) 
             (c ?x)))
   =>)
(join-activity joins)
(assert (c 1))
(join-activity joins)
(assert (a 1))
(join-activity joins)
(assert (b 1))
(join-activity joins)
(assert (a 2))
(join-activity joins)
(assert (b 2))
(join-activity joins)
(assert (c 2))
(join-activity joins)
(retract 2)
(join-activity joins)
(retract 5)
(join-activity joins)
(assert (a 3))
(join-activity joins)
(join-activity joins succinct)
(join-activity-reset)
(join-activity joins)
(clear)

(defrule joins 
   (a) 
   (not (and (b) 
             (not (and (c) 
                       (d)))))
   =>)
(assert (a))
(join-activity joins)
(join-activity-reset)
(assert (b))
(join-activity joins)
(join-activity-reset)
(assert (c))
(join-activity joins)
(join-activity-reset)
(assert (d))
(join-activity joins)
(join-activity-reset)
(retract 2)
(join-activity joins)
(join-activity-reset)
(retract 1)
(join-activity joins)
(clear)

(defrule joins
   (a) 
   (b) 
   (c)
   =>)
(matches joins)
(matches joins verbose)
(matches joins succinct)
(matches joins terse)
(matches joins bogus)
(matches bogus)
(assert (c))
(matches joins)
(assert (a))
(matches joins)
(assert (b))
(matches joins)
(clear)
(defrule joins 
   (a) 
   (not (and (b) 
             (c)))
   =>)
(matches joins)
(assert (a))
(matches joins)
(assert (b))
(matches joins)
(assert (c))
(matches joins)
(clear)
   
(defrule joins
   (a)
   (not (and (b)
             (not (and (c)
                       (d)))))
   =>)   
(matches joins)
(assert (a))
(matches joins)
(assert (b))
(matches joins)
(assert (c))
(matches joins)
(assert (d))
(matches joins)
(matches joins succinct)
(matches joins terse)
(clear)
   
(defrule joins
   (a)
   (not (and (b) (c)))
   (d)
   (not (and (e) (f)))
   (g)
   =>)
(matches joins)
(assert (a) (d) (g))
(matches joins)
(assert (b) (f))
(matches joins)
(assert (c) (e))
(matches joins)
(clear)

(defrule joins
  (a)
  (not (and (b) (c)))
  (not (and (d) (e)))
  (f)
  =>)
(assert (a) (f))
(matches joins)
(assert (b) (e))
(matches joins)
(assert (d))
(matches joins)
(assert (c))
(matches joins)
(clear)

(defrule joins
  (not (and (a)
            (b)))
  =>)
(matches joins)
(assert (a))
(matches joins)
(assert (b))
(matches joins)
(clear)
