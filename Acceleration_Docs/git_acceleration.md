## Introduction

Revision management tools (Git, SVN, SCM) are critical in enabling large teams to develop a software stack independently while avoiding conflicts in the code base. Git is the most popular revision management tool in the modern workspace, although alternatives exist. For ease-of-use, we will be using Git on this project. This guide assumes you are using either:
* Git Bash Terminal on a Windows machine or 
* The terminal on a UNIX-based machine (Mac, Linux). 

For Linux discussion, I will be assuming you are using a Debian-based distribution. If you are not using one, resources related to your distro should still be plentiful online.

This guide will cover mainly cover Git on the command line. Desktop GUIs for Git & Github do exist and are supported on Windows, but aside from ease-of-use visualizing the revision tree of the project, there aren't many advantages to using them exclusively. Being able to use Git from your terminal frees you from dependence on the GUI in development environments where you do not have access to a GUI. 

## Getting Started

If you do not already have Git, you need to install it. 
 * For Windows devices, install Git from [here](https://git-scm.com/download/win).
 * On MacOS 10.9 and higher, you will be prompted to install Git upon the first time you try to execute Git from the terminal. Use ```$ git --version``` to verify or prompt for installation.
 * On Linux, use ```sudo apt install git-all install-info``` from your command line.
You will also need to make an account on [github.com](https://github.com/). To push to Git from the CLI, you will also need a personal access token. In order to create one, go to Settings > Developer Settings > Personal Access Tokens > Tokens Classic on Github, then click "Generate New Token" near to top of the page. Make a quick not describing what this token is for, select the access perms you need to give this token (usually just repo for the case of the project), and then click "Generate Token" at the bottom of the screen. Save this token somewhere permanent, as you will only be able to see it until you close the window. **You will not be able to see this token on Github again**.

In order to set up your git credentials to be able to push, you need to set a user email and password. To do so, use: 

    git config --global user.email [myemail@myemailplace.com] 
    git config --global user.name [myuser]

Make sure to verify you username and email by using the same commands without appending your email to the end. They should return the value currently stored in that config variable, ie. ```git config --global user.name``` should return `myuser`.

There are ways to hang onto your personal access token for ease of use. One method is to use a [Git Credential Manager](https://github.blog/2022-04-07-git-credential-manager-authentication-for-everyone/). If you do not wish to use this, you can append your .bashrc file like [this](https://devconnected.com/set-environment-variable-bash-how-to/) (scroll down to "Setting Permanent Environment Variables In Bash), then ```$echo``` the variable name when you need to copy it. You can also clone the repo with your personal access token like this:

    git clone https://${GITHUB_TOKEN}:@github.com/${GITHUB_REPOSITORY}

You should not need to use your PAT to push with this method.

## Git Essentials

#### Repository: 
A place where things are stored. In the case of Git, a remote repository would be where the master version of the code base is stored, ie. `Github` or Bitbucket. A `local repository` is the directory on your file system where the code base is stored. You can make initalize a copy of a repository on your system by `cloning` it.

![Git_Staging_Image](/Acceleration_Docs/Acceleration_Doc_Images/nm1w0gnf2zh11.png)

#### Working Directory:
 In your local repository, the area you are doing work in is called your working directory. This is more of a semantic term, but for example, let's say that your local repository is stored at `/User/Documents/Repo`, and you are doing work constrained within `/User/Documents/Repo/MotorControl`. Your working directory at the time would be `.../Repo/MotorControl`.

#### Snapshots:
Git `commits` are considered well as snapshots of the code base that refer back to preious revisions. Git commits are each given a unique ID number, which can be used to refer back to a commit of interest. Commits are semi-permanent, and unless there is a reason to delete a set of commits it is not recommended. 

![Git Branch](/Acceleration_Docs/Acceleration_Doc_Images/git-commit-history.png)

#### Staging Area: 
The staging area is where you stage, or assign, files to be committed in a snapshot. For example, let's say I have three files `1.md`, `2.md`, and `four.txt`. If I just wanted to commit `2.md`, I would use ```git add 2.md```. That way, modifications to the other two files will not be reflected in this commit. We will discuss git commands further in this document.

#### Push

After staging your target files, you are ready to push them, setting them as the head, or most recent, commit for either `main` or a branch of interest. 

It is important to note that files are not phyiscally moved in between these workflow partitions. Moving files between working, staging, and commit changes how Git interacts with them, not how you interact with them physically.

### Branches

Branches in Git are paths away from `main`'s understanding of the revision history, where the structure between `main` and the branch are shared at a commit relative to `main` or another branch of interest. Branching is useful in compartmentalizing development. For example, one might make a branch to focus on motor control firmware exclusively. This reduces risk in the `main` branch by isolating all changes to the `motor control` branch until the engineer decides it is ready to be merged back to main.

## Git Commands
Each command in this list is hyperlinked to Atlassian's Git documentation, as their documentation is easy to read and will provide much more information on the behavior and usage of each command. Working with Git, you will probably still need to google for a specific command if you do not already know what to use (ie. "How to push from local repository to a remote branch"), but this list should still help foster some familiarity with the names & general idea. This is not a comprehensive list, but more of a starting point and a set of commands you will use regularly. 


[git init](https://www.atlassian.com/git/tutorials/setting-up-a-repository/git-init) - Creates the `.git` folder in the directory it is called in, initializing a local repository.

[git remote](https://www.atlassian.com/git/tutorials/syncing) - Allows interfacing with remote repositories. For example, using `git remote -v` will list all remote repositories.

[git clone [repo]](https://www.atlassian.com/git/tutorials/setting-up-a-repository/git-clone) - Clones a Git repository into the directory it is called in. You can specify a target directory as an argument, calling as `git clone [repo] [directory]`. 

[git config](https://www.atlassian.com/git/tutorials/setting-up-a-repository/git-config) - Allows the user to make changes to their Git configuration file either on the global (system wide) level, or the project level. Git config was used in `Getting Started` section of this document.

When you clone the RASC-AL repo, make sure to navigate to the repo folder and call `git config credential.helper store` to store your PAT after its first usage. You should not need to enter it again afterwards in order to push.

[git status](https://www.atlassian.com/git/tutorials/inspecting-a-repository#:~:text=The%20git%20status%20command%20displays,regarding%20the%20committed%20project%20history.) - Gives the user status information on the working directory and staging area of the branch you are currently in.

[git diff](https://www.atlassian.com/git/tutorials/saving-changes/git-diff) - Compare differences between prior commit and current working directory.

[git log](https://www.atlassian.com/git/tutorials/git-log) - Gives the user information on the current commit history of the branch they are in. 

![Git_Log](/Acceleration_Docs/Acceleration_Doc_Images/Git_Log.png)

[git branch [branch_name]](https://www.atlassian.com/git/tutorials/using-branches) - Create a new branch. `git branch -a` lists all branches. 

[git checkout [branch_name]](https://www.atlassian.com/git/tutorials/using-branches/git-checkout) - Allows you to switch to working in specified branch - `git checkout -b branch_name` will create a branch called "branch_name" AND switch to it.

[git add](https://www.atlassian.com/git/tutorials/saving-changes) - Moves a file to the staging area, called as `git add [filename]`. To add all files in the current directory to the staging area, use a period as the argument for filename.

[git commit](https://www.atlassian.com/git/tutorials/saving-changes/git-commit) -  Commits a "snapshot" of the all changes in the staging area. Commits usually require a message, when you can add with the -m argument, ie. `git commit -m "hello shmello"`. The `-a` argument allows you to commit all changes in the **working directory** - without adding all changes to the staging area first. `git commit --amend` will modify the contents of the most recent commit instead of making a new commit. **Its use can be dangerous**, and I do not recommend calling a commit amend. If you do need to use it, only use it in a local branch and **NEVER ** EVER EVER ** USE IT IN MAIN**. It is very liable to cause a merge conflict for dependent branches, and should only be used in dependent branches.

[git push](https://www.atlassian.com/git/tutorials/syncing/git-push) - Git push will set the most recent commit to `HEAD` of the target branch. **In the RASC-AL repo, do not use --force**, no matter what.

[git pull](https://www.atlassian.com/git/tutorials/syncing/git-pull) - Git pull downloads & updates the most recent commit from the target branch. If pulling from a remote branch, use `git pull remote`. **DO NOT REBASE** without explicitly knowing what you are doing.

[git fetch](https://www.atlassian.com/git/tutorials/syncing/git-fetch#:~:text=In%20review%2C%20git%20fetch%20is,the%20state%20of%20a%20remote.) - Downloads remote contents without pulling. This is considered to be the safer version of pulling, and is used in conjunction with other commands to update the local repo. Read the Atlassian documentation for more info.

[git reset](https://www.atlassian.com/git/tutorials/undoing-changes/git-reset#:~:text=To%20review%2C%20git%20reset%20is,correspond%20to%20the%20three%20trees.) - Reset a branch to a specified commit index. This is useful when you want to undo all changes to your working directory or need to do so to resolve a merge conflict.

## Conflicts

![merge_conflcts](/Acceleration_Docs/Acceleration_Doc_Images/Git_Merge_Conflict.png)

Conflicts occur when a branch attempting to merge into or pull from a branch that has a disparate view of the revision history. For example, Engineer A and Engineer B both branch off of `main` into their respective branches `A` and `B`, then begin editing foo.md. Engineer B appends "Hello Wolf!" to the bottom of foo.md. Engineer A appends "Hello Dog!" to the same line. If Engineer B pushes to `main` first, then Engineer A attempts to merge main into their branch or push to main, then a conflict will be observed. Conflicts must be resolved by the user that is behind the main branch. There are online resources that will describe conflict resolution much better than I can, including [here], [over_here], and [here] as well. However, the gist is that the conflicts on the branch must change to reflect `main`'s (or the branch that is being interacted with) understanding of the revision history for the files in conflict. In this case, Engineer A would have to update is version of foo.md to look exactly like what Engineer B pushed to `main`. 

Another example, let's say Engineer A and Engineer B both pull their project from `main` and work on the same directory, `/bar`. This directory contains three files, `1.md`, `2.txt`, and `four.xml` . Engineer B makes some changes to `2.txt`, then deletes `four.xml` , and pushes to main. In the mean time, Engineer A makes modifications to `four.xml`. What happens when Engineer A tries to pull from `main`?

A conflict will not be observed on `2.txt`, as its current state reflects main's understanding of the revision history. On `four.xml`, this merge attempt will be interpreted as "making these changes to the deleted file `four.xml`", as the last understanding of the revision history from main's perspective is that `four.xml` no longer exists. In order to resolve this conflict, Engineer A will have to delete `four.xml`, or at least, move it out of the `/bar` push to main, and then move it back into `/bar` and push again. 

## Conflict Avoidance

Conflict avoidance is paramount in minimizing headache for yourself and your entire team. Conflict avoidance can be practiced with good team communication and good Git habits. 

In terms of team habits, work on files should be *compartmentalized* between pre-scheduled merges. For example, if the team decides that daily pushes are a good policy to hold, work on a particular file or directory should be assigned to a dedicated individual for the duration of that push cycle. These assignments can be tracked in commit messages, commit descriptions, or ideally, a more comprehensive external team management tool. 

During active work, if a critical modification is made to a piece of software, it is good practice to push it, even if ahead of schedule (still following the mandated team push schedule) in order to ensure that the critical component is not at risk of having to be removed or otherwise fanangled with in the event of a conflict.

It is also good practice to manually review the difference between your current commit and main before merging, as well as during your development cycle if a new push reaches `main`, then resolving conflicts manually before attempting to make a merge or push. This reduces your likelihood of frustration and keeps expectations in check for what will happen during the next merge attempt. 

Sometimes, merge conflicts are inevitable. When you are not able to prevent a merge conflict, it is incumbent upon the user attempting to merge to resolve the conflict on their end. 

When you go to merge and a conflict is found, Git will insert **conflict markers** around the text blocks that are in contention. You can ```grep``` for ```<<<<<<```,```======```, and ```>>>>>>``` to find files and blocks of code that are in conflict with `main`. 


## Additional Resources
[Branching Theory I Don't Know Where To Put You Right Now](https://nvie.com/posts/a-successful-git-branching-model/)

[Git Concepts In Less Than 10 Minutes](https://opensource.com/article/22/11/git-concepts)

[Decent git best practices](https://www.freecodecamp.org/news/how-to-use-git-best-practices-for-beginners/#best-practices-for-using-git)


[Git Commit History](https://opensource.com/article/22/11/git-concepts)
