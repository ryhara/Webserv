# Config
configファイルの使用について記述する

## 仕様



## Test
- Github Actionsを用いたテスト

1. tests以下に`XX_test.cpp`を作成
2. `tests/test_test.cpp`を参考にテストケースを作成する。（includeは`相対パス`で指定）
3. push or pull_request で自動でテストが回る
4. TODO ローカルでもまわせるようにする