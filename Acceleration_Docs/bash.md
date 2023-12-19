bash redirection to stderr stream - `>&2`

`>` output redirection operator

`&` specfies file descriptor 

`2` fd for stderr

everything in linux is a file :)

`getopts` - command line arguments in bash. 
Syntax:

```bash
while getopts "a:bcd:" opt; do
#while loop for all incoming options
```

arguments to getopts specified in quotation brackets, 'opt' acts as an internal variable name for the next case statement, its value can change, and the value stored in the 'opt' variable will be placed into `$OPTARG` by `getopts`
arguments terminated by a colon `:` require an argument following their instantiation.

case statement ends w/ `esac`

`getopts` only supports single character arguments. must use `getopt` for 
multicharacter argument calls like --fix-broken. 

an example getopt call may look like this:

```bash
OPTS=$(getopt -o "a:b:c" --long "delta:theta:gamma" -n "$(basename "$0")" -- "$@")

```

`-o`      argument that  absorbs the options specified to `getopt`
`--long`  absorbs the long options specified
`-n`      specifies the name of the script for error messages
`--`      also specifies the end of option arguments specified to getopt. all subsequent args will not be treated as `getopt` options
`$@`      variable representing all args passed to the script

getopt error checking example line

```bash 
if [ $? -ne 0 ]; then
echo "errata"
exit 1
fi
```
`$?`   is a variable that holds the exit status of the last command
`-ne`  not equal operator

option management

script will also take in option and non-option args, if required you can guard your script arg parsing.

a string like ``` eval set -- "$OPTS" ```
will do the following:

`set`   sets script positional parameters

`--`    signals end of options

`eval`  signals to commandline to parse this line as one string

the additional verbosity here leaves getopt and bash with no misunderstanding as to your intentions with variables.

where `getopts` will automatically shift through argument vectors for you, `getopt` won't. that's a minor bummer.

shift 2 to jump next w/ commands requiring args, and one without.

here is an example script. note that here, `$#` signals the number of args and `-gt` is the "greater than" operator

```bash
while [ $# -gt 0 ]; do
  case "$1" in
    -a|--alpha)
      ALPHA="$2"
      shift 2
      ;;
    -b|--beta)
      BETA="$2"
      shift 2
      ;;
    --)
      shift
      break
      ;;
    *)
      echo "Unknown option: $1"
      exit 1
      ;;
  esac
done
```
