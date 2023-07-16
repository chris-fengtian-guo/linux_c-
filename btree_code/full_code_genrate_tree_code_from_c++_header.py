import xml.etree.ElementTree as ET
import re
import sys

def get_function_call_code(header_filename, function_name):
    with open(header_filename, 'r') as file:
        data = file.read()

    pattern = re.compile(r'static (\w+)\s*\((.*?)\)\s*;', re.MULTILINE | re.DOTALL)
    matches = pattern.findall(data)

    for func_name, args in matches:
        if func_name == function_name:
            arg_list = args.split(',')
            variables = []
            for i, arg in enumerate(arg_list):
                type_name, var_name = arg.split()
                default_value = '""' if 'string' in type_name else '0'  # 默认字符串为空，其它类型为0
                variables.append(var_name)
            return f'DataControl::{func_name}({", ".join(variables)})'
    return None  # 没有找到匹配的函数

def generate_cpp_code_from_bt_xml(bt_xml_filename, header_filename):
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

        function_call_code = get_function_call_code(header_filename, node_id)

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
        if function_call_code:
            print(f'        return {function_call_code};')
        else:
            print(f'        // No matching function found in the header file for "{node_id}"')
        print('    }')
        print('};')
        print('\n')

generate_cpp_code_from_bt_xml('my_tree2.xml', 'class_head.h')
