import xml.etree.ElementTree as ET

def generate_cpp_functions_from_bt_xml(bt_xml_filename):
    tree = ET.parse(bt_xml_filename)
    root = tree.getroot()

    print("class DataControl\n{")
    print("public:")

    # 为每个 Action 和 Condition 节点生成函数
    for node_type in ['Action', 'Condition']:
        for node in root.iter(node_type):
            node_id = node.attrib['ID']

            # 生成函数定义
            print(f'    static void {node_id}(std::string input1, std::string input2);')

    print("};")

generate_cpp_functions_from_bt_xml('my_tree2.xml')
