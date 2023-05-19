# Software Renderer
My first experience with C language started with [Handmade Hero](https://handmadehero.org) series hosted by Casey Muratori. Following along forced me to think about how computers actually work and how will my code be ran, I've also learned how to render things onto the screen without any external dependencies - only using Win32 API. There is a clear boundary between platform specific and universal code, which not only helped port the project to Linux, but also allowed the unviersal code to be recompiled while the program was running. One interesting fact is that the entire program runs on one big chunk of memory - there is no dynamic allocations happening on runtime.

https://github.com/Sokus/software-renderer/assets/26815390/74ebcca4-c4d8-4e28-8e0f-d488b33f1d2d

