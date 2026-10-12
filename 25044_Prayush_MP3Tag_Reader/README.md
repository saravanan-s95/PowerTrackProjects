# MP3 Metadata Editor (ID3v2.3 / v2.4)

A minimal C-based MP3 metadata viewer and editor that supports:

- Reading ID3v2 headers
- Parsing ID3v2.3 and ID3v2.4 frames
- Handling SynchSafe integers (v2.4)
- Editing frame contents safely
- Managing padding correctly
- Preventing tag corruption

---

# 📁 Project Structure


    .
    ├── Assets/
    │ ├── song.mp3
    │ ├── song2.mp3
    │ └── temp.mp3
    ├── edit.c
    ├── edit.h
    ├── helpers.c
    ├── helpers.h
    ├── main.c
    ├── mp3.c
    ├── mp3.h
    ├── render.c
    ├── render.h
    ├── types.h
    ├── view.c
    └── README.md


---

## 🚀 Usage

### 1. View All MP3 Tags

```bash
executable - ./build/MP3_TagReader
```

```bash
executable -v <file.mp3>
```


### 2. Edit a Specific Tag

```bash
executable -e <file.mp3> <flag> <content>
```

## 🏷️ Available Flags

### Flag	Description
    -t	Title
    -a	Artist
    -A	Album
    -y	Year
    -c	Comment
    -C	Composer
## ⚠️ Notes
Use quotes (" ") if the content contains spaces.
Only one tag can be edited per command.
Ensure the file provided is a valid .mp3 file.
## ❓ Help

To display usage instructions from the binary:

```bash
executable -h
```

---





# 🛡️ Safety Checks Used

    Prevent reading beyond tag end

    Validate frame size before allocation

    Detect padding via zeroed tag

    Ensure tag size consistency

    Use unsigned comparisons correctly

# 🧩 Key Takeaways

    ID3v2.3 uses big-endian integers.

    ID3v2.4 uses SynchSafe integers.

    Frame corruption usually comes from:

    Wrong size encoding

    Padding miscalculation

    Mutating header buffers

    Always validate frame size against remaining tag size.
