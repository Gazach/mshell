# 🌕MoonShell

> [!WARNING]
> THIS SHELL IS CURRENTLY WORK IN PROGRESS! ANYTHING CAN CHANGE AT ANY MOMENT WITHOUT ANY NOTICE! USE THIS SHELL AT YOUR OWN RISK!

MoonShell is a native Windows shell with a Linux aesthetic (larp). It's fully native to Windows — there's no POSIX layer underneath, so it won't run Unix programs on its own. If you want that, you'll need something like a pre-built MinGW environment from [MSYS2](https://www.msys2.org/) (or similar) already on your system PATH.

## Features

- **Feels like a real terminal, not a wrapper.** MoonShell takes over the console directly (raw input, its own cursor and redraw handling) instead of leaning on `cmd.exe`'s line editing.
- **Familiar line editing** — arrow keys, Home/End, Delete, and Backspace all work the way you'd expect.
- **Command history** — browse previous commands with the Up/Down arrows.
- **Readline-style kill shortcuts** — delete a word, clear the line, or kill to the start/end of it without reaching for the mouse.
- **Typo suggestions** — mistype a command and MoonShell will point you at the closest real one (`la` → "Did you mean \"ls\"?").
- **Interactive `help` menu** — built-in commands are grouped by category in a simple numbered menu, similar to a phone's call menu.
- **Runs anything on your PATH** — if a command isn't built in, MoonShell looks for it on PATH the same way `cmd.exe` would (`.exe`, `.com`, `.bat`, `.cmd`, respecting `PATHEXT`) and runs it — so `code .`, `git status`, `python`, etc. all work normally.

## Built-in commands

### Basic

| Command | Description |
| --- | --- |
| `echo` | Print text back to the screen |
| `date` | Show the current date and time |
| `exit` | Quit the shell |
| `clear` / `clr` | Clear the screen and scrollback |
| `help` | Show the interactive help menu (or `help <command>` for a direct lookup) |

### File & Directory

| Command | Description |
| --- | --- |
| `cd` | Change the current directory |
| `ls` | List files and folders in the current directory |
| `blank` | Create a new empty file |
| `mkdir` | Create a new directory |
| `rmdir` | Remove a directory |
| `rm` | Remove a file |
| `mv` | Move or rename a file |
| `cp` | Copy a file |
| `pcd` | Print the current/working directory |

Anything else is looked up on PATH before MoonShell gives up on it.

## Keyboard shortcuts

| Shortcut | Action |
| --- | --- |
| `↑` / `↓` | Browse command history |
| `←` / `→` | Move the cursor |
| `Home` / `End` | Jump to the start / end of the line |
| `Delete` | Delete the character ahead of the cursor |
| `Backspace` | Delete the character behind the cursor |
| `Ctrl+W` | Delete the word behind the cursor |
| `Ctrl+U` | Kill from the cursor to the start of the line |
| `Ctrl+K` | Kill from the cursor to the end of the line |
| `Alt+C` | Clear the whole line |

## Building from source

You'll need [CMake](https://cmake.org/), [Ninja](https://ninja-build.org/), and [LLVM/Clang](https://releases.llvm.org/) for Windows.

```bat
build.bat debug     REM or: build.bat release
```

This configures and builds with CMake + Ninja + Clang, and drops the binary at:

```
build\<debug|release>\bin\mshell.exe
```

If Clang is installed somewhere other than `C:\Program Files\LLVM\bin`, update `CLANG_PATH` near the top of `build.bat` first.

## Usage

```
$MShell C:\Users\you\projects> ls
$MShell C:\Users\you\projects> cd moonshell
$MShell C:\Users\you\projects\moonshell> code .
$MShell C:\Users\you\projects\moonshell> git status
$MShell C:\Users\you\projects\moonshell> la
Unknown Command! : la
  Did you mean "ls"?
$MShell C:\Users\you\projects\moonshell> help
=== MoonShell Help ===
Pick a category:

  1) Basic
  2) File & Directory

  0) Exit help

help>
```

## Roadmap

- [ ] Pipelines and redirection (`|`, `>`, `>>`, `<`)

## License

MIT License — see [LICENSE](LICENSE) for the full text.
