import struct

# Open the binary file in binary mode
with open('infiletest', 'rb') as binary_file:
    # Read the binary data
    binary_data = binary_file.read()

# Unpack the binary data using little-endian format
values = struct.unpack('<' + 'I' * (len(binary_data) // 4), binary_data)

# Print the values, skipping every other entry
with open('output_text_file.txt', 'w') as output_file:
    for i in range(0, len(values), 2):
        output_file.write(f'The LA is {values[i]:02X} and Translated PA is {i * 4}\n')