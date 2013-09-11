#const size = 8.
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
like(1,1,5).
like(1,1,6).
like(1,2,8).
like(1,2,7).
prefer(1,5,8).
prefer(1,5,7).
prefer(1,6,8).
prefer(1,6,7).
like(2,1,6).
like(2,2,7).
like(2,2,8).
like(2,3,5).
prefer(2,6,7).
prefer(2,6,8).
prefer(2,6,5).
prefer(2,7,5).
prefer(2,8,5).
like(3,1,5).
like(3,2,7).
like(3,3,6).
like(3,4,8).
prefer(3,5,7).
prefer(3,5,6).
prefer(3,5,8).
prefer(3,7,6).
prefer(3,7,8).
prefer(3,6,8).
like(4,1,7).
like(4,1,5).
like(4,2,8).
like(4,3,6).
prefer(4,7,8).
prefer(4,7,6).
prefer(4,5,8).
prefer(4,5,6).
prefer(4,8,6).
like(5,1,4).
like(5,1,2).
like(5,2,1).
like(5,3,3).
prefer(5,4,1).
prefer(5,4,3).
prefer(5,2,1).
prefer(5,2,3).
prefer(5,1,3).
like(6,1,2).
like(6,2,1).
like(6,3,4).
like(6,4,3).
prefer(6,2,1).
prefer(6,2,4).
prefer(6,2,3).
prefer(6,1,4).
prefer(6,1,3).
prefer(6,4,3).
like(7,1,4).
like(7,1,3).
like(7,1,1).
like(7,2,2).
prefer(7,4,2).
prefer(7,3,2).
prefer(7,1,2).
like(8,1,4).
like(8,2,3).
like(8,3,2).
like(8,4,1).
prefer(8,4,3).
prefer(8,4,2).
prefer(8,4,1).
prefer(8,3,2).
prefer(8,3,1).
prefer(8,2,1).
