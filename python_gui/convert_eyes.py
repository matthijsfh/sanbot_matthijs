def ihex_to_byte_list_with_print(ihex_lines):
    for line in ihex_lines.splitlines():
        if line.startswith(":"):
            record_length = int(line[1:3], 16)
            data = line[9 : 9 + record_length * 2]
            byte_list = [f"0x{data[i:i+2]}" for i in range(0, len(data), 2)]
            print(byte_list)


ihex_dump = """
:10FB7C000000000000000000000000000000000079
:10FB8C000000000000000000000000000000000069
:10FB9C000000000000000000000000000000000059
:10FBAC000000000000000000000000000000000049
:10FBBC000000000000000000000000000000000039
:10FBCC000000000000000000000000000000000029
:10FBDC000000000000000000000000000000000019
:10FBEC000000000000000000000000000000000009
:10FBFC0000000000000000000000000000000000F9
:10FC0C0000000000000000000000000000000000E8
:10FC1C0000000000000000000000000000000000D8
:10FC2C0000000000000000000000000000000000C8
:10FC3C0000000000000000000000000000000000B8
:10FC4C0000000000000000000000000000000000A8
:10FC5C000000000000000000000000000000000098
:10FC6C000000000000000000000000000000000088
:10FC7C000000000000000000000000000000000078
:10FC8C0000000000000000000000003CF0E0C0801C
:10FC9C0000000000000000000080808080808080D8
:10FCAC008080808080808080808080808080808048
:10FCBC000000000000000000000000000000000038
:10FCCC000000000000000000000000000000000028
:10FCDC000000000000000000000000000000000018
:10FCEC000000000000000000000000000000000008
:10FCFC0000000000000000000000000000000000F8
:10FD0C000000000000000000000000000187CFFF91
:10FD1C00FFFFFEFEFEFFFFFFFFFFFFFFFFFFFFFFEA
:10FD2C00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7
:10FD3C00FFFFFFFFFFFFFEFEFEFEFCFCF8F8F8F0F5
:10FD4C00F0E0E0C0C08000000000000000000000F7
:10FD5C000000000000000000000000000000000097
:10FD6C000000000000000000000000000000000087
:10FD7C000000000000000000000000000000000077
:10FD8C00000000000000000000000C1E3F7FFFFF81
:10FD9C00FF7F7F3F3F1F1F0F0F0F07070703030353
:10FDAC000101010101000000000000000000000042
:10FDBC000000000000010101010103030303070718
:10FDCC00070F0F1F1F3F3F7F7EFCF8F0E0C0800045
:10FDDC000000000000000000000000000000000017
:10FDEC000000000000000000000000000000000007
:10FDFC0000000000000000000000000000000000F7
:10FE0C0000000000000000000000000000000001E5
:10FE1C0000000000000000000000000000000000D6
:10FE2C0000000000000000000000000000000000C6
:10FE3C0000000000000000000000000000000000B6
:10FE4C000000000000000000000000010103070793
:10FE5C000C08000000000000000000000000000082
:10FE6C000000000000000000000000000000000086
:10FE7C000000000000000000000000000000000076
:10FE8C000000000000000000000000000000000066
:10FE9C000000000000000000000000000000000056
:10FEAC000000000000000000000000000000000046
:10FEBC000000000000000000000000000000000036
:10FECC000000000000000000000000000000000026
:10FEDC000000000000000000000000000000000016
:10FEEC000000000000000000000000000000000006
:10FEFC0000000000000000000000000000000000F6
:10FF0C0000000000000000000000000000000000E5
:10FF1C0000000000000000000000000000000000D5
:10FF2C0000000000000000000000000000000000C5
:10FF3C0000000000000000000000000000000000B5
:10FF4C0000000000000000000000000000000000A5
:10FF5C000000000000000000000000000000000095
:10FF6C000000000000000000000000000000000085
"""

bytes_list = ihex_to_byte_list_with_print(ihex_dump)
print(bytes_list)
