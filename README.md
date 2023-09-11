# pengu-mac
🔨 An experimental Pengu Loader for macOS.

## Getting started

### Prerequisites

- MacOS 13+ (`Intel x86-64` / `Apple Silicon arm64`)
- Xcode + Clang compiler (`xcode-select --install`)
- GNU make
- League of Legends installed

### Building

Make sure you have cloned the repo:

```
git clone https://github.com/PenguLoader/pengu-mac.git
```

Then nagivate to the repo folder and build it:

```
cd pengu-mac
make
```

By default, the expected LoL path should be `/Applications/League of Legends.app`. You can change in the `makefile` if it is different.

### Testing

1. Start your **Riot Client**
2. Run `make install`
3. Launch **League of Legends**

Hot keys:
- <kbd>Cmd + Alt + I</kbd> or <kbd>F12</kbd> : open Chrome DevTools
- <kbd>Cmd + Alt + R</kbd> : reload the client

To install plugins, run `make open` to open the plugins folder in Finder and then put your JavaScript files into it. There are some limitations compared to **Pengu Loader**, please read the next section.

A screenshot with [Floyare's Purple theme](https://github.com/floyare/league-purple-theme):

![image](https://github.com/PenguLoader/pengu-mac/assets/38210249/9b146dd4-00ff-4ca5-a034-dd511bb66907)

### Limitations

- No plugin module system.
- No local assets support.
- No remote DevTools
- You can't paste your code into the DevTools' console.
- No loader UI, you need to install it after Riot Client login and before launching LoL.

## Developing

**Visual Studio Code** and **C/C++ extension** are required for development. You also can use **Xcode** but it need some addition configurations.

### Debugging

// todo

### Remote dev

> MacOS and its key bindings is disgusting.

If you prefer to use Windows or Linux, it should be the best choice. You will need to enable remote access and SSH server in system preferences, then connect your VSCode via SSH.

If you have no Mac, try GitHub actions and follow this guide: https://www.youtube.com/watch?v=MZYly2gmmHs 😎.
With headless Mac, you should use NoMachine, a VNC client or Parsec for the best experience.
