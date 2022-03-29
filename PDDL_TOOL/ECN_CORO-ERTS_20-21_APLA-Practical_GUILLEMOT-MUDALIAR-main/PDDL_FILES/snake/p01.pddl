(define (problem snake-empty-5x5-1-5-10-11170)
(:domain snake)
(:objects
    pos0-0 pos0-1 pos0-2 pos0-3 pos0-4 pos1-0 pos1-1 pos1-2 pos1-3 pos1-4 pos2-0 pos2-1 pos2-2 pos2-3 pos2-4 pos3-0 pos3-1 pos3-2 pos3-3 pos3-4 pos4-0 pos4-1 pos4-2 pos4-3 pos4-4
)
(:init
    (ISADJACENT pos0-0 pos1-0)
    (ISADJACENT pos0-0 pos0-1)
    (ISADJACENT pos0-1 pos1-1)
    (ISADJACENT pos0-1 pos0-2)
    (ISADJACENT pos0-1 pos0-0)
    (ISADJACENT pos0-2 pos1-2)
    (ISADJACENT pos0-2 pos0-3)
    (ISADJACENT pos0-2 pos0-1)
    (ISADJACENT pos0-3 pos1-3)
    (ISADJACENT pos0-3 pos0-4)
    (ISADJACENT pos0-3 pos0-2)
    (ISADJACENT pos0-4 pos1-4)
    (ISADJACENT pos0-4 pos0-3)
    (ISADJACENT pos1-0 pos2-0)
    (ISADJACENT pos1-0 pos1-1)
    (ISADJACENT pos1-0 pos0-0)
    (ISADJACENT pos1-1 pos2-1)
    (ISADJACENT pos1-1 pos1-2)
    (ISADJACENT pos1-1 pos0-1)
    (ISADJACENT pos1-1 pos1-0)
    (ISADJACENT pos1-2 pos2-2)
    (ISADJACENT pos1-2 pos1-3)
    (ISADJACENT pos1-2 pos0-2)
    (ISADJACENT pos1-2 pos1-1)
    (ISADJACENT pos1-3 pos2-3)
    (ISADJACENT pos1-3 pos1-4)
    (ISADJACENT pos1-3 pos0-3)
    (ISADJACENT pos1-3 pos1-2)
    (ISADJACENT pos1-4 pos2-4)
    (ISADJACENT pos1-4 pos0-4)
    (ISADJACENT pos1-4 pos1-3)
    (ISADJACENT pos2-0 pos3-0)
    (ISADJACENT pos2-0 pos2-1)
    (ISADJACENT pos2-0 pos1-0)
    (ISADJACENT pos2-1 pos3-1)
    (ISADJACENT pos2-1 pos2-2)
    (ISADJACENT pos2-1 pos1-1)
    (ISADJACENT pos2-1 pos2-0)
    (ISADJACENT pos2-2 pos3-2)
    (ISADJACENT pos2-2 pos2-3)
    (ISADJACENT pos2-2 pos1-2)
    (ISADJACENT pos2-2 pos2-1)
    (ISADJACENT pos2-3 pos3-3)
    (ISADJACENT pos2-3 pos2-4)
    (ISADJACENT pos2-3 pos1-3)
    (ISADJACENT pos2-3 pos2-2)
    (ISADJACENT pos2-4 pos3-4)
    (ISADJACENT pos2-4 pos1-4)
    (ISADJACENT pos2-4 pos2-3)
    (ISADJACENT pos3-0 pos4-0)
    (ISADJACENT pos3-0 pos3-1)
    (ISADJACENT pos3-0 pos2-0)
    (ISADJACENT pos3-1 pos4-1)
    (ISADJACENT pos3-1 pos3-2)
    (ISADJACENT pos3-1 pos2-1)
    (ISADJACENT pos3-1 pos3-0)
    (ISADJACENT pos3-2 pos4-2)
    (ISADJACENT pos3-2 pos3-3)
    (ISADJACENT pos3-2 pos2-2)
    (ISADJACENT pos3-2 pos3-1)
    (ISADJACENT pos3-3 pos4-3)
    (ISADJACENT pos3-3 pos3-4)
    (ISADJACENT pos3-3 pos2-3)
    (ISADJACENT pos3-3 pos3-2)
    (ISADJACENT pos3-4 pos4-4)
    (ISADJACENT pos3-4 pos2-4)
    (ISADJACENT pos3-4 pos3-3)
    (ISADJACENT pos4-0 pos4-1)
    (ISADJACENT pos4-0 pos3-0)
    (ISADJACENT pos4-1 pos4-2)
    (ISADJACENT pos4-1 pos3-1)
    (ISADJACENT pos4-1 pos4-0)
    (ISADJACENT pos4-2 pos4-3)
    (ISADJACENT pos4-2 pos3-2)
    (ISADJACENT pos4-2 pos4-1)
    (ISADJACENT pos4-3 pos4-4)
    (ISADJACENT pos4-3 pos3-3)
    (ISADJACENT pos4-3 pos4-2)
    (ISADJACENT pos4-4 pos3-4)
    (ISADJACENT pos4-4 pos4-3)
    (tailsnake pos3-0)
    (headsnake pos4-0)
    (nextsnake pos4-0 pos3-0)
    (blocked pos3-0)
    (blocked pos4-0)
    (spawn pos2-0)
    (NEXTSPAWN pos1-0 pos2-0)
    (NEXTSPAWN pos2-0 pos1-4)
    (NEXTSPAWN pos1-4 pos1-1)
    (NEXTSPAWN pos1-1 pos0-1)
    (NEXTSPAWN pos0-1 pos3-3)
    (NEXTSPAWN pos3-3 pos4-2)
    (NEXTSPAWN pos4-2 pos3-4)
    (NEXTSPAWN pos3-4 pos0-0)
    (NEXTSPAWN pos0-0 pos1-2)
    (NEXTSPAWN pos1-2 pos1-0)
    (ispoint pos0-4)
    (ispoint pos3-1)
    (ispoint pos1-3)
    (ispoint pos2-4)
    (ispoint pos4-1)
)
(:goal
(and
    (not (ispoint pos0-4))
    (not (ispoint pos3-1))
    (not (ispoint pos1-3))
    (not (ispoint pos2-4))
    (not (ispoint pos4-1))
    (not (ispoint pos2-0))
    (not (ispoint pos1-4))
    (not (ispoint pos1-1))
    (not (ispoint pos0-1))
    (not (ispoint pos3-3))
    (not (ispoint pos4-2))
    (not (ispoint pos3-4))
    (not (ispoint pos0-0))
    (not (ispoint pos1-2))
    (not (ispoint pos1-0))
)
)
)
