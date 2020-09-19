# cScell
A framework written with python, in order to quickly create C shell projects on Linux envinronment.

<p align="center">
  <img src="https://github.com/euberdeveloper/cScell/raw/master/docs/example.gif">
</p>

## Install

**Note**: this package is thought to be used by **Linux** platforms.

You can install cScell with `pip` or `pip3`:

```bash
$ pip3 install cScell
```

The package is usually used as a global module. It is possible that a warning will appear during the installation, saying that the location of the package is not on the `$PATH` variable. In order to be able to use it from any directory, add that location to the `$PATH` variable.

## Usage

### Syntax

The syntax of the command is:

```bash
cScell [app_name] [commands_json_path] [output_dir_path]
```

The three parameters are:
* __app_name__: The name of the project. Default: `scell_project`.
* __commands_json_path__: The path to the json file describing the commands of the shell program. Default: `./commands.json`.
* __output_dir_path__: The path where the project directory will be created. Default: `./`.

### Examples

This example will create in the current folder a project called "myProject":

```bash
cScell myProject
```

This example will create in the parent folder a project called "myProject", with the commands described in "my-commands.json":

```bash
cScell myProject ./my-commands.json ../
```

### Usage as local module:

The module could be actually used also as a local module, here is an example:

```python
from cScell.cScell import generate_scell

APP_NAME = 'myProject'
JSON_COMMANDS = './my-commands.json
OUTPUT_PATH = '../'

generate_scell(APP_NAME, JSON_COMMANDS, OUTPUT_PATH)
```

## Json format

You can describe the **commands** of your c shell program with a **json file**.

In the commands json file:
* Each root **key** is the name of a **command** of the shell. Its value must be an **object**.
* The special `_arguments` **command** is the command that could be executed when the user passes command line arguments to the program.
* An exception for what was said above is the `promptSymbol` **key**, that specifies the **symbol** displayed **before each line of the prompt**. If **not specified**, its value will be `>>`.
* For **each command**, the `function` property indicates the **name** of the function that **handles** that command.
* For **each command**, the `arguments` property indicates the arguments accepted by that command. Its value must be an **object**.
* For **each arguments object**, every **key** is the **name** of an argument and its value must be an **object**.
* For **each argument value**, the key `type` is mandatory and describes the **variable type** of the argument. The possible options are: `int`, `long`, `float`, `double`, `char`, `char*`, `array<type>` (where `type` is one of the precedent types).
* For **each argument value**, the key `alias` is not mandatory and describes an **alias** (**single character**) that could be used instead of the extended argument name.
* For **each argument value**, the key `default` is not mandatory. If it is not specified, the argument is mandatory when the command is typed by the user. NB: The **bool** type is **always false as default**. The **array** type **can only be "NULL"** as default if it is not a required argument.
* For **each argument value**, the key `min` is not mandatory. It can be used with **numeric** types and indicates that the passed argument value must be **bigger or equal** than the value of **min**.
* For **each argument value**, the key `max` is not mandatory. It can be used with **numeric** types and indicates that the passed argument value must be **lower or equal** than the value of **max**.
* For **each argument value**, the key `description` is not mandatory. It is the displayed **description for the argument** when the command **help** is executed.
* For **each command**, the key `description` is not mandatory. It is the displayed **description for the command** when the command **help** is executed.

An example:

```json
{
    "promptSymbol": ">>>",
    "_arguments": {
        "function": "arguments",
        "arguments": {
            "thing": {
                "type": "int",
                "alias": "t",
                "default": 1,
                "min": 0,
                "description": "The thing argument description"
            },
            "stuff": {
                "type": "float",
                "alias": "s",
                "max": 10,
                "description": "The stuff argument description"
            },
            "ware": {
                "type": "array<char*>",
                "alias": "w",
                "default": "NULL",
                "description": "The ware argument description"
            }
        }
    },
    "increment": {
        "function": "inc",
        "arguments": {
            "number": {
                "type": "int",
                "alias": "n",
                "description": "The number to increment"
            },
            "amount": {
                "type": "int",
                "alias": "a",
                "default": 1,
                "description": "How much will the number incremented"
            }
        },
        "description": "Increments a number"
    },
    "add": {
        "function": "add",
        "arguments": {
            "first": {
                "type": "int",
                "alias": "x",
                "description": "The first addend"
            },
            "second": {
                "type": "int",
                "alias": "y",
                "description": "The second addend"
            }
        },
        "description": "Adds two addends"
    },
    "concat": {
        "function": "concat",
        "arguments": {
            "strings": {
                "type": "array<char*>",
                "default": "NULL",
                "description": "The strings to concat"
            },
            "reverse": {
                "type": "bool",
                "alias": "r",
                "description": "If the strings will be in reversed order"
            }
        },
        "description": "Concats an array of strings"
    },
    "quit": {
        "function": "quit",
        "description": "closes the program"
    }
}
```

## Result

At the end of execution, a **project folder** called as you specified for the `app_name` argument will be created in the `output_dir_path` that you specified.

That folder is **structured** as specified below:
* There will be a `compile.sh` bash script. It is the script to **compile** the **c generated source code**. If it is not already executable, you can execute `sudo chmod +x compile.sh`.
* There will be a `[app_name].c` file, named as the **name of the project**. It will contain the **main function** of the program, it imports the **commands interpreter** generated by the module and the **commands handlers** that you should implement and will **run the shell loop**.
* There will be the `shellutils` folder, containing all the `libraries` that make the shell work. These libraries are:
    - `colour`, for a coloured output
    - `constructs`, containing only header files, a `boolean.h` for the boolean enum and a `sh_state.h` for the state of a command enum.
    - `logger`, used by the shell to log errors or warnings.
    - `text`, containing all the text functions and the advanced terminal.
    - `shell_utils`, used by the shell_commands library
    - `shell_commands`, the only one that **depends** on the `commands.json` file passed before creating the project. It will contain the code that **parses the shell commands** prompted by the user and **calls the right function handler**
* There will be the `handlers` folder, containing the `handlers.h` and `handlers.c` files. You will **edit** the `handlers.c` files in order to define the code that will be executed when the user prompts a specific command.

There will be also **three** variables exported by `shell_commands.h`:
* `char *sh_last_command`: It is the string variable containing the last prompted command
* `char *sh_prompt_symbol`: It is the variable whose value is the string that specifies the **symbol** displayed **before each line of the prompt**.
* `bool sh_use_advanced_terminal`: It is the boolean variable that determines if the **advanced terminal** will be used. The advanced terminal makes possible to the user to use the **up and down arrows** to navigate the **commands cronology**. Its default value is `true`, but you can change it before the `sh_loop();` call in the `[app_name].c` file.

To start the program you just need to execute:
```bash
cd [your_project_dir]
sudo chmod +x ./compile.sh
./compile.sh
./[your_app_name]
```
This will compile the c code and start the generate executable program.

You should **edit** the `handlers.c` file to define the behaviour of each command.

## What the framework does

Under the hood:
* All arguments containing **hyphens** in the name will be purged with **underscores**.
* If the `_arguments` command was defined, if the users passes **command line arguments** they will be parsed and the corrispondent handler executed.
* An **infinite loop** that **listens** for the user's commands will be then executed.
* All **empty prompted lines** will be ignored.
* After a command is prompted, the program will **check if the command exists**. If not, it will **display a proper error message**.
* If the command exists, it will be **properly** parsed.
* If a **required argument** misses, a **value** is invalid, a **value** misses or does not matches **provided numerical ranges**, an error will be displayed.
* The **extended** argument names should be preceded by two hyphens `--` while the **aliased** argument names by a single hyphen `-`.
* If a **value without argument** will be passed, a warning will be displayed, but it will not block the command.
* If all checks pass and the command is correctly parsed, the corrispondent **handler function** is called with the parsed arguments. You can easily **edit** the `handlers.c` file and implement the handlers, which by default print the command name.
* If the `help` command is prompted, the documentation for all the commands will be displayed
* If the `help [command]` command is prompted, the documentation for the given command will be displayed

## Notes

* This framework works only for **Linux**.
* Only **named** arguments are allowed. This means that all commands will be like `add -x 1 -y 2` or `concat --strings first second third` but commands like `add 1 2` will not be allowed.
* If a string parameter **contains spaces**, you can include it inside double apix such as `concat --strings "first string" "second string"`
