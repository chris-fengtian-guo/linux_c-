import xml.etree.ElementTree as ET
import re
import sys

class_name = None

def parse_header(header_filename):
    global class_name
    with open(header_filename, 'r') as file:
        data = file.read()
    print(f"Data read from file: \n{data}\n")

    function_list = []

    # 正则表达式匹配类名
    class_pattern = re.compile(r'class\s+(\w+)', re.MULTILINE | re.DOTALL)
    class_match = class_pattern.search(data)
    if class_match:
        class_name = class_match.group(1)
        print(f"Class name found: {class_name}\n")
    else:
        print("No class name found.\n")

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
    
    function_list = parse_header(header_filename)
   
    tree = ET.parse(bt_xml_filename)
    root = tree.getroot()

    node_dict = {}
    # 先解析 <Nodes> 下的所有节点
    for node in root.find('Nodes'):
        node_id = node.attrib['ID']
        inports = [port.attrib['name'] for port in node.findall('InputPort')]
        outports = [port.attrib['name'] for port in node.findall('OutputPort')]
        node_dict[node_id] = (inports, outports)

    # 遍历行为树结构
    for action in root.iter('Action'):
        node_id = action.attrib['ID']
        inports, outports = node_dict.get(node_id, ([], []))
        
        # 生成代码
        print(f'class {node_id}(BT::SyncActionNode)')
        print('{')
        print('public:')
        print(f'    {node_id}(const std::string& name, const BT::NodeConfiguration& config)')
        print('        : BT::SyncActionNode(name, config)')
        print('    { }')

        print('    static BT::PortsList providedPorts()')
        print('    {')
        print('        return{')
        for port_name in inports:
            print(f'            {"{"}BT::InputPort<std::string>("{port_name}"){"}"},')
        for port_name in outports:
            print(f'            {"{"}BT::OutputPort<std::string>("{port_name}"){"}"},')
        print('        };')
        print('    }')


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
            print(f'        {return_type} result = {class_name}::{function_name}({", ".join(args)});')
            #print(f'        {return_type} result = DataControl::{function_name}({", ".join(args)});')
            print(f'        return result == 0 ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;')
        else:
            print(f'        // No matching function found in the header file for "{node_id}"')
        print('    }')
        print('\n')

generate_cpp_code_from_bt_xml('my_tree2.xml', 'class_head.h')

