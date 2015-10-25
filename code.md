---
title:  "Code"
layout: post
date:   2015-10-25 22:08
categories: code
---

### Source code can be found here:
[http://www.github.com/liquidityc/justdontdie](http://www.github.com/liquidityc/justdontdie)

### Compiling
Apart from the build-essentials you'll need to install the libraries listed below.
Once that is done running 'make' should be fine if you're on archlinux. If not it's probably paths that
are wrong. Shouldn't be too hard to figure out. If it's tricky just contact me.

If you want to build in debug mode then do the following

{% highlight bash %}
cp Makefile.config.template Makefile.config
cd flat && cp Makefile.config.template Makefile.config && cd ..
{% endhighlight %}

### Libraries
- [boost](http://www.boost.org/)
- [Catch](https://github.com/philsquared/Catch)
- [SDL2](https://www.libsdl.org/index.php)
- [SDL2-image](http://www.libsdl.org/projects/SDL_image/)
- [SDL2-mixer](https://www.libsdl.org/projects/SDL_mixer/)
- [SDL2-ttf](http://www.libsdl.org/projects/SDL_ttf/)
