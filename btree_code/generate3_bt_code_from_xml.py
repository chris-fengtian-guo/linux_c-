import xml.etree.ElementTree as ET

def parse_xml(file):
    tree = ET.parse(file)
    root = tree.getroot()

    nodes = []

    # 记录每个动作节点出现的次数
    action_count = {}

    # 解析条件节点
    for condition in root.iter('Condition'):
        condition_name = condition.get('ID')
        nodes.append(('Condition', condition_name, [], []))

    # 解析动作节点
    for action in root.iter('Action'):
        action_name = action.get('ID')

        # 如果动作节点已出现过，则在其名字后添加计数值
        if action_name in action_count:
            action_count[action_name] += 1
            action_name += str(action_count[action_name])
        else:
            action_count[action_name] = 0

        inputs = []
        outputs = []
        
        # 解析输入端口
        for input_port in action.iter('InputPort'):
            inputs.append(input_port.get('name'))

        # 解析输出端口
        for output_port in action.iter('OutputPort'):
            outputs.append(output_port.get('name'))

        nodes.append(('Action', action_name, inputs, outputs))

    return nodes

def generate_cpp(nodes):
    for node_type, node_name, inputs, outputs in nodes:
        if node_type == 'Condition':
            parent_class = 'BT::ConditionNode'
        elif node_type == 'Action':
            parent_class = 'BT::SyncActionNode'
        else:
            continue

        print(f"class {node_name} : public {parent_class}")
        print("{")
        print("public:")
        print(f"    {node_name}(const std::string& name, const BT::NodeConfiguration& config)")
        print("        : BT::SyncActionNode(name, config)")
        print("    {}")
        print()
        print("    BT::NodeStatus tick() override")
        print("    {")
        print("        std::string input_str, output_str;")
        print(f"        return DataControl::{node_name}(input_str, output_str);")
        print("    }")
        print()
        print("    static BT::PortsList providedPorts()")
        print("    {")
        print("        return{")
        for input_port in inputs:
            print(f"            BT::InputPort<std::string>(\"{input_port}\"),")
        for output_port in outputs:
            print(f"            BT::OutputPort<std::string>(\"{output_port}\"),")
        print("        };")
        print("    }")
        print("};")
        print()

if __name__ == "__main__":
    nodes = parse_xml('my_tree.xml')
    generate_cpp(nodes)
