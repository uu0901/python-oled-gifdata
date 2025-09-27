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

def process_binary_image(mat: np.ndarray) -> List[str]:
    """
    精确匹配OLED页模式的数据组织方式
    数据排列：页0的所有列 → 页1的所有列 → ... → 页7的所有列
    每页包含64列，每列8个像素组成1个字节
    字节内位序：bit0=顶部像素，bit7=底部像素
    像素映射：黑像素(0) → 1，白像素(255) → 0
    """
    hex_data = []
    height, width = mat.shape  # 64x64
    pages = height // 8  # 8页
    
    print(f"图像尺寸: {height}x{width}, 页数: {pages}")
    
    for page in range(pages):
        start_row = page * 8
        
        for col in range(width):
            # 收集当前列的8个像素（从上到下）
            byte_value = 0
            for bit_position in range(8):
                row = start_row + bit_position
                pixel = mat[row, col]
                
                # 黑像素(0)设置对应位为1，白像素(255)为0
                if pixel == 0:  # 注意：这里是0表示黑像素
                    byte_value |= (1 << bit_position)
            
            hex_str = f"0x{byte_value:02X}"
            hex_data.append(hex_str)
    
    # 验证数据长度（8页 × 64列 = 512字节）
    expected_bytes = pages * width
    if len(hex_data) != expected_bytes:
        print(f"警告: 数据长度异常，期望{expected_bytes}字节，实际{len(hex_data)}字节")
    
    return hex_data

def process_all_frames(mat_list: List[np.ndarray]) -> List[List[str]]:
    """处理所有帧的图像数据"""
    data = []
    for idx, mat in enumerate(mat_list):
        print(f"正在处理第 {idx + 1}/{len(mat_list)} 帧...")
        strings = process_binary_image(mat)
        data.append(strings)
        
        # 调试输出：显示前几个字节的值
        if idx == 0:  # 只显示第一帧的前几个字节用于调试
            print("第一帧前16个字节:")
            for i, byte in enumerate(strings[:16]):
                print(f"  Byte {i}: {byte}")
            print("第一帧最后16个字节:")
            for i, byte in enumerate(strings[-16:]):
                print(f"  Byte {512-16+i}: {byte}")
    
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
        builder.append(f"// OLED页模式数据格式：8页×64列=512字节")
        builder.append(f"// 数据排列：页0列0→页0列1→...→页0列63→页1列0→...→页7列63")
        builder.append(f"// 字节内位序：bit0=顶部像素，bit7=底部像素")
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

def create_test_pattern() -> np.ndarray:
    """创建测试图案用于验证数据格式"""
    test_img = np.ones((64, 64), dtype=np.uint8) * 255  # 全白背景
    
    # 添加一些测试图案
    # 1. 左上角8x8黑色方块
    test_img[0:8, 0:8] = 0
    
    # 2. 右上角单个黑色像素
    test_img[0, 63] = 0
    
    # 3. 底部黑色横线
    test_img[63, :] = 0
    
    # 4. 中间垂直线
    test_img[:, 32] = 0
    
    return test_img

def main(image_folder: str, output_file: str = "gif_data.h"):
    """主函数：处理指定文件夹中的所有图像"""
    try:
        print("正在加载图像...")
        mat_list = load_and_binarize_images(image_folder)
        
        # 可选：添加测试图案作为第一帧用于调试
        # test_pattern = create_test_pattern()
        # mat_list.insert(0, test_pattern)
        
        print("正在处理图像数据...")
        data = process_all_frames(mat_list)
        
        print("正在生成输出...")
        output = print_data(data)
        
        save_to_file(output, output_file)
        
        print(f"\n处理完成!")
        print(f"总帧数: {len(data)}")
        print(f"每帧字节数: {len(data[0]) if data else 0} (应为512)")
        print(f"总字节数: {len(data) * len(data[0]) if data else 0}")
        
        # 输出数据格式说明
        print("\n数据格式说明:")
        print("1. 组织方式: 页0的所有列 → 页1的所有列 → ... → 页7的所有列")
        print("2. 每页包含64列，每列8个像素组成1个字节")
        print("3. 字节内位序: bit0=顶部像素，bit7=底部像素")
        print("4. 像素映射: 黑像素(0)→1，白像素(255)→0")
        
    except Exception as e:
        print(f"处理过程中出现错误: {e}")

if __name__ == "__main__":
    image_folder_path = "./frames_resized_white_binarize"
    output_file_path = "gif_data_column_row.h"
    main(image_folder_path, output_file_path)