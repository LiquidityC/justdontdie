---
title:  "Reset, map making and windows compile"
layout: post
date:   2015-10-26 23:41
categories: maps memory
---
So, last few commits have all been about fixing the reset command. I
recently added an object known as the GameStateController which is
intended to handle what state to render (eg. Splashscreen, loadingbars
or actual gameplay). Once I built it I moved all the current game setup
code from the main method of the program to this new class. It worked
like a charm and [Valgrind][valgrind] reported no leaks.

However a few hours later I noticed that the games memory would increase
slightly every time I triggered one of these reloads. Naturally this
would be because of some data container that loaded, held and disposed
of data in a nice way at exit but wasn't cleared on every reload thus
stacking the same data repeatedly on every reset.

It took some searching to find it. Turned out it was the ResourceContainer
doing what it was created to do but not in an efficient way.

Whilst looking for it I learned of this command to track memory allocation
during runtime of a program.

{% highlight bash %}
valgrind --tool=massif <executable>
{% endhighlight %}

I never got super much use out of it. But it's definitely going in the
pile of good things to know when debugging C/C++ programs.

### Compiling for windows
The next big thing was re-testing the windows compile. I've gotten this
working previously but the executable would never start properly on
windows and I had no clue why. Turns out running the .exe in wine gave
me a lot more information and the solution was to copy some more .dll
files into the distro folder.

I haven't tested it on actual windows yet. But I'm hoping that getting
something to run under wine should be a good indicator that it will also
play nice on windows (knowing my luck this will turn out to be completely
wrong once I test it).

The only issue that remains with the windows .exe that I've noticed now
is that random doesn't seem to work the same way on windows as it does
on linux.

The following code seems to be missbehaving. Once I confirm it on windows
and solve it I will make a post on what I changed.

{% highlight cpp %}
#include <random>

int getRandomBetween(int a, int b)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(a, b);
	return dis(gen);
}
{% endhighlight %}

If I'm really lucky it's just wine messing it up and not actual
windows. According to the [MSDN][msdn random] page it should work as
expected on windows as well. But when does that ever happen?

### The next step
I've started working on some map sketches in my notepad (I heard this
was a good way to start).  I'll try to get the 3-6 first maps created
and after that I'll try to force some friends to beta-test the game
at their leisure.  I'm thinking of having a sort of learning focus on
these first maps. Challenge the player with basic mechanics forcing him
to learn what to do and what not to do as he plays along. Examples are,
ghost mode, falling death, spikes, double-jump and the boost powerup.

### Travis.ci
Is still broken for my project with no new information to ad.

[valgrind]:		http://valgrind.org/
[msdn random]:	https://msdn.microsoft.com/en-us/library/bb982398.aspx#code
