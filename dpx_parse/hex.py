def hexdump_and_save(file_path, start, end, output_file_path):
    with open(file_path, 'rb') as f_in, open(output_file_path, 'wb') as f_out:
        f_in.seek(start)
        data = f_in.read(end - start + 1)
        f_out.write(data)  # 将读取的数据写入新的二进制文件

        # 以下为可选的hexdump显示部分，用于在控制台输出数据内容
        #print("Offset  Hex        ASCII")
        #for i in range(0, len(data), 16):
            #chunk = data[i:i + 16]
            #hex_data = ' '.join(f'{b:02X}' for b in chunk)
            #ascii_data = ''.join(chr(b) if 32 <= b <= 126 else '.' for b in chunk)
            #print(f'{i + start:08X}  {hex_data:<48}  {ascii_data}')

# 使用示例
hexdump_and_save('input.dpx', 8192, 8302591, 'output.bin')
