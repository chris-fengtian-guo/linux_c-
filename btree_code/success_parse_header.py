import re
import sys

def parse_header(header_filename):
    with open(header_filename, 'r') as file:
        data = file.read()
    print(f"Data read from file: \n{data}\n")

    # 正则表达式匹配成员函数定义
    pattern = re.compile(r'static\s+(.*?)\s+(\w+)\s*\((.*?)\)\s*;', re.MULTILINE | re.DOTALL)
    matches = pattern.findall(data)

    if matches:
        print(f"Matches found: {matches}\n")
    else:
        print("No matches found.\n")

    for return_type, function_name, args in matches:
        arg_list = args.split(',')
        print(f"Return type: {return_type}, Function name: {function_name}, Args: {arg_list}\n")

        # 定义变量
        variables = []
        for i, arg in enumerate(arg_list):
            type_name, var_name = arg.split()
            default_value = '""' if 'string' in type_name else '0'  # 默认字符串为空，其它类型为0
            print(f'{type_name} {var_name} = {default_value};')
            variables.append(var_name)

        # 输出函数调用
        print(f'auto result = Circle::{function_name}({", ".join(variables)});\n')

if len(sys.argv) != 2:
    print("Usage: python parse_header.py <header_filename>")
    sys.exit(1)

parse_header(sys.argv[1])
