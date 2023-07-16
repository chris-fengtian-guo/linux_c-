import xml.etree.ElementTree as ET
import re

def parse_header(header_filename):
    with open(header_filename, 'r') as file:
        data = file.read()

    # 正则表达式匹配成员函数定义
    pattern = re.compile(r'static (\w+)\s*\((.*?)\)\s*;', re.MULTILINE | re.DOTALL)
    matches = pattern.findall(data)

    function_data = []
    if matches:
        for function_name, args in matches:
            arg_list = args.split(',')

            # 定义变量
            variables = []
            for i, arg in enumerate(arg_list):
                type_name, var_name = arg.split()
                variables.append((type_name, var_name))

            function_data.append((function_name, variables))
    print(f"Functions found in the header file: {[f[0] for f in function_data]}")
    return function_data

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
            function_name, variables = function[0]
            args = []
            for type_name, var_name in variables:
                default_value = '""' if 'string' in type_name else '0'
                print(f'        {type_name} {var_name} = {default_value};')
                args.append(var_name)
            print(f'        return DataControl::{function_name}({", ".join(args)});')
        else:
            print(f'        // No matching function found in the header file for "{node_id}"')
        print('    }')
        print('\n')

generate_cpp_code_from_bt_xml('my_tree2.xml', 'class_head.h')
