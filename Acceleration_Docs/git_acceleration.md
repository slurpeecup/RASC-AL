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
![Git_Staging_Image](/Acceleration_Docs/Acceleration_Doc_Images/nm1w0gnf2zh11.png)

[Git_Branching_Image](link_dfajkdfhlkjahdsljkfhklajdsfhlkjasdfjlkhasdkljfhlakdjfhk)


## Conflicts & Conflict Avoidance

![merge_conflcts](/Acceleration_Docs/Acceleration_Doc_Images/Git_Merge_Conflict.png)

Conflicts occur when a branch attempting to merge into or pull from a branch that has a disparate view of the revision history. For example, Engineer A and Engineer B both branch off of `main` into their respective branches `A` and `B`, then begin editing foo.md. Engineer B appends "Hello Wolf!" to the bottom of foo.md. Engineer A appends "Hello Dog!" to the same line. If Engineer B pushes to `main` first, then Engineer A attempts to merge main into their branch or push to main, then a conflict will be observed. Conflicts must be resolved by the user that is behind the main branch. There are online resources that will describe conflict resolution much better than I can, including [here], [over_here], and [here] as well. However, the gist is that the conflicts on the branch must change to reflect `main`'s (or the branch that is being interacted with) understanding of the revision history for the files in conflict. In this case, Engineer A would have to update is version of foo.md to look exactly like what Engineer B pushed to `main`. 

Another example, let's say Engineer A and Engineer B both pull their project from `main` and work on the same directory, `/bar`. This directory contains three files, `1.md`, `2.txt`, and `four.xml` . Engineer B makes some changes to `2.txt`, then deletes `four.xml` , and pushes to main. In the mean time, Engineer A makes modifications to `four.xml`. What happens when Engineer A tries to pull from `main`?

A conflict will not be observed on `2.txt`, as its current state reflects main's understanding of the revision history. On `four.xml`, this merge attempt will be interpreted as "making these changes to the deleted file `four.xml`", as the last understanding of the revision history from main's perspective is that `four.xml` no longer exists. In order to resolve this conflict, Engineer A will have to delete `four.xml`, or at least, move it out of the `/bar` push to main, and then move it back into `/bar` and push again. 

Conflict avoidance is paramount in minimizing headache for yourself and your entire team. Conflict avoidance can be practiced with good team communication and good Git habits. 

In terms of team habits, work on files should be *compartmentalized* between pre-scheduled merges. For example, if the team decides that daily pushes are a good policy to hold, work on a particular file or directory should be assigned to a dedicated individual for the duration of that push cycle. These assignments can be tracked in commit messages, commit descriptions, or ideally, a more comprehensive external team management tool. 

During active work, if a critical modification is made to a piece of software, it is good practice to push it, even if ahead of schedule (still following the mandated team push schedule) in order to ensure that the critical component is not at risk of having to be removed or otherwise fanangled with in the event of a conflict.

It is also good practice to manually review the difference between your current commit and main before merging, as well as during your development cycle if a new push reaches `main`, then resolving conflicts manually before attempting to make a merge or push. This reduces your likelihood of frustration and keeps expectations in check for what will happen during the next merge attempt. 

Sometimes, merge conflicts are inevitable. When you are not able to prevent a merge conflict, it is incumbent upon the user attempting to merge to resolve the conflict on their end. 

When you go to merge and a conflict is found, Git will insert **conflict markers** around the text blocks that are in contention. You can ```grep``` for ```<<<<<<```,```======```, and ```>>>>>>``` to find files and blocks of code that are in conflict with `main`. 

[Branching Theory I Don't Know Where To Put You Right Now](https://nvie.com/posts/a-successful-git-branching-model/)

[Decent git best practices](https://www.freecodecamp.org/news/how-to-use-git-best-practices-for-beginners/#best-practices-for-using-git)




