# pengu-mac
🔨 An experimental Pengu Loader for macOS.

## Getting started

### Prerequisites

- MacOS 13+ (`x86-64`/`arm64`)
- League of Legends installed

### Installing build tools

Get Clang compiler & GNU make:

```
xcode-select --install
brew install make
```

### Building

Make sure you have cloned the repo:

```
git clone https://github.com/PenguLoader/pengu-mac.git
```

Then nagivate to the repo folder and clone dependencies:

```
cd pengu-mac
git clone https://github.com/nomi-san/cef-include -b 91.1.22-mac include
```

Build it:

```
make
```

By default, the expected LoL path should be `/Applications/League of Legends.app`. You can change in the `makefile` if it is different.

### Testing

1. Start your **Riot Client**
2. Run `make install`
3. Launch **League of Legends**

Hot keys:
- <kbd>Cmd + Alt + I</kbd> or <kbd>F12</kbd> -> open DevTools
- <kbd>Cmd + Alt + R</kbd> -> reload the Client

A screenshot with [Floyare's Purple theme](https://github.com/floyare/league-purple-theme):

![image](https://github.com/PenguLoader/pengu-mac/assets/38210249/9b146dd4-00ff-4ca5-a034-dd511bb66907)

To install plugins, run `make open` to open the plugins folder in Finder and then put your JavaScript files into it. There are some limitations compared to **Pengu Loader**, please read the next section.

To uninstall the Pengu, just close your Riot Client and reopen it then press update your LoL. 

### Limitations

- No plugin module system.
- No local assets support.
- No remote DevTools
- You can't paste your code into the DevTools' console.
- No loader UI, you need to install it after Riot Client login and before launching LoL.

## How it works?

On macOS, there is nothing likes IFEO on Windows, but **dylib inserting** and **dylib proxying** work as well as injection method.
Unfortunately, they should not work because Riot will re-validate files every you launch game. The method is used in this repo, is replacing `libEGL.dylib` (via `make install`) after you sign in Riot Client.

`libEGL.dylib` is a runtime library from ANGLE project, a part of Chromium project aims to support hardware accelerated GLES. It is also a dependency of Chromium Embedded Framework (libcef), then we replace it and do proxying back to the original dylib is placed in Pengu root directory. By using `install_name_tool`, we can replace dependency path of libcef and original dylib, and then use `dladdr()` to obtain related paths in Pengu.

## Developing

**Visual Studio Code** and **C/C++ extension** are required for development. You also can use **Xcode** but it need some addition configurations.

### Debugging

// todo

### Remote dev

> MacOS and its key bindings is disgusting.

If you prefer to use Windows or Linux, it should be the best choice. You will need to enable remote access and SSH server in system preferences, then connect your VSCode via SSH.

If you have no Mac, try GitHub actions and follow [this guide](https://www.youtube.com/watch?v=MZYly2gmmHs).
With headless Mac, you should use NoMachine, a VNC client or Parsec for the best experience.
