## Hello, Gulls
Let's learn some basic commands to become more familiar with UNIX style CLI interfaces.<br>

All of these commands will have additional features, beyond the ones listed.<br>
If you wish to learn more, run one with help arguments.<br>
Those are usually -h or --help. For example: ```git -h```.<br>
For more information, read the documentation on man7.org, or a command's respective knowledge base.<br>

## Essentials
```ls```: List - show me what is in the current directory. ```-a``` will show hidden files. <br>

```cd```: Change directory - move to a new folder. You can move to any directory from anywhere in your terminal if you 
specify the top level directory from either the system root ```"/"``` or system home ```"~/"```, ie ```"cd ~/"```. You 
can move up a directory using ```".."```,and can chain directories by delimiting with ```/```; 
ie. ```cd ../../build/test/results```<br>

```./```: Run program | ie. ```./mainprogram```. You can run programs in different directories by using the same syntax as cd. ie. ```./../../build/test/test.a```<br>

```Ctrl+C```: Escape key to exit a program running in a terminal.

```&```: After a call to run a program, use ampersand to disown it from the terminal instance it is called in, ie. ```./virtuoso &```.

```>```: Pipe terminal output to file | ie ``` ./somecommand > targetfile.txt```<br>

```pwd```: Present working directory - shows what directory you are in.<br>

```mkdir```: Make directory | ie. ```mkdir test_folder```<br>

```rmdir```: Remove directory | ie. ```rmdir test_folder``` ! directory must be empty unless using ```-f``` argument.<br>

```touch```: Create a file | ie. ```"touch filename.f"```<br>

```nano```: GNU nano text editor, call nano then name of file. <br>

```rm```: Remove - remove file |``` rm -rf /d 'directoryname'``` is a nuke, use w/ caution.<br>

```&&```: Run command and next command if prior is successful | ie. ```mkdir a && cd a && cd .. && rmdir a``` <br>

```||```: Run command and next command regardless of success | ie. ```sudo apt install aplay || sudo apt install inkscape``` (aplay will fail, inkscape will still install)

```/.```: Hidden file or dir. The focus here is on the ```.``` in front of the subject | ie. ```cd ~/.hiddenfolder || nano .hiddenfile```

While we're here... doing things with folder and file names that have spaces
is really annoying. <br> By default, the terminal will use the " " character as a delimiter. If you need to do this, the name of the target must be wrapped in quotes<br>
ie. ```"rmdir "test file"``` or ```rmdir `test file`"```.<br>

Aliases: In the .bashrc file, you can instantiate custom shortcuts for commands <br>
called aliases. ```alias ="cd ~/Documents/Vivado && ./vivado2022.4``` <br>

## SUDO

```sudo```: Superuser do - use admin permissions... requires password & can be dangerous <br>

The following commands require sudo access. To run a command with sudo, just <br>
wire it at the beginning of the command, ie. ```sudo nano ~/.bashrc```<br>

```apt```: This deserves a full subtopic, so I'm going to start one.<br>

```apt``` is the default package manager on Debian based systems, Debian being a <br>
very common Linux distribution for other distros to base off of. The OS on the <br>
RPI4s and the Orin are Raspberry Pi OS and Jetpack 5, both based on Debian. <br>
taking this a step further, Jetpack 5 is based on Ubuntu 20, which is Debian based <br>

Some common ```apt``` commands you will use are ```install```, ```remove```, ```update```, ```upgrade```.<br>
You will come across many more as you comb through forum posts. You don't have <br>
to remember them all, but understanding what they do is helpful sometimes.<br>

```apt install```: Checks if a packages is in the ```apt``` sources list, if it is then install ie. ```sudo apt install supertuxkart``` will install supertuxkart, a really cool game. You can install multiple packages in the same ```apt install``` command by delimiting package names with a space, for example: ```sudo apt install git-all inkscape``` to install git AND inkspace. <br>

```apt remove```: Opposite of install...<br>

```apt update```: Checks all system packages that ```apt``` is aware of to see if there is <br>
and update available at the source. If so, it lets you know. <br>

```apt upgrade```: Applies those updates.<br>

```source```: Refreshes the file for programs dependent upon it. If I add an alias to ```.bashrc```, then I will need to run ```source ~/.bashrc``` to use that alias.<br> 

```chmod```: Change permissions. ```(+-)rwx read write execute```. ie. ```sudo chmod -x+w setup.sh```<br>

```dpkg```: Manual package management from .deb. -i installs, -r removes, ie. ```sudo dpkg -i package.deb```<br>

```tar```: Unpack contents of a tar archive. Read more on tar [here](https://www.geeksforgeeks.org/tar-command-linux-examples/).<br>

## Bash Scripting
Bash scripting allows you to package multiple commands into a single script that will run sequentially. You can alias the run script for these files to simplify the calling of frequent command groups youd like to use. You initialize a bash script by creating a file with extension .sh, for example: ```cd ~/.bashscripts && nano fun_script.sh```

A critical command to know for running multiple processes from a bash script is ```disown```. Read more [here](https://phoenixnap.com/kb/disown-command-linux).

Every bash script starts with ```#!bin/bash/```
Subsequent scripts will follow. Here is an example of a simple one.
```
#!/bin/bash
sudo thunar /usr/local/share/'older plus themes'/WINDOWS/Resources/Themes &
cd /home/slurpeecup/Downloads/Chicago95/Plus
./PlusGUI.py &
```
Learn more on bash scripting at the [bash scripting cheatsheet](https://devhints.io/bash).

## Conclusion
Okay, now that you've got some background, play one of these three games. They're designed to give you more famililarity with CLI interfaces.<br>

[Bashcrawl](https://gitlab.com/slackermedia/bashcrawl)<br>
[Terminus](https://web.mit.edu/mprat/Public/web/Terminus/Web/main.html)<br>
[CLI Mystery](https://github.com/veltman/clmystery)

Beyond this, if you do not already own one, I recommend you dual-boot (if hard drive space permits) or purchase a beater computer for UNIX development. Good, high performance, < $200 candidates are the Lenovo Thinkpad T440P and HP Elitebook series, especially the Ryzen 7 / Core i7 equipped G5-G7s. I use a G5, and they are fruitful and abudant on Ebay. Linux dual-booting is fairly straight forward, although the experience is not the same for every device. A good starter Debian-based distribution would be [Linux Mint](https://linuxmint.com/edition.php?id=306), [MX Linux](https://mxlinux.org/download-links/), or [Pop!_OS](https://pop.system76.com/). I will not discuss my feelings on Ubuntu. 

There are plenty of great docs out there that touch on Linux dual-booting, and I would refer you to [Google](https://google.com) to find a more detailed one. For an originally windows-based machine, the case and point is that you will need to download a disk image (such as one linked above), then write it to a USB flash drive using software like [Rufus](https://rufus.ie/en/) or [Balena Etcher](https://etcher.balena.io/#download-etcher). You'll then have to go into your BIOS, disable SecureBoot and TPM, change the boot order to set the flash drive on top, and finally run the guest OS on your live USB and use the installer. Partition your hard drive however you wish (defrag in Windows first to minimize risk) and have fun. 

Assuming the git accelerator doc has been written, read that next. 
