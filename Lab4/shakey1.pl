% Move Shakey from room3 to room1

% actions
act(go(X,Y),
  [at(shakey,X), connected(X,Y)],
  [at(shakey, X)],
  [at(shakey,Y)]
).

act(push(B, X, Y),
  [at(shakey, X), on(shakey, floor), at(B, X), at(S,X),  lightOn(S), box(B), connected(X, Y)],
  [at(B, X), at(shakey, X)],
  [at(B, Y), at(shakey, Y)]
).

act(turnOn(S),    % turn on light using switch S
  [at(S,R), at(shakey,R), at(B,R), on(shakey, B), lightOff(S), box(B)],
  [lightOff(S)],
  [lightOn(S)]
).

act(turnOff(S),
  [at(S,R), at(shakey,R), at(B,R), on(shakey, B), lightOn(S), box(B)],
  [lightOn(S)],
  [lightOff(S)]
).
  /*
act(turnOn(S),    % turn on light using switch S
  [at(S,R), at(shakey,S), at(B,S), on(shakey, B), room(R), lightOff(S), box(B)],
  [lightOff(S)],
  [lightOn(S)]
).

act(turnOff(S),
  [at(S,R), at(shakey,S), at(B,S), on(shakey, B), room(R), lightOn(S), box(B)],
  [lightOn(S)],
  [lightOff(S)]
).    */

act(climbUp(B),
  [at(shakey, R), at(B,R), on(shakey,floor), floor(floor), box(B)],
  [on(shakey,floor)],
  [on(shakey,B)]
).

act(climbDown(B),
  [at(shakey,R), at(B,R), on(shakey,B), floor(floor), box(B)],
  [on(shakey,B)],
  [on(shakey,floor)]
).

goal_state( [/*at(shakey,r1) ,*/ lightOff(s1) /*, at(b2,r2)*/] ).

initial_state(
[
  room(r1),
  room(r2),
  room(r3),
  room(r4),
  floor(floor),

  at(shakey,r3),
  on(shakey, floor),

  lightOn(s1) ,
  lightOff(s2),
  lightOff(s3),
  lightOn(s4),

  switch(s1),
  switch(s2),
  switch(s3),
  switch(s4),

  at(s1,r1),
  at(s2,r2),
  at(s3,r3),
  at(s4,r4),

  box(b1),
  box(b2),
  box(b3),
  box(b4),

  at(b1,r1),
  at(b2,r1),
  at(b3,r1),
  at(b4,r1),

  connected(r1,r2),
  connected(r1,r3),
  connected(r1,r4),
  connected(r2,r3),
  connected(r2,r4),
  connected(r3,r4),

  connected(r2,r1),
  connected(r3,r1),
  connected(r4,r1),
  connected(r3,r2),
  connected(r4,r2),
  connected(r4,r3),

  connected(r1,r1) ,
  connected(r2,r2)  ,
  connected(r3,r3)   ,
  connected(r4,r4)  ,
  
  connected(r1,s1),
  connected(r2,s2),
  connected(r3,s3),
  connected(r4,s4)

]).
