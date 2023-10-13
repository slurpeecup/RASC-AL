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

Make sure to verify you username and email by using the same commands without appending your email to the end. They should return the value currently stored in that config variable, ie ```git config --global user.name``` should return `myuser`.

There are ways to hang onto your personal access token for ease of use. One method is to use a [Git Credential Manager](https://github.blog/2022-04-07-git-credential-manager-authentication-for-everyone/). If you do not wish to use this, you can append your .bashrc file like [this](https://devconnected.com/set-environment-variable-bash-how-to/) (scroll down to "Setting Permanent Environment Variables In Bash), then ```$echo``` the variable name when you need to copy it. You can also clone the repo with your personal access token like this:

    git clone https://${GITHUB_TOKEN}:@github.com/${GITHUB_REPOSITORY}

You should not need to use your PAT to push with this method.

## Git Essentials
![Git_Staging_Image](/Acceleration_Docs/Acceleration_Doc_Images/nm1w0gnf2zh11.png)

[Git_Branching_Image](link_dfajkdfhlkjahdsljkfhklajdsfhlkjasdfjlkhasdkljfhlakdjfhk)


[Branching Theory I Don't Know Where To Put You Right Now](https://nvie.com/posts/a-successful-git-branching-model/)

test 111111

