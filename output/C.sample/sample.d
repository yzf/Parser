#domain i(X).
#domain i(Y).
#domain i(NV_0).
#domain i(NV_1).
#domain i(PN_0).
#domain i(PN_1).

#domain min(MIN).
#domain max(MAX).

min(1).
i(1..2).
max(2).

_p(X) :- not p(X).
_pr(X) :- not pr(X).
_s(X,Y) :- not s(X,Y).
_t(X,Y) :- not t(X,Y).
_succ(X,Y,NV_0,NV_1) :- not succ(X,Y,NV_0,NV_1).
