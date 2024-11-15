# Process Hitman
Use Alt+F5 to kill any process, more efficiently than Alt+F4

*Have you ever been in a situation where an application stops responding, gets stuck in a loading screen, or is just annoying. You immediately press Alt+F4, but the application doesn't shut down?*

**Process Hitman will ensure the process is killed.**

## Purpose
Some programs and games are annoying to close once opened. Some games I play don't work with Alt+F4, and I am too lazy to go back through tons of menus to exit a game. There are also some apps that have long splash screens, so if you accidentally open one of them, you have to either wait to close it, or use Task Manager.

I built this because I needed a program like this, nothing groundbreaking, but I hope it helps someone out.

## How it works
It's very simple, if you press Alt + F5, it will kill the current process. This will not show any "Save unsaved changes" menu, it will not wait for the application to save and exit. It will kill the process, hence the name, Process Hitman.

Built using C++ and the Windows API.

## Contributing
Contributors are welcome to fork the repository or send pull requests directly to this repository. I don't intend on releasing any updates to this software (except for bug fixes for major security issues), as its purpose is quite simple.  

## Building
You should ideally use the compile.bat script included in the repository. You need the G++ compiler, but you can probably use other compilers as well. I used the MinGW G++ compiler.

```shell
compile.bat
```

## License

MIT License

Copyright (c) 2024 zyapguy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


## Attribution

Unedited App Icon by freepik: [Link](https://www.freepik.com/free-vector/mysterious-gangster-character_7075529.htm#fromView=keyword&page=1&position=6&uuid=77c07a48-2a0d-497c-be29-154325ce7e35)
