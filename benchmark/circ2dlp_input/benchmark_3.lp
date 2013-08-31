#const adder_size = 3.
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
inb(1,4).

ina(1,1).
:- inb(1,1).
out(1,2).
out(1,5).
ina(2,1).
inb(2,1).
out(2,2).
out(2,5).
ina(3,1).
inb(3,1).
out(3,2).
out(3,5).
