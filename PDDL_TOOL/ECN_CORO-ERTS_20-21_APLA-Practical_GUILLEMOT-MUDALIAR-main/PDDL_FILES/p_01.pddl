(define (problem BLOCKS-10-0)
(:domain BLOCKS)

(:objects D A H G B J E I F C )

(:init (clear C) (clear F) (ontable I) (ontable F)
       (on C E) (on E J) (on J B) (on B G) (on G H)
       (on H A) (on A D) (on D I) (handempty))



(:goal(AND (on D C) (on C F) (on F J) (on J E)
            (on E H) (on H B) (on B A) (on A G)
            (on G I))))
