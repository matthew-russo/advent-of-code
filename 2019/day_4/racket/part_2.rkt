#lang racket/base

(require racket/list)
(require racket/stream)

(define (lower-bound) 178416)
(define (upper-bound) 676461)

(define (is-password? number)
  (and (two-but-no-more-adjacent-digits-same? number)
       (digits-never-decrease? number)))

(define (chunk-like-digits number)
  (let ([char-digit-list (string->list (number->string number))])
    (foldl (lambda (char-digit acc)
             (if (equal? (length acc) 0)
                 (list (list char-digit))
                 (let ([last-char-digit (car (car acc))])
                   (if (equal? char-digit last-char-digit)
                       (append
                         (list (append (car acc) (list char-digit)))
                         (cdr acc))
                       (append (list (list char-digit)) acc)))))
           '()
           char-digit-list)))

(define (two-but-no-more-adjacent-digits-same? number)
  (let ([chunked-same-adjacent-digits (chunk-like-digits number)])
    (ormap (lambda (group) (equal? (length group) 2)) chunked-same-adjacent-digits)))
    
(define (digits-never-decrease? number)
  (let ([char-digit-list (string->list (number->string number))])
    (if (< (length char-digit-list) 2)
      #t
      (let ([first  (first char-digit-list)]
            [second (second char-digit-list)])
        (if (char>? first second)
            #f
            (if (equal? (length char-digit-list) 2)
                #t
                (digits-never-decrease? (string->number (list->string (append (list second) (list-tail char-digit-list 2)))))))))))

(define (solution)
  (let* ([password-range (stream->list (in-range (lower-bound) (upper-bound)))]
         [valid-passwords (filter is-password? password-range)])
    (println valid-passwords)
    (println (length valid-passwords))))

(solution)
