
(use-modules (opencog nlp))
(use-modules (opencog nlp chatbot))
(use-modules (opencog nlp relex2logic))

(display "Loading wordnet...\n")
(primitive-load "/home/misgana/Desktop/database/kb/wordnet.scm")
(display "Loading conceptnet...\n")
(primitive-load "/home/misgana/Desktop/database/kb/conceptnet4.scm")
(display "Finished loading.")

(nlp-start-stimulation 70)
(parse-all nlp-parse "/home/misgana/OPENCOG/opencog/experiments/insect-poison/exp1_insects_sent.txt")
