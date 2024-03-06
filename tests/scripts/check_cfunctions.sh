#!/bin/bash

# 実行ファイルが指定されているか確認
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <executable file>"
    exit 1
fi

# 実行ファイルの存在を確認
if [ ! -f "$1" ]; then
    echo "Error: No such file: $1"
    exit 1
fi

# チェックする関数のリスト
functions=(execve dup dup2 pipe strerror gai_strerror errno dup dup2 \
fork socketpair htons htonl ntohs ntohl select poll epoll epoll_create \
epoll_ctl epoll_wait kqueue kqueue kevent socket accept listen send recv \
chdir bind connect getaddrinfo freeaddrinfo setsockopt getsockname \
getprotobyname fcntl close read write waitpid kill signal access stat opendir readdir closedir fcntl)

# nm コマンドを使用して未定義のシンボルを取得
undefined_symbols=($(nm -u "$1" | grep "^_[a-z]" | sed 's/^_//' | tr '\n' ' '))

# 各関数についてチェック
for symbol in "${undefined_symbols[@]}"; do
	found=0
	for func in "${functions[@]}"; do
		if [[ "$symbol" = "$func" ]]; then
			echo -e "\033[32m$symbol\033[0m" # 緑色で表示
			found=1
			break
		fi
	done
	if [ "$found" -eq 0 ]; then
		echo -e "\033[31m$symbol\033[0m" # 赤色で表示
	fi
done

echo "赤色の関数は禁止関数です"
echo "ただし、プログラム内に記述されていない場合があるのでコードを確認してください。"

exit 0
