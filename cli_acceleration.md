Okay, let's learn some basic commands to become more familiar with UNIX style CLI interfaces.<br>

All of these commands will have additional features, beyond the listed.<br>
If you wish to learn more, run one with help arguments.<br>
Those are usually -h or --help. For example: ```git -h```.<br>
For more information, read the documentation on man7.org.<br>

ls: list - show me what is. <br>

cd: change directory - move to a new folder. You can move to any directory from<br>
anywhere in your terminal if you specify the top level directory from either <br>
the system root "/" or system home "~/", ie "cd ~/"<br>. you can move up a <br>
directory using "..",and can chain directories by delimiting with / <br> 
ie "cd ../../build/test/results"<br><br>

./: run program | ie. ```./mainprogram```. You can run programs in different <br>
directories by using the same syntax as cd. ie. ```./../../build/test/test.a```<br>

pwd: present working directory - shows what directory you are in.<br>

mkdir: make directory | ie. ```mkdir test_folder```<br><br>

rmdir: remove directory | ie. ```rmdir test_folder``` ! directory must be empty<br> 
unless using -f argument.<br><br>

touch: create a file | ie. "touch filename.f"<br>

nano: GNU nano text editor, call nano then name of file. <br>

rm: remove - remove file |``` rm -rf /d 'directoryname'``` is a nuke, use w/ caution.<br>

&&: run command and command | ie. ```mkdir a && cd a && cd .. && rmdir a``` <br>

While we're here... doing things with folder and file names that have spaces<br>
is really annoying. By default, the terminal will use the " " character as a <br>
delimiter. If you need to do this, the name of the target must be wrapped in quotes<br>
ie. "rmdir "test file" or rmdir `test file`"<br><br>

aliases: in the .bashrc file, you can instantiate custom shortcuts for commands <br>
called aliases. ```alias ="cd ~/Documents/Vivado && ./vivado2022.4``` <br>

sudo: superuser do - use admin permissions... requires password & can be dangerous <br>

The following commands require sudo access. To run a command with sudo, just <br>
wire it at the beginning of the command, ie. ```sudo nano ~/.bashrc```<br>

apt: This deserves a full subtopic, so I'm going to start one.<br><br>

apt is the default package manager on Debian based systems, Debian being a <br>
very common Linux distribution for other distros to base off of. The OS on the <br>
RPI4s and the Orin are Raspberry Pi OS and Jetpack 5, both based on Debian. <br>
taking this a step further, Jetpack 5 is based on Ubuntu 20, which is Debian based <br>

Some common apt commands you will use are install, remove, update, upgrade.<br>
You will come across many more as you comb through forum posts. You don't have <br>
to remember them all, but understanding what they do is helpful sometimes.<br><br>

apt install: checks if a packages is in the apt sources list, if it is then install <br>
ie. "sudo apt install supertuxkart" will install supertuxkart, a really cool game <br>
t remove: opposite of install...<br>

apt update: checks all system packages that apt is aware of to see if there is <br>
and update available at the source. If so, it lets you know. <br><br>

apt upgrade: applies those updates.<br>

source: refreshes the file for programs dependent upon it. If I add an alias <br>
to .bashrc, then I will need to run ```source ~/.bashrc``` to use that alias.<br> 

chmod: change permissions. (+-)rwx read write execute. ie. ```chmod -x+w setup.sh```<br>

Okay, now that you've got some background, play one of these three games.<br>
``` https://gitlab.com/slackermedia/bashcrawl
https://web.mit.edu/mprat/Public/web/Terminus/Web/main.html
https://github.com/veltman/clmystery
``` 

Then, go and read the git accelerator doc... whenever that comes out. 
