from PIL import Image
import struct

def parse_rgba_bin_to_image(file_path, width, height, output_file_path):
    # 创建一个新的RGBA模式的图像对象
    image = Image.new('RGBA', (width, height))
    
    # 打开二进制文件
    with open(file_path, 'rb') as f:
        # 逐像素读取RGBA数据并设置到图像对象中
        for y in range(height):
            for x in range(width):
                # 从文件中读取4个字节（RGBA）
                rgba_bytes = f.read(4)
                # 使用struct模块解包数据为RGBA值
                r, g, b, a = struct.unpack('BBBB', rgba_bytes)
                # 获取图像的像素访问对象
                pixels = image.load()
                # 设置像素值
                pixels[x, y] = (r, g, b, a)
    
    # 保存图像为PNG格式（PNG支持透明度，因此适合RGBA图像）
    image.save(output_file_path, 'PNG')

# 使用示例
width = 1920  # 图像的宽度
height = 1080  # 图像的高度
input_file_path = 'output1.bin'  # 输入的二进制文件路径
output_file_path = 'output_image.png'  # 输出的PNG文件路径

# 解析二进制文件并生成图像
parse_rgba_bin_to_image(input_file_path, width, height, output_file_path)

print(f"图像已保存为 {output_file_path}")
