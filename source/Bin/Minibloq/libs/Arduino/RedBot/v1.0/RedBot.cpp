#include "RedBot.h"
#include <Arduino.h>

/**********************************************************

This file exists solely because I don't feel right not having
 a file with the name of the library. Lacking anything else
 constructive to put here, I'll just throw in the text of
 Abbott and Costello's comedy classic, "Who's on first?"
 
 Abbott: Well Costello, I'm going to New York with you. You know Bucky Harris, the Yankee's manager, gave me a job as coach for as long as you're on the team.

Costello: Look Abbott, if you're the coach, you must know all the players.

Abbott: I certainly do.

Costello: Well you know I've never met the guys. So you'll have to tell me their names, and then I'll know who's playing on the team.

Abbott: Oh, I'll tell you their names, but you know it seems to me they give these ball players now-a-days very peculiar names.

Costello: You mean funny names?

Abbott: Strange names, pet names...like Dizzy Dean...

Costello: His brother Daffy.

Abbott: Daffy Dean...

Costello: And their French cousin.

Abbott: French?

Costello: Goofè.

Abbott: Goofè Dean. Well, let's see, we have on the bags, Who's on first, What's on second, I Don't Know is on third...

Costello: That's what I want to find out.

Abbott: I say Who's on first, What's on second, I Don't Know's on third.

Costello: Are you the manager?

Abbott: Yes.

Costello: You gonna be the coach too?

Abbott: Yes.

Costello: And you don't know the fellows' names?

Abbott: Well I should.

Costello: Well then who's on first?

Abbott: Yes.

Costello: I mean the fellow's name.

Abbott: Who.

Costello: The guy on first.

Abbott: Who.

Costello: The first baseman.

Abbott: Who.

Costello: The guy playing...

Abbott: Who is on first!

Costello: I'm asking YOU who's on first.

Abbott: That's the man's name.

Costello: That's who's name?

Abbott: Yes.

Costello: Well go ahead and tell me.

Abbott: That's it.

Costello: That's who?

Abbott: Yes.

PAUSE

Costello: Look, you gotta first baseman?

Abbott: Certainly.

Costello: Who's playing first?

Abbott: That's right.

Costello: When you pay off the first baseman every month, who gets the money?

Abbott: Every dollar of it.

Costello: All I'm trying to find out is the fellow's name on first base.

Abbott: Who.

Costello: The guy that gets...

Abbott: That's it.

Costello: Who gets the money...

Abbott: He does, every dollar. Sometimes his wife comes down and collects it.

Costello: Who's wife?

Abbott: Yes.

PAUSE

Abbott: What's wrong with that?

Costello: Look, all I wanna know is when you sign up the first baseman, how does he sign his name?

Abbott: Who.

Costello: The guy.

Abbott: Who.

Costello: How does he sign...

Abbott: That's how he signs it.

Costello: Who?

Abbott: Yes.

PAUSE

Costello: All I'm trying to find out is what's the guy's name on first base.

Abbott: No. What is on second base.

Costello: I'm not asking you who's on second.

Abbott: Who's on first.

Costello: One base at a time!

Abbott: Well, don't change the players around.

Costello: I'm not changing nobody!

Abbott: Take it easy, buddy.

Costello: I'm only asking you, who's the guy on first base?

Abbott: That's right.

Costello: Ok.

Abbott: All right.

PAUSE

Costello: What's the guy's name on first base?

Abbott: No. What is on second.

Costello: I'm not asking you who's on second.

Abbott: Who's on first.

Costello: I don't know.

Abbott: He's on third, we're not talking about him.

Costello: Now how did I get on third base?

Abbott: Why you mentioned his name.

Costello: If I mentioned the third baseman's name, who did I say is playing third?

Abbott: No. Who's playing first.

Costello: What's on first?

Abbott: What's on second.

Costello: I don't know.

Abbott: He's on third.

Costello: There I go, back on third again!

PAUSE

Costello: Would you just stay on third base and don't go off it.

Abbott: All right, what do you want to know?

Costello: Now who's playing third base?

Abbott: Why do you insist on putting Who on third base?

Costello: What am I putting on third.

Abbott: No. What is on second.

Costello: You don't want who on second?

Abbott: Who is on first.

Costello: I don't know.

Abbott & Costello Together:Third base!

PAUSE

Costello: Look, you gotta outfield?

Abbott: Sure.

Costello: The left fielder's name?

Abbott: Why.

Costello: I just thought I'd ask you.

Abbott: Well, I just thought I'd tell ya.

Costello: Then tell me who's playing left field.

Abbott: Who's playing first.

Costello: I'm not... stay out of the infield! I want to know what's the guy's name in left field?

Abbott: No, What is on second.

Costello: I'm not asking you who's on second.

Abbott: Who's on first!

Costello: I don't know.

Abbott & Costello Together: Third base!

PAUSE

Costello: The left fielder's name?

Abbott: Why.

Costello: Because!

Abbott: Oh, he's centerfield.

PAUSE

Costello: Look, You gotta pitcher on this team?

Abbott: Sure.

Costello: The pitcher's name?

Abbott: Tomorrow.

Costello: You don't want to tell me today?

Abbott: I'm telling you now.

Costello: Then go ahead.

Abbott: Tomorrow!

Costello: What time?

Abbott: What time what?

Costello: What time tomorrow are you gonna tell me who's pitching?

Abbott: Now listen. Who is not pitching.

Costello: I'll break your arm, you say who's on first! I want to know what's the pitcher's name?

Abbott: What's on second.

Costello: I don't know.

Abbott & Costello Together: Third base!

PAUSE

Costello: Gotta a catcher?

Abbott: Certainly.

Costello: The catcher's name?

Abbott: Today.

Costello: Today, and tomorrow's pitching.

Abbott: Now you've got it.

Costello: All we got is a couple of days on the team.

PAUSE

Costello: You know I'm a catcher too.

Abbott: So they tell me.

Costello: I get behind the plate to do some fancy catching, Tomorrow's pitching on my team and a heavy hitter gets up. Now the heavy hitter bunts the ball. When he bunts the ball, me, being a good catcher, I'm gonna throw the guy out at first base. So I pick up the ball and throw it to who?

Abbott: Now that's the first thing you've said right.

Costello: I don't even know what I'm talking about!

PAUSE

Abbott: That's all you have to do.

Costello: Is to throw the ball to first base.

Abbott: Yes!

Costello: Now who's got it?

Abbott: Naturally.

PAUSE

Costello: Look, if I throw the ball to first base, somebody's gotta get it. Now who has it?

Abbott: Naturally.

Costello: Who?

Abbott: Naturally.

Costello: Naturally?

Abbott: Naturally.

Costello: So I pick up the ball and I throw it to Naturally.

Abbott: No you don't, you throw the ball to Who.

Costello: Naturally.

Abbott: That's different.

Costello: That's what I said.

Abbott: You're not saying it...

Costello: I throw the ball to Naturally.

Abbott: You throw it to Who.

Costello: Naturally.

Abbott: That's it.

Costello: That's what I said!

Abbott: You ask me.

Costello: I throw the ball to who?

Abbott: Naturally.

Costello: Now you ask me.

Abbott: You throw the ball to Who?

Costello: Naturally.

Abbott: That's it.

Costello: Same as you! Same as YOU! I throw the ball to who. Whoever it is drops the ball and the guy runs to second. Who picks up the ball and throws it to What. What throws it to I Don't Know. I Don't Know throws it back to Tomorrow, Triple play. Another guy gets up and hits a long fly ball to Because. Why? I don't know! He's on third and I don't give a darn!

Abbott: What?

Costello: I said I don't give a darn!

Abbott: Oh, that's our shortstop.

***********************************************************************/