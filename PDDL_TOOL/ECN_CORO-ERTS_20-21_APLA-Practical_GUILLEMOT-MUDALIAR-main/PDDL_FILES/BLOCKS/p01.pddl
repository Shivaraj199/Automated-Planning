(define (problem BLOCKS-10-0)
(:domain BLOCKS)

(:objects D A H G B J E I F C )

(:INIT (CLEAR C) (CLEAR F) (ONTABLE I) (ONTABLE F)
       (ON C E) (ON E J) (ON J B) (ON B G) (ON G H)
       (ON H A) (ON A D) (ON D I) (HANDEMPTY))



(:goal (AND (ON D C) (ON C F) (ON F J) (ON J E)
            (ON E H) (ON H B) (ON B A) (ON A G)
            (ON G I))))
