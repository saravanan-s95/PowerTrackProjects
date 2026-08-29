"In my Image Steganography project, I used BMP images because BMP files are uncompressed and store pixel data directly. The LSB technique works by modifying the least significant bits of pixel data, and BMP allows direct access to these bytes. Since changing the LSB causes only a very small change in color values, the image appears visually unchanged.

Other formats such as JPEG use lossy compression. During compression, pixel values are transformed and approximated, which can alter or remove the hidden data. Therefore, BMP is preferred for implementing basic LSB-based steganography because it provides predictable pixel storage and simplifies encoding and decoding."

Use of Magic string encoding:->
A Magic String is a unique identifier hidden inside the image before the actual secret data.
It is used to verify whether the image actually contains data encoded by your steganography program.


Process of an Encoding : ->
"During encoding, the program first validates the input files and checks whether the BMP image has sufficient capacity to store the secret data. It then copies the 54-byte BMP header unchanged to the output image. After that, it encodes a magic string, the secret file extension size, the extension, the secret file size, and finally the secret file data using the Least Significant Bit technique. Each secret bit is embedded into the least significant bit of an image byte. After encoding all data, the remaining image bytes are copied to create the final stego image."


Why encoding all the things except data : ->
"The secret file data alone is not sufficient for successful decoding. Therefore, I also encode metadata such as the magic string, extension size, extension, and file size. The magic string verifies that the image contains valid hidden data, the extension size and extension help reconstruct the original file type, and the file size tells the decoder exactly how much data to extract. This metadata ensures accurate and reliable recovery of the secret file."