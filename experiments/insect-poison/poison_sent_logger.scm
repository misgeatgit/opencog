;#!/usr/bin/guile

(use-modules (ice-9 rdelim))
(use-modules (ice-9 popen))
(use-modules (web client))
(use-modules (opencog nlp relex2logic))
(use-modules (opencog nlp chatbot))

(define OC_DIR    "/home/misgana/OPENCOG/opencog/")
(define BASE_DIR  (string-append OC_DIR "experiments/insect-poison/"))
(define POISON_SENT_FILE (string-append BASE_DIR "exp1_poison_sent.txt"))
(define LOG_FILE   (string-append BASE_DIR "exp1_poison.aflog"))

(define fp (open-file POISON_SENT_FILE "r"))
(define fl (open-file LOG_FILE "w"))

(define (get-af-json)  
  (let* ((port (open-input-pipe "curl -H \"Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\"  http://localhost:5000/api/v1.1/atoms?filterby=attentionalfocus&includeIncoming=true&includeOutgoing=true"))
         (text "")
         (str (read-line port)))
    (while (not (eof-object? str))
           (set! text (string-append text str))
           (set! str (read-line port)))              
    (close-pipe port)
  text))

(define (parse-poisonf) 
 (display "NLP parsing started...make sure you've set stimulus(i.e nlp-stimulate <number>\n")
 (let ((sentence (read-line fp)))
       (while (not (eof-object? sentence))              
              (nlp-parse sentence)
              (sleep 2) ;sleep for few seconds to let attentino spreading happen
              (display (strftime "\n[time %H:%M:%S]\n" (localtime (current-time))) fl)
              (display (string-append "SENTENCE: " sentence "\n") fl)
              (display (string-append "AFB: " (object->string (cog-af-boundary)) "\n") fl)
              ;(display (cog-af) fl) 
              (display (get-af-json) fl) 
              (set! sentence (read-line fp))))                   
 (close-port fp)
 (close-port fl)
 (display "DONE LOGGING POISON SENTENCE PARSING.\n")
 )     

