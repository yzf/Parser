#const adder_size = 33.
#const gate_size = 5.
#domain adder(I).
#domain adder(J).
#domain gate(X).

adder(1..adder_size).
gate(1..gate_size).

xor(I,1).
xor(I,2).
and(I,3).
and(I,4).
or(I,5).

% 与门逻辑
out(I,X) | aba(I,X) :- and(I,X), ina(I,X), inb(I,X).
inb(I,X) | aba(I,X) :- and(I,X), ina(I,X), out(I,X).
ina(I,X) | aba(I,X) :- and(I,X), inb(I,X), out(I,X).
ina(I,X) | inb(I,X) | aba(I,X) :- and(I,X), out(I,X).
% 或门逻辑
out(I,X) | abo(I,X) :- or(I,X), ina(I,X), inb(I,X).
out(I,X) | inb(I,X) | abo(I,X) :- or(I,X), ina(I,X).
out(I,X) | ina(I,X) | abo(I,X) :- or(I,X), inb(I,X).
ina(I,X) | inb(I,X) | abo(I,X) :- or(I,X), out(I,X).
% 异或门逻辑
abx(I,X) :- xor(I,X), ina(I,X), inb(I,X), out(I,X).
out(I,X) | abx(I,X) | inb(I,X) :- xor(I,X), ina(I,X).
out(I,X) | abx(I,X) | ina(I,X) :- xor(I,X), inb(I,X).
abx(I,X) | ina(I,X) | inb(I,X) :- xor(I,X), out(I,X).

% 单个全加法器电路结构逻辑
out(I,1) :- ina(I,2).
ina(I,2) :- out(I,1).

out(I,1) :- ina(I,4).
ina(I,4) :- out(I,1).

out(I,4) :- ina(I,5).
ina(I,5) :- out(I,4).

inb(I,4) :- inb(I,2).
inb(I,2) :- inb(I,4).

ina(I,1) :- ina(I,3).
ina(I,3) :- ina(I,1).

inb(I,1) :- inb(I,3).
inb(I,3) :- inb(I,1).

out(I,3) :- inb(I,5).
inb(I,5) :- out(I,3).

% 串行进位加法器逻辑
out(I,5) :- inb(J,4), I==J-1.
inb(J,4) :- out(I,5), I==J-1.

% 加法器
:- inb(1,4).

:- ina(1,1).
inb(1,1).
out(1,2).
out(1,5).
:- ina(2,1).
inb(2,1).
:- out(2,2).
out(2,5).
:- ina(3,1).
:- inb(3,1).
:- out(3,2).
:- out(3,5).
ina(4,1).
:- inb(4,1).
out(4,2).
:- out(4,5).
:- ina(5,1).
inb(5,1).
:- out(5,2).
:- out(5,5).
:- ina(6,1).
:- inb(6,1).
:- out(6,2).
out(6,5).
:- ina(7,1).
inb(7,1).
:- out(7,2).
:- out(7,5).
ina(8,1).
:- inb(8,1).
:- out(8,2).
out(8,5).
:- ina(9,1).
:- inb(9,1).
:- out(9,2).
out(9,5).
ina(10,1).
inb(10,1).
out(10,2).
out(10,5).
:- ina(11,1).
inb(11,1).
:- out(11,2).
out(11,5).
:- ina(12,1).
:- inb(12,1).
out(12,2).
:- out(12,5).
:- ina(13,1).
inb(13,1).
out(13,2).
:- out(13,5).
ina(14,1).
:- inb(14,1).
out(14,2).
:- out(14,5).
:- ina(15,1).
:- inb(15,1).
out(15,2).
:- out(15,5).
ina(16,1).
:- inb(16,1).
:- out(16,2).
:- out(16,5).
:- ina(17,1).
:- inb(17,1).
out(17,2).
out(17,5).
:- ina(18,1).
inb(18,1).
:- out(18,2).
:- out(18,5).
:- ina(19,1).
:- inb(19,1).
out(19,2).
out(19,5).
ina(20,1).
:- inb(20,1).
out(20,2).
out(20,5).
:- ina(21,1).
inb(21,1).
:- out(21,2).
out(21,5).
ina(22,1).
inb(22,1).
out(22,2).
out(22,5).
ina(23,1).
inb(23,1).
:- out(23,2).
:- out(23,5).
ina(24,1).
inb(24,1).
out(24,2).
:- out(24,5).
:- ina(25,1).
:- inb(25,1).
out(25,2).
out(25,5).
ina(26,1).
inb(26,1).
out(26,2).
out(26,5).
ina(27,1).
inb(27,1).
:- out(27,2).
out(27,5).
:- ina(28,1).
:- inb(28,1).
:- out(28,2).
out(28,5).
ina(29,1).
inb(29,1).
out(29,2).
out(29,5).
ina(30,1).
:- inb(30,1).
:- out(30,2).
out(30,5).
:- ina(31,1).
:- inb(31,1).
out(31,2).
out(31,5).
:- ina(32,1).
:- inb(32,1).
:- out(32,2).
:- out(32,5).
:- ina(33,1).
:- inb(33,1).
:- out(33,2).
:- out(33,5).
