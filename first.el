                                        ; Lisp
(defun sum(n)
  (let ((s 0))
    (dotimes (i n s)
      (incf s i))))
                                        ; Lisp
(defun addn(n)
  #'(lambda (x)
      (+ x n)))
