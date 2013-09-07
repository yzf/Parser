#const size = 10.
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
like(1,1,6).
like(1,2,9).
like(1,3,7).
like(1,4,10).
like(1,5,8).
prefer(1,6,9).
prefer(1,6,7).
prefer(1,6,10).
prefer(1,6,8).
prefer(1,9,7).
prefer(1,9,10).
prefer(1,9,8).
prefer(1,7,10).
prefer(1,7,8).
prefer(1,10,8).
like(2,1,6).
like(2,1,7).
like(2,2,8).
like(2,2,9).
like(2,3,10).
prefer(2,6,8).
prefer(2,6,9).
prefer(2,6,10).
prefer(2,7,8).
prefer(2,7,9).
prefer(2,7,10).
prefer(2,8,10).
prefer(2,9,10).
like(3,1,7).
like(3,2,9).
like(3,2,8).
like(3,3,6).
like(3,4,10).
prefer(3,7,9).
prefer(3,7,8).
prefer(3,7,6).
prefer(3,7,10).
prefer(3,9,6).
prefer(3,9,10).
prefer(3,8,6).
prefer(3,8,10).
prefer(3,6,10).
like(4,1,9).
like(4,2,6).
like(4,3,8).
like(4,4,7).
like(4,5,10).
prefer(4,9,6).
prefer(4,9,8).
prefer(4,9,7).
prefer(4,9,10).
prefer(4,6,8).
prefer(4,6,7).
prefer(4,6,10).
prefer(4,8,7).
prefer(4,8,10).
prefer(4,7,10).
like(5,1,6).
like(5,2,10).
like(5,3,9).
like(5,4,8).
like(5,5,7).
prefer(5,6,10).
prefer(5,6,9).
prefer(5,6,8).
prefer(5,6,7).
prefer(5,10,9).
prefer(5,10,8).
prefer(5,10,7).
prefer(5,9,8).
prefer(5,9,7).
prefer(5,8,7).
like(6,1,3).
like(6,2,1).
like(6,3,2).
like(6,4,5).
like(6,5,4).
prefer(6,3,1).
prefer(6,3,2).
prefer(6,3,5).
prefer(6,3,4).
prefer(6,1,2).
prefer(6,1,5).
prefer(6,1,4).
prefer(6,2,5).
prefer(6,2,4).
prefer(6,5,4).
like(7,1,4).
like(7,2,2).
like(7,3,5).
like(7,3,3).
like(7,4,1).
prefer(7,4,2).
prefer(7,4,5).
prefer(7,4,3).
prefer(7,4,1).
prefer(7,2,5).
prefer(7,2,3).
prefer(7,2,1).
prefer(7,5,1).
prefer(7,3,1).
like(8,1,4).
like(8,1,5).
like(8,2,1).
like(8,2,2).
like(8,2,3).
prefer(8,4,1).
prefer(8,4,2).
prefer(8,4,3).
prefer(8,5,1).
prefer(8,5,2).
prefer(8,5,3).
like(9,1,5).
like(9,2,2).
like(9,2,4).
like(9,2,3).
like(9,3,1).
prefer(9,5,2).
prefer(9,5,4).
prefer(9,5,3).
prefer(9,5,1).
prefer(9,2,1).
prefer(9,4,1).
prefer(9,3,1).
like(10,1,1).
like(10,1,2).
like(10,2,4).
like(10,3,3).
like(10,4,5).
prefer(10,1,4).
prefer(10,1,3).
prefer(10,1,5).
prefer(10,2,4).
prefer(10,2,3).
prefer(10,2,5).
prefer(10,4,3).
prefer(10,4,5).
prefer(10,3,5).
unaccept(4,7).
