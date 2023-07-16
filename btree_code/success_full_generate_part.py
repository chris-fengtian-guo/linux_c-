import xml.etree.ElementTree as ET
import re
import sys
def parse_header(header_filename):
    with open(header_filename, 'r') as file:
        data = file.read()
    print(f"Data read from file: \n{data}\n")

    function_list = []

    # 正则表达式匹配成员函数定义
    pattern = re.compile(r'static\s+(.*?)\s+(\w+)\s*\((.*?)\)\s*;', re.MULTILINE | re.DOTALL)
    matches = pattern.findall(data)

    if matches:
        print(f"Matches found: {matches}\n")
    else:
        print("No matches found.\n")

    for return_type, function_name, args in matches:
        arg_list = [tuple(arg.split()) for arg in args.split(',')]
        function_list.append((function_name, return_type, arg_list))
        print(f"Return type: {return_type}, Function name: {function_name}, Args: {arg_list}\n")

    return function_list


def generate_cpp_code_from_bt_xml(bt_xml_filename, header_filename):
    tree = ET.parse(bt_xml_filename)
    root = tree.getroot()

    function_list = parse_header(header_filename)

    # 遍历行为树结构
    for action in root.iter('Action'):
        node_id = action.attrib['ID']

        print('    virtual BT::NodeStatus tick() override')
        print('    {')
        function = [f for f in function_list if f[0] == node_id]
        if function:
            function_name, return_type, variables = function[0]
            args = []
            for type_name, var_name in variables:
                default_value = '""' if 'string' in type_name else '0'
                print(f'        {type_name} {var_name} = {default_value};')
                args.append(var_name)
            print(f'        {return_type} result = DataControl::{function_name}({", ".join(args)});')
            print(f'        return result == 0 ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;')
        else:
            print(f'        // No matching function found in the header file for "{node_id}"')
        print('    }')
        print('\n')

generate_cpp_code_from_bt_xml('my_tree2.xml', 'class_head.h')

