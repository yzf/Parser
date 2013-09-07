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
rank(1..size).

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
like(1,1,4).
like(1,2,3).
prefer(1,4,3).
like(2,1,3).
like(2,2,4).
prefer(2,3,4).
like(3,1,1).
like(3,2,2).
prefer(3,1,2).
like(4,1,1).
like(4,2,2).
prefer(4,1,2).
