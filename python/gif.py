from PIL import Image
import os
import cv2
import numpy as np  # 补充这一行
import glob
import re
def gif_separate(original_source, new_path):
    """
    分解 GIF 并保存为 PNG 格式
    :param original_source: 目标 gif 文件路径
    :param new_path: 分解后的文件夹路径
    :return: 分解后的图片帧数（失败返回 -1）
    """
    try:
        # 创建输出文件夹
        os.makedirs(new_path, exist_ok=True)

        # 打开 GIF 文件
        gif = Image.open(original_source)
        frame_count = 0

        while True:
            # 转换为 RGB 格式（防止带透明通道）
            frame = gif.convert("RGBA")
            # 转换成 OpenCV 格式 (PIL -> numpy -> BGR)
            frame_cv = cv2.cvtColor(np.array(frame), cv2.COLOR_RGBA2BGRA)

            # 保存 PNG 文件
            output_path = os.path.join(new_path, f"{frame_count}.png")
            cv2.imwrite(output_path, frame_cv)

            frame_count += 1

            # 读取下一帧
            try:
                gif.seek(gif.tell() + 1)
            except EOFError:
                break

        return frame_count
    except Exception as e:
        print(f"Error: {e}")
        return -1

def natural_sort_key(s):
    """实现自然排序，将字符串中的数字按整数排序"""
    return [int(text) if text.isdigit() else text.lower() for text in re.split(r'(\d+)', s)]

def convert_to_grayscale_with_alpha(input_path, output_path, threshold=5):
    """
    将指定文件夹下的 PNG 图片批量灰度化并保存为带透明背景的 PNG
    :param input_path: 输入文件夹路径
    :param output_path: 输出文件夹路径
    :param threshold: 阈值，小于此灰度值的像素视为透明背景
    :return: 处理的图片数量
    """
    os.makedirs(output_path, exist_ok=True)

    # 使用自然排序，确保帧顺序与 GIF 原始帧一致
    png_files = glob.glob(os.path.join(input_path, "*.png"))
    png_files.sort(key=natural_sort_key)

    print(f"检测到 {len(png_files)} 张 PNG 图片")

    for idx, img_path in enumerate(png_files):
        img = cv2.imread(img_path, cv2.IMREAD_UNCHANGED)

        if img is None:
            print(f"跳过无法读取的图片: {img_path}")
            continue

        # 转为灰度
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # 创建 alpha 通道（背景透明，前景不透明）
        alpha = np.where(gray > threshold, 255, 0).astype(np.uint8)

        # 构建 RGBA 图像（灰度值写入 RGB，Alpha 控制透明度）
        rgba = cv2.merge([gray, gray, gray, alpha])

        # 保持原始帧顺序（使用原文件名，而不是 idx）
        out_file = os.path.join(output_path, os.path.basename(img_path))
        cv2.imwrite(out_file, rgba)

    return len(png_files)

def crop_images(input_path, output_path, start_x=8, start_y=24, width=216, height=216):
    """
    将指定文件夹下的图片批量裁剪为固定矩形区域并保存
    :param input_path: 输入文件夹路径（含图片）
    :param output_path: 输出文件夹路径
    :param start_x: 裁剪起点 X 坐标
    :param start_y: 裁剪起点 Y 坐标
    :param width: 裁剪宽度
    :param height: 裁剪高度
    :return: 处理的图片数量
    """
    os.makedirs(output_path, exist_ok=True)

    # 使用自然排序，确保与 GIF 原始帧一致
    image_files = glob.glob(os.path.join(input_path, "*.png")) + \
                  glob.glob(os.path.join(input_path, "*.jpg"))
    image_files.sort(key=natural_sort_key)

    print(f"检测到 {len(image_files)} 张图片进行裁剪")

    for img_path in image_files:
        img = cv2.imread(img_path, cv2.IMREAD_UNCHANGED)  # 保持通道

        if img is None:
            print(f"跳过无法读取的图片: {img_path}")
            continue

        if img.shape[0] < start_y + height or img.shape[1] < start_x + width:
            print(f"图片尺寸不足，跳过: {img_path}")
            continue

        cropped_img = img[start_y:start_y + height, start_x:start_x + width]

        # 保留原始文件名
        out_file = os.path.join(output_path, os.path.basename(img_path))
        cv2.imwrite(out_file, cropped_img)

    return len(image_files)

def resize_images(input_path, output_path, target_width=64, target_height=64):
    """
    缩放指定文件夹下的所有图片到目标尺寸并保存
    :param input_path: 输入文件夹路径（含图片）
    :param output_path: 输出文件夹路径
    :param target_width: 缩放后的宽度
    :param target_height: 缩放后的高度
    :return: 处理的图片数量
    """
    os.makedirs(output_path, exist_ok=True)

    # 获取所有图片文件（支持 PNG、JPG）
    image_files = sorted(glob.glob(os.path.join(input_path, "*.png")) +
                         glob.glob(os.path.join(input_path, "*.jpg")))

    print(f"检测到 {len(image_files)} 张图片进行缩放")

    for idx, img_path in enumerate(image_files):
        # 读取图片
        img = cv2.imread(img_path, cv2.IMREAD_UNCHANGED)

        if img is None:
            print(f"跳过无法读取的图片: {img_path}")
            continue

        # 缩放
        resized_img = cv2.resize(img, (target_width, target_height), interpolation=cv2.INTER_AREA)

        # 保存
        out_file = os.path.join(output_path, f"{idx}.png")
        cv2.imwrite(out_file, resized_img)

    return len(image_files)

def resize_images_white_bg(input_path, output_path, target_width=64, target_height=64):
    """
    缩放指定文件夹下的所有图片到目标尺寸并将背景设为白色
    :param input_path: 输入文件夹路径
    :param output_path: 输出文件夹路径
    :param target_width: 缩放后的宽度
    :param target_height: 缩放后的高度
    :return: 处理的图片数量
    """
    os.makedirs(output_path, exist_ok=True)

    # 使用自然排序，确保与原始帧顺序一致
    image_files = glob.glob(os.path.join(input_path, "*.png")) + \
                  glob.glob(os.path.join(input_path, "*.jpg"))
    image_files.sort(key=natural_sort_key)

    print(f"检测到 {len(image_files)} 张图片进行缩放并替换背景为白色")

    for img_path in image_files:
        img = cv2.imread(img_path, cv2.IMREAD_UNCHANGED)

        if img is None:
            print(f"跳过无法读取的图片: {img_path}")
            continue

        # 如果是灰度图，则转换成 3 通道
        if len(img.shape) == 2:
            img = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)

        # 如果是带透明通道的 RGBA
        if img.shape[2] == 4:
            b, g, r, a = cv2.split(img)
            alpha = a.astype(float) / 255.0
            white_bg = np.ones_like(img[:, :, :3], dtype=np.uint8) * 255
            img = (white_bg * (1 - alpha[:, :, None]) + img[:, :, :3] * alpha[:, :, None]).astype(np.uint8)

        resized_img = cv2.resize(img, (target_width, target_height), interpolation=cv2.INTER_AREA)

        # 保留原始文件名
        out_file = os.path.join(output_path, os.path.basename(img_path))
        cv2.imwrite(out_file, resized_img)

    return len(image_files)

def binarize_images(input_path, output_path, threshold=128):
    """
    将指定文件夹下的图片进行二值化处理
    :param input_path: 输入文件夹路径
    :param output_path: 输出文件夹路径
    :param threshold: 二值化阈值（0~255）
    :return: 处理的图片数量
    """
    os.makedirs(output_path, exist_ok=True)

    # 使用自然排序，确保与前面处理步骤一致
    image_files = glob.glob(os.path.join(input_path, "*.png")) + \
                  glob.glob(os.path.join(input_path, "*.jpg"))
    image_files.sort(key=natural_sort_key)

    print(f"检测到 {len(image_files)} 张图片进行二值化处理")

    for img_path in image_files:
        img = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)  # 二值化只需要灰度图

        if img is None:
            print(f"跳过无法读取的图片: {img_path}")
            continue

        _, binary_img = cv2.threshold(img, threshold, 255, cv2.THRESH_BINARY)

        # 保留原始文件名
        out_file = os.path.join(output_path, os.path.basename(img_path))
        cv2.imwrite(out_file, binary_img)

    return len(image_files)

# 示例用法
if __name__ == "__main__":
    gif_path = "test.gif"
    gif_break_dir = "./frames/"
    gif_break_gray_dir = "./frames_gray/"  # 灰度化输出路径
    gif_break_gray_crop_dir = "./frames_cropped/" # 裁剪输出路径
    gif_break_gray_crop_scale_dir = "./frames_resized/"   # 缩放后的输出路径
    gif_break_gray_crop_scale_to_white_dir = "./frames_resized_white/"   # 缩放+白底后的输出路径
    gif_break_gray_crop_scale_to_white_binarize_dir = "./frames_resized_white_binarize/"   # 二值化后的输出路径
    frames = gif_separate(gif_path, gif_break_dir)
    print(f"GIF 分解完成，共 {frames} 帧" if frames != -1 else "分解失败")
    # count = convert_to_grayscale(gif_break_dir, gif_break_gray_dir)#bug 背景为黑色
    count = convert_to_grayscale_with_alpha(gif_break_dir, gif_break_gray_dir)
    print(f"灰度化完成，共处理 {count} 张图片")
    count = crop_images(gif_break_gray_dir, gif_break_gray_crop_dir)
    print(f"裁剪完成，共处理 {count} 张图片")
    count = resize_images_white_bg(gif_break_gray_crop_dir, gif_break_gray_crop_scale_to_white_dir)
    print(f"缩放并替换背景完成，共处理 {count} 张图片")
    count = binarize_images(gif_break_gray_crop_scale_to_white_dir, gif_break_gray_crop_scale_to_white_binarize_dir)
    print(f"二值化完成，共处理 {count} 张图片")
