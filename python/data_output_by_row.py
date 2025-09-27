import numpy as np
from typing import List
import cv2
import os
from pathlib import Path
import re

def natural_sort_key(s: str):
    """自然排序键：按数字和字符串混合排序"""
    return [int(text) if text.isdigit() else text.lower() for text in re.split(r'(\d+)', s)]

def load_and_binarize_images(image_folder: str) -> List[np.ndarray]:
    """加载并二值化指定文件夹中的所有图像"""
    mat_list = []
    image_folder = Path(image_folder)
    
    # 支持的图像格式
    image_extensions = ['.png', '.jpg', '.jpeg', '.bmp', '.tiff', '.tif']
    
    # 获取文件夹中的所有图像文件（避免重复计数）
    image_files = []
    for ext in image_extensions:
        lower_files = list(image_folder.glob(f'*{ext}'))
        upper_files = list(image_folder.glob(f'*{ext.upper()}'))
        
        all_files = set()
        for file in lower_files + upper_files:
            if file.exists():
                all_files.add(file.resolve())
        
        image_files.extend([Path(f) for f in all_files])
    
    # 使用自然排序，而不是单纯按文件名
    image_files.sort(key=lambda x: natural_sort_key(x.name))
    
    if not image_files:
        raise ValueError(f"在文件夹 {image_folder} 中未找到图像文件")
    
    print(f"找到 {len(image_files)} 张图像")
    
    for img_path in image_files:
        print(f"处理图像: {img_path.name}")
        img = cv2.imread(str(img_path), cv2.IMREAD_GRAYSCALE)
        
        if img is None:
            print(f"警告: 无法读取图像 {img_path}")
            continue
        
        if img.shape != (64, 64):
            img = cv2.resize(img, (64, 64))
            print(f"  调整大小: {img.shape} -> 64x64")
        
        _, binary_img = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)
        mat_list.append(binary_img)
    
    return mat_list

def bin2hex(bin_str: str) -> str:
    """二进制倒序转化为十六进制"""
    reversed_bin = bin_str[::-1]
    value = int(reversed_bin, 2)
    return f"0x{value:02X}"

def process_binary_image(mat: np.ndarray) -> List[str]:
    """处理单帧二值图像"""
    strings = []
    builder = []
    length = 0
    
    for i in range(mat.shape[0]):  # 行
        for j in range(mat.shape[1]):  # 列
            if mat[i, j] == 255:
                builder.append("0")
            else:
                builder.append("1")
            
            if length == 7:
                length = -1
                bin_str = ''.join(builder)
                strings.append(bin2hex(bin_str))
                builder = []
            length += 1
    
    return strings

def process_all_frames(mat_list: List[np.ndarray]) -> List[List[str]]:
    """处理所有帧的图像数据"""
    data = []
    for idx, mat in enumerate(mat_list):
        print(f"正在处理第 {idx + 1}/{len(mat_list)} 帧...")
        strings = process_binary_image(mat)
        data.append(strings)
    return data

def print_data(data: List[List[str]]) -> str:
    """格式化输出为C头文件格式"""
    builder = []
    builder.append("#ifndef GIF_H")
    builder.append("#define GIF_H")
    builder.append("")
    builder.append("#include <stdint.h>")
    builder.append("")
    
    max_len = 16
    builder.append(f"static const uint32_t gif_length = {len(data)};")
    builder.append("")
    
    if data:
        bytes_per_frame = len(data[0])
        builder.append(f"static const uint8_t gif[{len(data)}][{bytes_per_frame}] = {{")
        
        for frame_idx, frame_data in enumerate(data):
            frame_builder = []
            frame_builder.append("    {")
            
            for i in range(0, len(frame_data), max_len):
                line_data = frame_data[i:i + max_len]
                line_str = ", ".join(line_data)
                if i + max_len < len(frame_data):
                    frame_builder.append(f"        {line_str},")
                else:
                    frame_builder.append(f"        {line_str}")
            
            if frame_idx == len(data) - 1:
                frame_builder.append("    }")
            else:
                frame_builder.append("    },")
            
            builder.extend(frame_builder)
        
        builder.append("};")
    
    builder.append("")
    builder.append("#endif")
    
    return "\n".join(builder)

def save_to_file(output_str: str, output_path: str = "gif_data.h"):
    """将结果保存到文件"""
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(output_str)
    print(f"结果已保存到: {output_path}")

def main(image_folder: str, output_file: str = "gif_data.h"):
    """主函数：处理指定文件夹中的所有图像"""
    try:
        print("正在加载图像...")
        mat_list = load_and_binarize_images(image_folder)
        
        print("正在处理图像数据...")
        data = process_all_frames(mat_list)
        
        print("正在生成输出...")
        output = print_data(data)
        
        save_to_file(output, output_file)
        
        print(f"\n处理完成!")
        print(f"总帧数: {len(data)}")
        print(f"每帧字节数: {len(data[0]) if data else 0}")
        print(f"总字节数: {len(data) * len(data[0]) if data else 0}")
        
    except Exception as e:
        print(f"处理过程中出现错误: {e}")

if __name__ == "__main__":
    image_folder_path = "./frames_resized_white_binarize"
    output_file_path = "gif_data_row.h"
    main(image_folder_path, output_file_path)
# gif[][512]
# 定义 unsigned char gif[][512] 时，编译器会统计初始化列表中包含多少个完整的 "行"（每个行有 512 个元素）
# 当需要修改数组内容（增加或减少行数）时，不需要手动修改第一维的大小，编译器会自动适配
# 避免因手动计算行数出错导致的数组越界问题


# 若要访问具体元素（如第 i 行第 j 列），必须使用两个索引 gif[i][j]。
# 只使用一个索引 gif[i] 时，得到的是指针，而非元素值，不能直接用于需要具体数值的场景。
# */

# /*
# 64×64 像素的计算过程
# 总像素数：64 × 64 = 4096 个像素
# 总位数：4096 像素 × 1 位 / 像素 = 4096 位
# 转换为字节：4096 位 ÷ 8 位 / 字节 = 512 字节
# 像素64*64/8=512byte
# 1行16byte 共32行 = 512byte