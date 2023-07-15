import xml.etree.ElementTree as ET

def generate_cpp_code_from_bt_xml(bt_xml_filename):
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
        print(f'        return DataControl::{node_id}(input_str, output_str);')
        print('    }')
        print('};')
        print('\n')

generate_cpp_code_from_bt_xml('my_tree2.xml')
