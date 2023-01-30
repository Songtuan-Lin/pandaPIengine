;; #state features
25
init
matched[1]
matched[2]
matched[3]
matched[4]
counted[0]
counted[1]
counted[0]
counted[1]
counted[2]
empty[0;0;0]
occupied[0;0;0]
empty[0;1;2]
occupied[0;1;2]
counted[0]
counted[1]
counted[2]
empty[1;0;0]
occupied[1;0;0]
counted[0]
counted[1]
counted[2]
counted[3]
empty[2;0;0]
occupied[2;0;0]

;; Mutex Groups
25
0 0 init
1 1 matched[1]
2 2 matched[2]
3 3 matched[3]
4 4 matched[4]
5 5 counted[0]
6 6 counted[1]
7 7 counted[0]
8 8 counted[1]
9 9 counted[2]
10 10 empty[0;0;0]
11 11 occupied[0;0;0]
12 12 empty[0;1;2]
13 13 occupied[0;1;2]
14 14 counted[0]
15 15 counted[1]
16 16 counted[2]
17 17 empty[1;0;0]
18 18 occupied[1;0;0]
19 19 counted[0]
20 20 counted[1]
21 21 counted[2]
22 22 counted[3]
23 23 empty[2;0;0]
24 24 occupied[2;0;0]

;; further strict Mutex Groups
0

;; further non strict Mutex Groups
0

;; known invariants
0

;; Actions
26
0
0 -1
0 1  -1
0 0  -1
0
2 -1
0 3  -1
0 2  -1
0
1 -1
0 2  -1
0 1  -1
0
3 -1
0 4  -1
0 3  -1
0
5 -1
0 6  -1
0 5  -1
0
7 -1
0 8  -1
0 7  -1
0
8 -1
0 9  -1
0 8  -1
0
-1
0 7  -1
-1
0
10 -1
-1
0 11  -1
1
11 0 -1
0 1  -1
0 10  0 0  -1
1
11 2 -1
0 3  -1
0 10  0 2  -1
0
12 -1
-1
0 13  -1
1
13 2 -1
0 3  -1
0 12  0 2  -1
0
14 -1
0 15  -1
0 14  -1
0
15 -1
0 16  -1
0 15  -1
0
-1
0 14  -1
-1
0
17 -1
-1
0 18  -1
1
18 0 -1
0 1  -1
0 17  0 0  -1
1
18 2 -1
0 3  -1
0 17  0 2  -1
0
19 -1
0 20  -1
0 19  -1
0
20 -1
0 21  -1
0 20  -1
0
21 -1
0 22  -1
0 21  -1
0
-1
0 19  -1
-1
0
23 -1
-1
0 24  -1
1
24 0 -1
0 1  -1
0 23  0 0  -1
1
24 2 -1
0 3  -1
0 23  0 2  -1

;; Initial State
0 5 10 11 12 13 17 18 23 24  -1

;; Goal
4  -1

;; Tasks (primitive and abstract)
44
0 matched[0]
0 matched[2]
0 matched[1]
0 matched[3]
0 counting[1]
0 counting[1]
0 counting[2]
0 start[0]
0 empty[0;0;0]
0 match_to_0[0;0;0]
0 match_to_2[0;0;0]
0 empty[0;1;2]
0 match_to_2[0;1;2]
0 counting[1]
0 counting[2]
0 start[1]
0 empty[1;0;0]
0 match_to_0[1;0;0]
0 match_to_2[1;0;0]
0 counting[1]
0 counting[2]
0 counting[3]
0 start[2]
0 empty[2;0;0]
0 match_to_0[2;0;0]
0 match_to_2[2;0;0]
1 counter[0]
1 compound[3]
1 compound[4]
1 compound[5]
1 prim[0]
1 prim[1]
1 prim[2]
1 counter[1]
1 slot[0;0;0]
1 block[0;0]
1 slot[0;1;2]
1 block[0;1]
1 counter[2]
1 slot[1;0;0]
1 block[1;0]
1 counter[3]
1 slot[2;0;0]
1 block[2;0]

;; Initial Abstract Task
27

;; Methods
30
count[0;1]
26
4 -1
-1
matching[0]
30
0 -1
-1
matching[0]
30
1 -1
-1
matching[1]
31
2 -1
-1
matching[2]
32
3 -1
-1
count[0;1]
33
5 -1
-1
count[1;2]
33
6 -1
-1
not_occupy[0;0;0]
34
8 -1
-1
occupy[0;0;0]
34
26 33 9 -1
0 1 1 2 -1
occupy[0;0;0]
34
26 33 10 -1
0 1 1 2 -1
decompose[0;0]
35
34 -1
-1
not_occupy[0;1;2]
36
11 -1
-1
occupy[0;1;2]
36
26 33 12 -1
0 1 1 2 -1
decompose[0;1]
37
36 -1
-1
method[0]
27
7 35 28 37 29 -1
0 1 1 2 2 3 3 4 -1
count[0;1]
38
13 -1
-1
count[1;2]
38
14 -1
-1
not_occupy[1;0;0]
39
16 -1
-1
occupy[1;0;0]
39
26 38 17 -1
0 1 1 2 -1
occupy[1;0;0]
39
26 38 18 -1
0 1 1 2 -1
decompose[1;0]
40
39 -1
-1
method[1]
28
15 40 30 31 -1
0 1 1 2 2 3 -1
count[0;1]
41
19 -1
-1
count[1;2]
41
20 -1
-1
count[2;3]
41
21 -1
-1
not_occupy[2;0;0]
42
23 -1
-1
occupy[2;0;0]
42
26 41 24 -1
0 1 1 2 -1
occupy[2;0;0]
42
26 41 25 -1
0 1 1 2 -1
decompose[2;0]
43
42 -1
-1
method[2]
29
22 43 32 -1
0 1 1 2 -1
