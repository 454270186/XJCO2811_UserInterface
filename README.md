# XJCO2811_UserInterface

![c++](https://img.shields.io/badge/C++-00599C.svg?style=for-the-badge&logo=C++&logoColor=white)
![QT](https://img.shields.io/badge/Qt-41CD52.svg?style=for-the-badge&logo=Qt&logoColor=white)

当你在本地完成了一部分工作并希望将更改推送到远程仓库时，可以按照以下流程进行操作：

1. 确保你的本地分支是基于远程仓库的最新代码创建的，并且你的更改已经提交到本地分支。

    切换到主分支（例如 main 或 master）并拉取最新的代码：

    ```shell
    git checkout main
    git pull origin main
    ```

2. 切换回你的分支：

    ```shell
    git checkout your_branch
    ```

3. 将主分支的最新代码合并到你的分支：
    ```shell
    git merge main
    ```
    如果在合并过程中出现冲突，需要解决冲突并手动编辑文件以保持代码的一致性。


4. 提交合并后的更改：
    ```shell
    git add .
    git commit -m "Merge main into your_branch"
    ```

5. 推送你的分支到远程仓库：
    ```shell
    git push origin your_branch
    ```
    这将把你的本地分支的更改推送到远程仓库的你的分支。


6. 在远程仓库中打开你的分支，并创建一个 Pull Request（PR）。

    在 PR 中提供标题和描述，解释你的更改内容和目的。
    邀请其他团队成员进行代码审查和讨论。
    根据审查意见进行必要的修改和提交。

7. 当你的更改得到批准后，将其合并到主分支。
