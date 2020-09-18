#!/usr/bin/python

import sys
import os
import json
import shutil
from pathlib import Path

# Template files

__dirname = os.path.dirname(os.path.realpath(__file__))

HEADER_TEMPLATE = f'{__dirname}/templates/header.template.txt'
C_TEMPLATE = f'{__dirname}/templates/c.template.txt'
SH_PARSE_COMMAND_TEMPLATE = f'{__dirname}/templates/sh_parse_command.template.txt'
SH_HELP_TEMPLATE = f'{__dirname}/templates/sh_help.template.txt'
SH_HELP_HANDLER_TEMPLATE = f'{__dirname}/templates/sh_help_handler.template.txt'
SH_HELP_HANDLER_PARAMETER_TEMPLATE = f'{__dirname}/templates/sh_help_handler_parameter.template.txt'
SH_HANDLE_COMMAND_TEMPLATE = f'{__dirname}/templates/sh_handle_command.template.txt'
SH_HANDLE_COMMAND_CONDITION_TEMPLATE = f'{__dirname}/templates/sh_handle_command_condition.template.txt'
SH_HANDLE_COMMAND_ARRAY_ASSIGNMENT_TEMPLATE = f'{__dirname}/templates/sh_handle_command_array_assignment.template.txt'
SH_COMPILER_TEMPLATE = f'{__dirname}/templates/sh_compiler.template.txt'
HANDLERS_H_TEMPLATE = f'{__dirname}/templates/handlers_h.template.txt'
HANDLERS_C_TEMPLATE = f'{__dirname}/templates/handlers_c.template.txt'
MAIN_TEMPLATE = f'{__dirname}/templates/main.template.txt'

# Utils functions

def purge_name(name: str):
    return name.replace('-', '_')

def library_name(path: str):
    von = path.rfind('/')
    return path[von+1:]

def split_array_argument(argument_type: str):
    if argument_type.startswith('array'):
        von, bis = argument_type.find('<'), argument_type.find('>')
        return 'array', argument_type[von+1:bis]
    else:
        return argument_type, None

def get_function_params(arguments: dict):
    params = []

    arguments = arguments if arguments is not None else {}

    for argument_name, argument_details in arguments.items():
        argument_raw_type = argument_details.get('type')
        argument_type, argument_item_type = split_array_argument(argument_raw_type)

        if argument_type == 'array':
           params.append(f'{argument_item_type}* {purge_name(argument_name)}, int {purge_name(argument_name)}_size')
        else:
           params.append(f'{argument_type} {purge_name(argument_name)}')
    
    return ', '.join(params)

def get_function_arguments(arguments: dict):
    args = []

    arguments = arguments if arguments is not None else {}

    for argument_name, argument_details in arguments.items():
        argument_raw_type = argument_details.get('type')
        argument_type, _ = split_array_argument(argument_raw_type)

        if argument_type == 'array':
           args.append(f'{purge_name(argument_name)}, {purge_name(argument_name)}_index')
        else:
           args.append(f'{purge_name(argument_name)}')
    
    return ', '.join(args)

def get_strcmp_condition(first: str, second: str):
    return f'strcmp({first}, {second}) == 0'

def get_conditional(condition: str, content: str, *, index=0):
    if index == 0:
        return f'if ({condition}) {{\n\t{content}\n}}'
    else:
        return f'else if ({condition}) {{\n\t{content}\n}}'

def tab(text: str, n_tabs: int, *, tab_first=False):
    return '\n'.join([(('\t' * n_tabs) if tab_first or index != 0 else '') + line for index, line in enumerate(text.splitlines())])

def get_argument_assignment(command_name: str, argument_name: str, argument_type: str, write: str):
    return f'!shu_get_{argument_type}_value("{command_name}", "{argument_name}", words[i], &{purge_name(argument_name)}, {write})'

def get_argument_max(command_name: str, argument_name: str, argument_type: str, max_value: str, write: str):
    return f'\n\t|| !shu_check_max_{argument_type}("{command_name}", "{argument_name}", {purge_name(argument_name)}, {max_value}, {write})'

def get_argument_min(command_name: str, argument_name: str, argument_type: str, min_value: str, write: str):
    return f'\n\t|| !shu_check_min_{argument_type}("{command_name}", "{argument_name}", {purge_name(argument_name)}, {min_value}, {write})'

# Generate header file

def generate_header_declared_help_function(command_name: str):
    return f'void sh_help_{purge_name(command_name)}();'

def generate_header_declared_help_functions(commands: dict):
    functions = [generate_header_declared_help_function(command_name) for command_name, _ in commands.items()]
    return '\n'.join(functions)

def generate_header_extern_function(command_details: dict):
    function_name = command_details.get('function')
    arguments = command_details.get('arguments')
    arguments = arguments if arguments is not None else {}
    params = get_function_params(arguments)

    return f'extern SH_STATE {function_name}({params});'

def generate_header_extern_functions(commands: dict):
    functions = [generate_header_extern_function(command_details) for _, command_details in commands.items()]
    return '\n'.join(functions)

def generate_header_declared_function(command_name: str):
    function_name = f'sh_handle_{command_name}'

    return f'SH_STATE {function_name}(char** words, int n_words);'

def generate_header_declared_functions(commands: dict):
    functions = [generate_header_declared_function(command_name) for command_name, _ in commands.items()]
    return '\n'.join(functions)

def generate_header_file(commands: dict):
    extern_functions = generate_header_extern_functions(commands)
    declared_help_functions = generate_header_declared_help_functions(commands)
    declared_functions = generate_header_declared_functions(commands)
    return header_template.format(extern_functions=extern_functions, declared_functions=declared_functions, declared_help_functions=declared_help_functions)

# Generate c file

def generate_c_handle_command_required_declarations(arguments: dict):
    lines = [f'bool is_assigned_{purge_name(argument_name)} = false;' for argument_name, argument_details in arguments.items() if argument_details.get('default') is None and argument_details.get('type') != 'bool']
    return '\n'.join(lines)

def generate_c_handle_command_argument_declaration(argument_name: str, argument_details: dict):
    argument_raw_type = argument_details.get('type')
    argument_type, argument_item_type = split_array_argument(argument_raw_type)
    argument_default = argument_details.get('default')

    if argument_type == 'array':
        return f'int {purge_name(argument_name)}_size = 0;\nint {purge_name(argument_name)}_index = 0;\n{argument_item_type} *{purge_name(argument_name)} = NULL;'
    elif argument_type == 'bool':
        return f'{argument_type} {purge_name(argument_name)} = false;'
    else:
        default_str = '' if argument_default is None else f' = {argument_default}'
        return f'{argument_type} {purge_name(argument_name)}{default_str};'

def generate_c_handle_command_arguments_declarations(arguments: dict):
    lines = [generate_c_handle_command_argument_declaration(argument_name, argument_details) for argument_name, argument_details in arguments.items()]
    return '\n'.join(lines)

def generate_c_handle_command_arguments_condition_assign_argument_array(command_name: str, argument_name: str, argument_item_type: str, argument_default: str):
    write = 'false' if command_name == '_arguments' else 'true'

    if argument_item_type == 'char*':
        assignment = f'{purge_name(argument_name)}[{purge_name(argument_name)}_index++] = strdup(words[i]);'
    else:
        assignment = f'finish = !shu_get_{argument_item_type}_value("{command_name}", "{argument_name}", words[i], &{purge_name(argument_name)}[{purge_name(argument_name)}_index++], {write});'

    assignment_required = f'is_assigned_{argument_name} = true;' if argument_default is None else ''
    last_check = f'finish = !shu_check_noval_array("{command_name}", "{argument_name}", is_assigned_{argument_name}, {write});' if argument_default is None else '' 

    return sh_handle_command_array_assignment.format(argument_name=argument_name, argument_item_type=argument_item_type, assignment=assignment, assignment_required=assignment_required, last_check=last_check)

def generate_c_handle_command_arguments_condition_assign_argument(command_name: str, argument_name: str, argument_details: dict):
    write = 'false' if command_name == '_arguments' else 'true'
    
    argument_raw_type = argument_details.get('type')
    argument_type, argument_item_type = split_array_argument(argument_raw_type)
    argument_max, argument_min = argument_details.get('max'), argument_details.get('min')
    argument_default = argument_details.get('default')

    if argument_type == 'array':
        return generate_c_handle_command_arguments_condition_assign_argument_array(command_name, argument_name, argument_item_type, argument_default)
    elif argument_type == 'bool':
        return f'{purge_name(argument_name)} = true;'
    elif argument_type == 'char*':
        return f'{purge_name(argument_name)} = strdup(words[i]);'
    else:
        assign_str = get_argument_assignment(command_name, argument_name, argument_type, write)
        max_str = get_argument_max(command_name, argument_name, argument_type, argument_max, write) if argument_max is not None else ''
        min_str = get_argument_min(command_name, argument_name, argument_type, argument_min, write) if argument_min is not None else ''
        return f'finish = {assign_str}{max_str}{min_str};'
    

def generate_c_handle_command_arguments_condition(command_name, argument_name, argument_details, *, index):
    write = 'false' if command_name == '_arguments' else 'true'

    argument_alias = argument_details.get('alias')
    argument_default = argument_details.get('default')
    argument_raw_type = argument_details.get('type')
    argument_type, argument_item_type = split_array_argument(argument_raw_type)
    
    
    condition_construct = 'if' if index == 0 else 'else if'
    condition_main = f'strcmp(argument, "{argument_name}") == 0'
    condition_alias = f' || (is_alias && strcmp(argument, "{argument_alias}") == 0)' if argument_alias is not None else ''
    condition = f'{condition_construct} ({condition_main}{condition_alias})'

    check_noval = f'finish = !shu_check_noval("{command_name}", "{argument_name}", n_words, &i, {write});' if argument_type != 'bool' else ''

    assign_is_assigned = tab(f'is_assigned_{purge_name(argument_name)} = true;' if argument_default is None and argument_type != 'bool' and argument_type != 'array' else '', 1)

    assign_argument = tab(generate_c_handle_command_arguments_condition_assign_argument(command_name, argument_name, argument_details), 2)

    return sh_handle_command_condition_template.format(condition=condition, check_noval=check_noval, assign_is_assigned=assign_is_assigned, assign_argument=assign_argument)

def generate_c_handle_command_arguments_conditions(command_name: str, command_details: dict):
    write = 'false' if command_name == '_arguments' else 'true'

    arguments = command_details.get('arguments')
    arguments = arguments if arguments is not None else {}

    command_conditions_blocks = [generate_c_handle_command_arguments_condition(command_name, argument_name, argument_details, index=index) for index, (argument_name, argument_details) in enumerate(arguments.items())]
    command_conditions = '\n'.join(command_conditions_blocks)
    
    unknown_open = 'else {\n\t' if arguments else ''
    unknown_close = '\n}' if arguments else ''
    unknown_argument = f'{unknown_open}shu_unknown_argument("{command_name}", words[i], {write});{unknown_close}'

    return f'{command_conditions}\n{unknown_argument}'

def generate_c_handle_command_required_check(command_name: str, argument_name: str, argument_details: dict):
    write = 'false' if command_name == '_arguments' else 'true'

    argument_default = argument_details.get('default')
    argument_type = argument_details.get('type')

    return (
        get_conditional('!finish', f'finish = !shu_check_required("{command_name}", "{argument_name}", is_assigned_{purge_name(argument_name)}, {write});') 
        if argument_default is None and argument_type != 'bool'
        else '')

def generate_c_handle_command_required_checks(command_name: str, arguments: dict):
    blocks = [generate_c_handle_command_required_check(command_name, argument_name, argument_details) for argument_name, argument_details in arguments.items()]
    return '\n'.join(blocks)

def generate_c_handle_command_command_function(command_details: dict):
    function_name = command_details.get('function')
    arguments = command_details.get('arguments')
    arguments = arguments if arguments is not None else {}
    args = get_function_arguments(arguments)

    return f'{function_name}({args});'

def generate_c_handle_command_function(command_name: str, command_details: dict):
    arguments = command_details.get('arguments')
    arguments = arguments if arguments is not None else {}

    write = 'false' if command_name == '_arguments' else 'true'
    required_declarations = tab(generate_c_handle_command_required_declarations(arguments), 1)
    arguments_declarations = tab(generate_c_handle_command_arguments_declarations(arguments), 1)
    arguments_conditions = tab(generate_c_handle_command_arguments_conditions(command_name, command_details), 3)
    required_check = tab(generate_c_handle_command_required_checks(command_name, arguments), 1)
    command_function = generate_c_handle_command_command_function(command_details)

    return sh_handle_command_template.format(command_name = command_name, required_declarations = required_declarations, arguments_declarations = arguments_declarations, arguments_conditions = arguments_conditions, required_check = required_check, command_function = command_function, write=write)

def generate_c_handle_command_functions(commands: dict):
    functions = [generate_c_handle_command_function(command_name, command_details) for command_name, command_details in commands.items()]
    return '\n'.join(functions)

def generate_c_help_handler_parameter(argument_name: str, argument_details: dict, index: int):
    alias_val = argument_details.get('alias')
    type_val = argument_details.get('type')
    default_val = argument_details.get('default')
    description_val = argument_details.get('description')
    
    name = f'"{argument_name}"'
    alias = f'"{alias_val}"' if alias_val is not None else 'NULL'
    type = f'"{type_val}"'
    default_value = f'"{default_val}"' if default_val is not None else ('NULL' if type_val != 'bool' else '"false"')
    description = f'"{description_val}"' if description_val is not None else 'NULL'

    return sh_help_handler_parameter_template.format(index=index, name=name, alias=alias, type=type, default_value=default_value, description=description)

def generate_c_help_handler_parameters(arguments: dict):
    parameters = [generate_c_help_handler_parameter(argument_name, argument_details, index) for index, (argument_name, argument_details) in enumerate(arguments.items())]
    return tab('\n'.join(parameters), 1)

def generate_c_help_handler_function(command_name: str, command_details: dict):
    arguments = command_details.get('arguments')

    size = len(arguments.keys()) if arguments is not None else 0
    struct_declaration = f'sh_param_details details[{size}];' if arguments is not None else ''
    parameters = '' if arguments is None else generate_c_help_handler_parameters(command_details.get('arguments'))
    parameters_struct = 'details' if arguments is not None else 'NULL'
    command_description = f'"{command_details.get("description")}"' if command_details.get('description') is not None else 'NULL'

    return sh_help_handler_template.format(struct_declaration=struct_declaration, size=size, parameters=parameters, parameters_struct=parameters_struct, command_name=command_name, command_description=command_description)

def generate_c_help_handler_functions(commands: dict):
    functions = [generate_c_help_handler_function(command_name, command_details) for command_name, command_details in commands.items()]

    return '\n'.join(functions)

def generate_c_help_function(commands: dict):
    command_names = commands.keys()
    gen_condition = lambda command_name, index: get_conditional(get_strcmp_condition('command', f'"{command_name}"'), f'sh_help_{purge_name(command_name)}();', index=index)

    all_commands = tab('\n'.join([f'sh_help_{purge_name(command_name)}();' for command_name, _ in commands.items()]), 2)
    conditions = [gen_condition(command_name, index) for index, command_name in enumerate(command_names)]
    command_conditions = tab('\n'.join(conditions), 2)
    unknown_costruct = 'if' if len(commands) == 0 else 'else'

    return sh_help_template.format(all_commands=all_commands, command_conditions=command_conditions, unknown_costruct=unknown_costruct)

def generate_c_parse_command_function(commands: dict):
    command_names = [key for key in commands.keys() if key != '_arguments']
    gen_condition = lambda command_name: get_conditional(get_strcmp_condition('command', f'"{command_name}"'), f'state = sh_handle_{purge_name(command_name)}(words, size);', index=1)

    conditions = [gen_condition(command_name) for command_name in command_names]
    commands_conditions = tab('\n'.join(conditions), 2)

    return sh_parse_command_template.format(commands_conditions=commands_conditions)

def generate_c_file(name: str, commands: dict):
    include = f'{name}.h'
    prompt_symbol = commands.get('promptSymbol') if commands.get('promptSymbol') else '>> '
    help_handler_functions = generate_c_help_handler_functions(commands)
    help_function = generate_c_help_function(commands)
    parse_command_function = generate_c_parse_command_function(commands)
    handle_command_functions = generate_c_handle_command_functions(commands)
    return c_template.format(include=include, prompt_symbol=prompt_symbol, parse_command_function=parse_command_function, help_handler_functions=help_handler_functions, help_function=help_function, handle_command_functions=handle_command_functions)

# Generate handlers header file

def generate_handlers_h_function(command_details: dict):
    function_name = command_details.get('function')
    arguments = command_details.get('arguments')
    arguments = arguments if arguments is not None else {}
    params = get_function_params(arguments)

    return f'SH_STATE {function_name}({params});'

def generate_handlers_h_functions(commands: dict):
    functions = [generate_handlers_h_function(command_details) for _, command_details in commands.items()]
    return '\n'.join(functions)

def generate_handlers_h_file(commands: dict):
    functions = generate_handlers_h_functions(commands)
    return handlers_h_template.format(functions=functions)

# Generate handlers c file

def generate_handlers_c_function(command_name: str, command_details: dict):
    function_name = command_details.get('function')
    arguments = command_details.get('arguments')
    arguments = arguments if arguments is not None else {}
    params = get_function_params(arguments)

    return (
        f'SH_STATE {function_name}({params}) {{\n\t'
        f'printf("{command_name}\\n");\n'
        '}'
    )

def generate_handlers_c_functions(commands: dict):
    functions = [generate_handlers_c_function(command_name, command_details) for command_name, command_details in commands.items()]
    return '\n'.join(functions)

def generate_handlers_c_file(commands: dict):
    functions = generate_handlers_c_functions(commands)
    return handlers_c_template.format(functions=functions)

# Generate main file

def generate_main_file(commands: dict):
    should_handle_arguments = '_arguments' in commands.keys()
    handle_arguments = '/* Handle command line arguments */\n\tsh_handle__arguments(argv, argc);' if should_handle_arguments else ''
    return main_template.format(handle_arguments=handle_arguments)

# Generate sh compiler file

def generate_sh_compiler_file(app_name: str):
    return sh_compiler_template.format(app_name=app_name)

# Exported functions

def generate_scell(app_name: str = 'scell_project', json_commands_path: str = './commands.json', output_path: str = './'):
    APP_NAME = app_name if app_name else 'scell_project'
    JSON_COMMANDS = json_commands_path if json_commands_path else './commands.json'
    OUTPUT_PATH = output_path if output_path else './'

    # Read template files

    with open(HEADER_TEMPLATE) as header_template_file:
        global header_template
        header_template = header_template_file.read()

    with open(C_TEMPLATE) as c_template_file:
        global c_template
        c_template = c_template_file.read()

    with open(SH_PARSE_COMMAND_TEMPLATE) as sh_parse_command_template_file:
        global sh_parse_command_template
        sh_parse_command_template = sh_parse_command_template_file.read()

    with open(SH_HELP_TEMPLATE) as sh_help_template_file:
        global sh_help_template
        sh_help_template = sh_help_template_file.read()

    with open(SH_HELP_HANDLER_TEMPLATE) as sh_help_handler_template_file:
        global sh_help_handler_template
        sh_help_handler_template = sh_help_handler_template_file.read()

    with open(SH_HELP_HANDLER_PARAMETER_TEMPLATE) as sh_help_handler_parameter_template_file:
        global sh_help_handler_parameter_template
        sh_help_handler_parameter_template = sh_help_handler_parameter_template_file.read()

    with open(SH_HANDLE_COMMAND_TEMPLATE) as sh_handle_command_template_file:
        global sh_handle_command_template
        sh_handle_command_template = sh_handle_command_template_file.read()

    with open(SH_HANDLE_COMMAND_CONDITION_TEMPLATE) as sh_handle_command_condition_template_file:
        global sh_handle_command_condition_template
        sh_handle_command_condition_template = sh_handle_command_condition_template_file.read()

    with open(SH_HANDLE_COMMAND_ARRAY_ASSIGNMENT_TEMPLATE) as sh_handle_command_array_assignment_file:
        global sh_handle_command_array_assignment
        sh_handle_command_array_assignment = sh_handle_command_array_assignment_file.read()

    with open(SH_COMPILER_TEMPLATE) as sh_compiler_file:
        global sh_compiler_template
        sh_compiler_template = sh_compiler_file.read()

    with open(HANDLERS_H_TEMPLATE) as handlers_h_template_file:
        global handlers_h_template
        handlers_h_template = handlers_h_template_file.read()

    with open(HANDLERS_C_TEMPLATE) as handlers_c_template_file:
        global handlers_c_template
        handlers_c_template = handlers_c_template_file.read()

    with open(MAIN_TEMPLATE) as main_template_file:
        global main_template
        main_template = main_template_file.read()
        
    # Read json file

    with open(JSON_COMMANDS) as json_commands_file:
        global commands
        commands = json.load(json_commands_file)

    # Generate output directory

    TARGET_PATH = Path(OUTPUT_PATH, APP_NAME)
    TARGET_PATH.mkdir(parents=True, exist_ok=True)

    # Copy shell utils to project dir and create other folders

    SCELL_UTILS_FROM_PATH = Path(__dirname, 'scellutils')
    SCELL_UTILS_TO_PATH = Path(TARGET_PATH, 'scellutils')

    if os.path.exists(SCELL_UTILS_TO_PATH):
        shutil.rmtree(SCELL_UTILS_TO_PATH)
    
    shutil.copytree(SCELL_UTILS_FROM_PATH, SCELL_UTILS_TO_PATH)

    Path(TARGET_PATH, 'scellutils', 'shell_commands').mkdir(parents=True, exist_ok=True)
    Path(TARGET_PATH, 'handlers').mkdir(parents=True, exist_ok=True)

    # Generate header file

    h_output = f'{str(TARGET_PATH)}/scellutils/shell_commands/shell_commands.h'
    with open(h_output, 'w') as h_output_file:
        h_output_file.write(generate_header_file(commands)) 

    # Generate c file
    c_output = f'{str(TARGET_PATH)}/scellutils/shell_commands/shell_commands.c'
    with open(c_output, 'w') as c_output_file:
        c_output_file.write(generate_c_file('shell_commands', commands)) 

    # Generate handlers h file
    handlers_h_output = f'{str(TARGET_PATH)}/handlers/handlers.h'
    with open(handlers_h_output, 'w') as handlers_h_output_file:
        handlers_h_output_file.write(generate_handlers_h_file(commands))

    # Generate handlers c file
    handlers_c_output = f'{str(TARGET_PATH)}/handlers/handlers.c'
    with open(handlers_c_output, 'w') as handlers_c_output_file:
        handlers_c_output_file.write(generate_handlers_c_file(commands))

    # Generate main file
    main_output = f'{str(TARGET_PATH)}/{app_name}.c'
    with open(main_output, 'w') as main_output_file:
        main_output_file.write(generate_main_file(commands))

    # Generate shell compiler file
    sh_compiler_output = f'{str(TARGET_PATH)}/compile.sh'
    with open(sh_compiler_output, 'w') as sh_compiler_file:
        sh_compiler_file.write(generate_sh_compiler_file(app_name))

