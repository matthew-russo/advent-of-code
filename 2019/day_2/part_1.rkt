#lang racket/base

(require racket/list)
(require racket/match)
(require racket/string)

(struct intcode-interpreter (src curr-pos))

(define (run-interpreter interpreter)
  (match (interpret-opcode (get-current-opcode interpreter))
         ['addition       (run-interpreter (interpret-addition interpreter))]
         ['multiplication (run-interpreter (interpret-multiplication interpreter))]
         ['exit           interpreter]
         ['unknown        (println (string-append "got unknown opcode: " (number->string get-current-opcode interpreter))) interpreter]))

(define (interpret-opcode opcode)
  (match opcode
         [1  'addition]
         [2  'multiplication]
         [99 'exit]
         [_  'unknown]))

(define (interpret-operation interpreter operation)
  (let* ([curr-pos (intcode-interpreter-curr-pos interpreter)]
         [lhs-index (+ curr-pos 1)]
         [rhs-index (+ curr-pos 2)]
         [result-pos-index (+ curr-pos 3)]
         [lhs-pos (get-number-at-index interpreter lhs-index)]
         [rhs-pos (get-number-at-index interpreter rhs-index)]
         [lhs (get-number-at-index interpreter lhs-pos)]
         [rhs (get-number-at-index interpreter rhs-pos)]
         [result-pos (get-number-at-index interpreter result-pos-index)]
         [result (operation lhs rhs)]
         [new-interp (store-number-at-index interpreter result-pos result)])
     (increment-curr-pos new-interp 4)))

(define (interpret-addition interpreter)
  (interpret-operation interpreter +))

(define (interpret-multiplication interpreter)
  (interpret-operation interpreter *))

(define (get-current-opcode interpreter)
  (let ([src (intcode-interpreter-src      interpreter)]
        [pos (intcode-interpreter-curr-pos interpreter)])
    (list-ref src pos)))

(define (get-number-at-index interpreter index)
  (let ([src (intcode-interpreter-src interpreter)])
    (list-ref src index)))

(define (store-number-at-index interpreter index value)
  (let* ([curr-pos (intcode-interpreter-curr-pos interpreter)]
         [src      (intcode-interpreter-src interpreter)]
         [new-src  (list-set src index value)])
    (intcode-interpreter new-src curr-pos)))

(define (increment-curr-pos interpreter amount-to-increment)
  (let* ([src          (intcode-interpreter-src interpreter)]
         [curr-pos     (intcode-interpreter-curr-pos interpreter)]
         [new-curr-pos (+ curr-pos amount-to-increment)])
    (intcode-interpreter src new-curr-pos)))

(define (read-input)
  (let* ([one-big-opcode-string (read-line)]
         [opcode-strings        (string-split one-big-opcode-string ",")])
    (map string->number opcode-strings)))

(define (interpreter->string interpreter)
  (let* ([src         (intcode-interpreter-src interpreter)]
         [src-strings (map number->string src)])
    (string-join src-strings ",")))

(define (replace-numbers interpreter)
  (let* ([with-one-replaced (store-number-at-index interpreter 1 12)]
         [with-two-replaced (store-number-at-index with-one-replaced 2 2)])
    with-two-replaced))

(define (solution)
  (let* ([interpreter (intcode-interpreter (read-input) 0)]
         [modified-interpreter (replace-numbers interpreter)]
         [result       (run-interpreter modified-interpreter)]
         [value-at-zero (get-number-at-index result 0)])
    (println (number->string value-at-zero))))

(solution)
