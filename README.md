# cScell
A framework written with python, in order to quickly create C shell projects on Linux envinronment.

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
