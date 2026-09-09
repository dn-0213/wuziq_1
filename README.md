# wuziq_1 · 五子棋（Gomoku）

15×15 五子棋（连珠）项目，包含 **网页版**（画面精美、可玩性强）与 **C 终端版** 两套实现，以及早期开发过程中的 C 单文件版本与测试代码。

## 目录结构

```
wuziq_1/
├── web/                 # 🎨 网页版五子棋（单文件 HTML，双击即玩）
│   └── gomoku.html
├── terminal/            # 🖥 新版 C 终端版（完善后的多文件工程）
│   ├── main.c           #    主程序（健壮输入：A1 / A 1 / a,1，U/R/Q 指令）
│   ├── game.h / game.c  #    游戏逻辑（15×15、悔棋、重开、胜负/平局判定）
│   ├── ui.h   / ui.c    #    终端界面（坐标网格棋盘，UTF-8 控制台适配）
│   └── build.bat        #    Windows/MSVC 一键编译脚本
├── main.c / game.c / game.h / ui.c / ui.h   # 早期 C 版（根目录，历史版本）
├── wuziq_clean.c        # 早期 C 单文件版
├── wuziq_win32.c        # 早期 Windows 单文件版
├── test_game.c          # 游戏逻辑测试（配合根目录 build.bat / Makefile）
├── Makefile             # 根目录测试构建（gcc）
├── design.md / tasks.md # 设计与任务文档
└── .arts/               # 工作区配置
```

## 🎨 网页版（推荐体验）

`web/gomoku.html` —— 单文件、零依赖、纯前端，浏览器双击打开即可游玩。

功能：木质棋盘画面、双人本地对战、悔棋（快捷键 `U`）、胜/平/负比分、每步倒计时（15/30/60 秒/不限时，超时判负）、认输、胜利庆祝页面（彩带 + 音效）、落子/胜利连线高亮、移动端触屏支持、可静音。

## 🖥 C 终端版（`terminal/`）

运行方式（任选其一）：

- **Windows + MSVC**：双击或命令行运行 `build.bat`，生成并执行 `gomoku.exe`。
- **gcc / clang**：`gcc -Wall -Wextra -o gomoku main.c game.c ui.c`

玩法提示：`A1`（或 `A 1`）落子；`U` 悔棋；`R` 重新开始；`Q` 退出。黑方 X 先手，白方 O。若终端中文/棋盘乱码，先执行 `chcp 65001`。

> 注：根目录的早期 C 版本（`main.c`/`game.c` 等）与 `terminal/` 新版接口不同，两者独立编译；`build.bat`/`Makefile` 位于根目录，属于早期测试工程，请勿混淆。
