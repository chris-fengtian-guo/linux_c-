#!/bin/bash

# 检查参数数量
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

DIR=$1

# 找到所有的 .cpp 和 .h 文件
find $DIR -type f \( -name "*.cpp" -o -name "*.h" \) -print0 | while IFS= read -r -d '' file; do
    # 删除单行注释
    sed -i 's/\/\/.*$//' "$file"
    
    # 删除多行注释
    sed -i ':a; /\/\*/,/\*\// { /.*\*\//!{ $!{ N; ba }; }; s|/\*.*\*/||; }' "$file"
done

echo "注释删除完成"
