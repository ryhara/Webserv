# Webserv
42Tokyo level 5 project

HTTPサーバーをC++で実装する課題です。

## Development Environment
- M1 MacBook Air / macOS Sonoma 14.2.1
- c++ -v
	- Apple clang version 15.0.0 (clang-1500.1.0.2.5)
	- Target: arm64-apple-darwin23.2.0
	- Thread model: posix
- kqueue()・kevent()を使用 (FreeBSD系)

## Usage

compile
```
make
```

execute

configファイルを引数に取ります。デフォルトでは`./config/default.conf`を用います。
```
./webserv [./config/XXX]
```

## Test
- Github Actions
- Google Test


## Wiki
