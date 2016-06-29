;;; --------------------------------------------------------------------------
;;; prog.mln

;; Evidence and query predicates and concepts:

(define friends (PredicateNode "friends"))
(define smokes (PredicateNode "smokes"))
(define cancer (PredicateNode "cancer"))

(define Anna (ConceptNode "Anna" (stv 0.1667 1)))
(define Bob (ConceptNode "Bob" (stv 0.1667 1)))
(define Edward (ConceptNode "Edward" (stv 0.1667 1)))
(define Frank (ConceptNode "Frank" (stv 0.1667 1)))
(define Gary (ConceptNode "Gary" (stv 0.1667 1)))
(define Helen (ConceptNode "Helen" (stv 0.1667 1)))

;; Rules

;; If X smokes, then X has cancer.
;; ForAll(x) Smokes(x) -> Cancer(x)
; MLN Rule Weight: 0.5
; Approximate probability: 0.6225

; Version #1
(define smokes-rule-has-cancer
(BindLink
(VariableList
                (TypedVariableLink
                   (VariableNode "$X")
                   (TypeNode "ConceptNode"))
)
(EvaluationLink
        smokes
        (ListLink
            (VariableNode "$X")))

(ImplicationLink
(EvaluationLink
        smokes
        (ListLink
            (VariableNode "$X")))
(EvaluationLink (stv 0.6225 1.0)
        cancer
        (ListLink
            (VariableNode "$X")))
)
))

; Associate a name to the rule
(define smokes-rule-has-cancer-name (DefinedSchemaNode "smokes-rule-has-cancer"))
(DefineLink
  smokes-rule-has-cancer-name
  smokes-rule-has-cancer)

;; In the case that X and Y are friends, if X smokes then so does Y.
;; ForAll(x,y) Friends(x,y) -> (Smokes(x) <-> Smokes(y))
; MLN Rule Weight: 0.4
; Approximate probability: 0.5987

; Version #3
(define smokes-rule-smokes
(BindLink
(VariableList
                (TypedVariableLink
                   (VariableNode "$X")
                   (TypeNode "ConceptNode"))
 (TypedVariableLink
                   (VariableNode "$Y")
                   (TypeNode "ConceptNode"))
)
(AndLink
(EvaluationLink
        friends
        (ListLink
            (VariableNode "$X")
            (VariableNode "$Y")))
 (EvaluationLink
            smokes
            (ListLink
                (VariableNode "$X")))
)

       
        (EvaluationLink (stv 0.5987 1.0)
            smokes
            (ListLink
                (VariableNode "$Y")))
))

; Associate a name to the rule
(define smokes-rule-smokes-name (DefinedSchemaNode "smokes-rule-smokes"))
(DefineLink
  smokes-rule-smokes-name
  smokes-rule-smokes)

; If X and Y are friends, then Y and X are friends.
; Note: this is not currently used.
;(EquivalenceLink (stv 1.0 1.0)
(define smokes-rule-friends
(BindLink
(VariableList
                (TypedVariableLink
                   (VariableNode "$X")
                   (TypeNode "ConceptNode"))
 (TypedVariableLink
                   (VariableNode "$Y")
                   (TypeNode "ConceptNode"))
)
    (EvaluationLink
        friends
        (ListLink
            (VariableNode "$X")
            (VariableNode "$Y")))
    (EvaluationLink (stv 1.0 1.0)
        friends
        (ListLink
            (VariableNode "$Y")
            (VariableNode "$X"))))
)

; Associate a name to the rule
(define smokes-rule-friends-name (DefinedSchemaNode "smokes-rule-friends"))
(DefineLink
  smokes-rule-friends-name
  smokes-rule-friends)


#!
; Version #1
(ImplicationLink (stv 0.6225 1.0)
    (EvaluationLink
        smokes
        (ListLink
            (VariableNode "$X1")))
    (EvaluationLink
        cancer
        (ListLink
            (VariableNode "$X1"))))

;; In the case that X and Y are friends, if X smokes then so does Y.
;; ForAll(x,y) Friends(x,y) -> (Smokes(x) <-> Smokes(y))
; MLN Rule Weight: 0.4
; Approximate probability: 0.5987

; Version #3
(ImplicationLink (stv 0.5987 1.0)
    (EvaluationLink
        friends
        (ListLink
            (VariableNode "$X2")
            (VariableNode "$Y2")))
    (ImplicationLink
        (EvaluationLink
            smokes
            (ListLink
                (VariableNode "$X2")))
        (EvaluationLink
            smokes
            (ListLink
                (VariableNode "$Y2")))))

!#