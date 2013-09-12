#const size = 4.
#const size_1_3 = size / 2 / 3.
#const size_2_3 = size / 3.
#domain person(X).
#domain person(Y).
#domain person(Y_1).
#domain person(Z_1).
#domain person(Z_2).
#domain rank(R).

person(1..size).
rank(1..(size / 2)).

ssex(1..(size / 2),1..(size / 2)).
ssex((size / 2 + 1).. size, (size / 2 + 1).. size).

:- block(X,Y).
pair(X,Y) :- pair(Y,X).
:- ssex(X,Y), pair(X,Y).

:- pair(X,Y), pair(X,Y_1), Y != Y_1.
block(X,Y) | pair(X,Y) :- pair(X,Z_1), pair(Y,Z_2), prefer(X,Y,Z_1), prefer(Y,X,Z_2).
hate_1(X) :- pair(X,Y), like(X,R,Y), R > size_2_3.
hate_2(X) :- pair(X,Y), like(X,R,Y), R > size_1_3, R <= size_2_3.
block(X,Y) :- unaccept(X,Y), pair(X,Y).

pair(1,1) :- r_1_1.
like(1,1,1) :- r_1_1.
r_1_1 :- pair(1,1), like(1,1,1).
pair(1,1) :- r_1_2.
like(1,2,1) :- r_1_2.
r_1_2 :- pair(1,1), like(1,2,1).
pair(1,2) :- r_1_3.
like(1,1,2) :- r_1_3.
r_1_3 :- pair(1,2), like(1,1,2).
pair(1,2) :- r_1_4.
like(1,2,2) :- r_1_4.
r_1_4 :- pair(1,2), like(1,2,2).
pair(1,3) :- r_1_5.
like(1,1,3) :- r_1_5.
r_1_5 :- pair(1,3), like(1,1,3).
pair(1,3) :- r_1_6.
like(1,2,3) :- r_1_6.
r_1_6 :- pair(1,3), like(1,2,3).
pair(1,4) :- r_1_7.
like(1,1,4) :- r_1_7.
r_1_7 :- pair(1,4), like(1,1,4).
pair(1,4) :- r_1_8.
like(1,2,4) :- r_1_8.
r_1_8 :- pair(1,4), like(1,2,4).
r_1_1 | r_1_2 | r_1_3 | r_1_4 | r_1_5 | r_1_6 | r_1_7 | r_1_8.

pair(2,1) :- r_2_1.
like(2,1,1) :- r_2_1.
r_2_1 :- pair(2,1), like(2,1,1).
pair(2,1) :- r_2_2.
like(2,2,1) :- r_2_2.
r_2_2 :- pair(2,1), like(2,2,1).
pair(2,2) :- r_2_3.
like(2,1,2) :- r_2_3.
r_2_3 :- pair(2,2), like(2,1,2).
pair(2,2) :- r_2_4.
like(2,2,2) :- r_2_4.
r_2_4 :- pair(2,2), like(2,2,2).
pair(2,3) :- r_2_5.
like(2,1,3) :- r_2_5.
r_2_5 :- pair(2,3), like(2,1,3).
pair(2,3) :- r_2_6.
like(2,2,3) :- r_2_6.
r_2_6 :- pair(2,3), like(2,2,3).
pair(2,4) :- r_2_7.
like(2,1,4) :- r_2_7.
r_2_7 :- pair(2,4), like(2,1,4).
pair(2,4) :- r_2_8.
like(2,2,4) :- r_2_8.
r_2_8 :- pair(2,4), like(2,2,4).
r_2_1 | r_2_2 | r_2_3 | r_2_4 | r_2_5 | r_2_6 | r_2_7 | r_2_8.

pair(3,1) :- r_3_1.
like(3,1,1) :- r_3_1.
r_3_1 :- pair(3,1), like(3,1,1).
pair(3,1) :- r_3_2.
like(3,2,1) :- r_3_2.
r_3_2 :- pair(3,1), like(3,2,1).
pair(3,2) :- r_3_3.
like(3,1,2) :- r_3_3.
r_3_3 :- pair(3,2), like(3,1,2).
pair(3,2) :- r_3_4.
like(3,2,2) :- r_3_4.
r_3_4 :- pair(3,2), like(3,2,2).
pair(3,3) :- r_3_5.
like(3,1,3) :- r_3_5.
r_3_5 :- pair(3,3), like(3,1,3).
pair(3,3) :- r_3_6.
like(3,2,3) :- r_3_6.
r_3_6 :- pair(3,3), like(3,2,3).
pair(3,4) :- r_3_7.
like(3,1,4) :- r_3_7.
r_3_7 :- pair(3,4), like(3,1,4).
pair(3,4) :- r_3_8.
like(3,2,4) :- r_3_8.
r_3_8 :- pair(3,4), like(3,2,4).
r_3_1 | r_3_2 | r_3_3 | r_3_4 | r_3_5 | r_3_6 | r_3_7 | r_3_8.

pair(4,1) :- r_4_1.
like(4,1,1) :- r_4_1.
r_4_1 :- pair(4,1), like(4,1,1).
pair(4,1) :- r_4_2.
like(4,2,1) :- r_4_2.
r_4_2 :- pair(4,1), like(4,2,1).
pair(4,2) :- r_4_3.
like(4,1,2) :- r_4_3.
r_4_3 :- pair(4,2), like(4,1,2).
pair(4,2) :- r_4_4.
like(4,2,2) :- r_4_4.
r_4_4 :- pair(4,2), like(4,2,2).
pair(4,3) :- r_4_5.
like(4,1,3) :- r_4_5.
r_4_5 :- pair(4,3), like(4,1,3).
pair(4,3) :- r_4_6.
like(4,2,3) :- r_4_6.
r_4_6 :- pair(4,3), like(4,2,3).
pair(4,4) :- r_4_7.
like(4,1,4) :- r_4_7.
r_4_7 :- pair(4,4), like(4,1,4).
pair(4,4) :- r_4_8.
like(4,2,4) :- r_4_8.
r_4_8 :- pair(4,4), like(4,2,4).
r_4_1 | r_4_2 | r_4_3 | r_4_4 | r_4_5 | r_4_6 | r_4_7 | r_4_8.

:- like(X,R,Y), ssex(X,Y).

like(1,1,3).
like(1,1,4).
like(2,1,4).
like(2,1,3).
like(3,1,1).
like(3,2,2).
prefer(3,1,2).
like(4,1,2).
like(4,2,1).
prefer(4,2,1).
