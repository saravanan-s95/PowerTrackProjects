#include "main.h"

/*
 * update_tag_size()
 * -----------------
 * The ID3v2 header (bytes 6-9) stores the TOTAL size of all frames,
 * syncsafe-encoded (7 bits used per byte). Whenever a frame grows or
 * shrinks, this total has to change by the same amount, or any other
 * tool/player reading the file will believe the tag ends somewhere it
 * doesn't (this program's own reader doesn't rely on it -- it just
 * stops at a null frame id -- but other real-world tools do).
 */
static Status update_tag_size(FILE *fp, int delta)
{
    unsigned char size_bytes[4];

    if (fseek(fp, 6, SEEK_SET) != 0)
        return e_failure;

    if (fread(size_bytes, 1, 4, fp) != 4)
        return e_failure;

    unsigned int old_tag_size =
          ((unsigned int)(size_bytes[0] & 0x7f) << 21) |
          ((unsigned int)(size_bytes[1] & 0x7f) << 14) |
          ((unsigned int)(size_bytes[2] & 0x7f) << 7)  |
           (unsigned int)(size_bytes[3] & 0x7f);

    long new_tag_size = (long)old_tag_size + delta;
    if (new_tag_size < 0)
        new_tag_size = 0;

    size_bytes[0] = (new_tag_size >> 21) & 0x7f;
    size_bytes[1] = (new_tag_size >> 14) & 0x7f;
    size_bytes[2] = (new_tag_size >> 7)  & 0x7f;
    size_bytes[3] =  new_tag_size        & 0x7f;

    if (fseek(fp, 6, SEEK_SET) != 0)
        return e_failure;

    if (fwrite(size_bytes, 1, 4, fp) != 4)
        return e_failure;

    return e_success;
}

/*
 * copy_bytes()
 * ------------
 * Copies exactly `count` bytes from the current position of `src` to
 * the current position of `dst`, in chunks (safe for large audio data).
 */
static Status copy_bytes(FILE *src, FILE *dst, long count)
{
    char buffer[4096];

    while (count > 0)
    {
        long chunk = count > (long)sizeof(buffer) ? (long)sizeof(buffer) : count;
        size_t got = fread(buffer, 1, (size_t)chunk, src);

        if (got == 0)
            return e_failure;

        if (fwrite(buffer, 1, got, dst) != got)
            return e_failure;

        count -= (long)got;
    }
    return e_success;
}

/*
 * copy_rest()
 * -----------
 * Copies everything remaining in `src` (from its current position to
 * EOF) into `dst`. Used for the audio data / later frames that come
 * after the frame being edited.
 */
static Status copy_rest(FILE *src, FILE *dst)
{
    char buffer[4096];
    size_t got;

    while ((got = fread(buffer, 1, sizeof(buffer), src)) > 0)
    {
        if (fwrite(buffer, 1, got, dst) != got)
            return e_failure;
    }
    return e_success;
}

Status edit_detail(Mp3Info *mp3info)
{
    if (open_files(mp3info) == e_failure)
    {
        printf("ERROR : File cannot be opened\n");
        return e_failure;
    }
    if (check_head(mp3info) == e_failure)
    {
        printf("ERROR : the file is not of type ID3\n");
        fclose(mp3info->fileptr);
        return e_failure;
    }
    if (check_version(mp3info) == e_failure)
    {
        printf("ERROR : the file is not of version 03 00\n");
        fclose(mp3info->fileptr);
        return e_failure;
    }
    if (check_extension(mp3info) == e_failure)
    {
        printf("ERROR : the file is not of .mp3 extension\n");
        fclose(mp3info->fileptr);
        return e_failure;
    }

    fseek(mp3info->fileptr, 10, SEEK_SET);

    long frame_start = 0;
    Status found = e_failure;

    /* Walk the frames, remembering where the matching frame's header
     * begins, so we can rebuild the file around it. */
    while (1)
    {
        frame_start = ftell(mp3info->fileptr);

        if (read_frame(mp3info) != e_success)
            break;

        if (strcmp(mp3info->frame_id, mp3info->edit_frame) == 0)
        {
            found = e_success;
            break;
        }
    }

    if (found == e_failure)
    {
        printf("ERROR : Frame %s not found in file\n", mp3info->edit_frame);
        fclose(mp3info->fileptr);
        return e_failure;
    }

    /* mp3info->fileptr now sits right after the OLD frame's data,
     * i.e. at the start of whatever comes next (next frame / padding). */
    long after_old_frame   = ftell(mp3info->fileptr);
    unsigned int old_frame_size = mp3info->frame_size;
    unsigned int new_frame_size = (unsigned int)strlen(mp3info->new_data) + 1; /* +1 = encoding byte */

    char temp_name[1024];
    snprintf(temp_name, sizeof(temp_name), "%s.tmp", mp3info->filename);

    FILE *out = fopen(temp_name, "wb");
    if (out == NULL)
    {
        printf("ERROR : Unable to create temporary file\n");
        fclose(mp3info->fileptr);
        return e_failure;
    }

    /* 1. Copy everything BEFORE this frame's header, byte-for-byte. */
    rewind(mp3info->fileptr);
    if (copy_bytes(mp3info->fileptr, out, frame_start) == e_failure)
    {
        printf("ERROR : Failed while copying data before edited frame\n");
        fclose(out);
        fclose(mp3info->fileptr);
        remove(temp_name);
        return e_failure;
    }

    /* 2. Write the new frame header: same id + flags, corrected size. */
    unsigned char size_field[4];
    size_field[0] = (new_frame_size >> 24) & 0xFF;
    size_field[1] = (new_frame_size >> 16) & 0xFF;
    size_field[2] = (new_frame_size >> 8)  & 0xFF;
    size_field[3] =  new_frame_size        & 0xFF;

    fwrite(mp3info->frame_id, 1, 4, out);
    fwrite(size_field, 1, 4, out);
    fwrite(mp3info->frame_flags, 1, 2, out);

    /* 3. Write the new frame data itself: encoding byte + text.
     *    No padding needed -- the frame is sized to fit exactly. */
    unsigned char encoding = 0;
    fwrite(&encoding, 1, 1, out);
    fwrite(mp3info->new_data, 1, strlen(mp3info->new_data), out);

    /* 4. Copy everything AFTER the OLD frame's data, unchanged
     *    (later frames, padding, and all the actual audio data). */
    fseek(mp3info->fileptr, after_old_frame, SEEK_SET);
    if (copy_rest(mp3info->fileptr, out) == e_failure)
    {
        printf("ERROR : Failed while copying data after edited frame\n");
        fclose(out);
        fclose(mp3info->fileptr);
        remove(temp_name);
        return e_failure;
    }

    fclose(out);
    fclose(mp3info->fileptr);

    /* 5. Patch the overall ID3v2 tag size to reflect the size change. */
    int delta = (int)new_frame_size - (int)old_frame_size;

    FILE *fixup = fopen(temp_name, "rb+");
    if (fixup != NULL)
    {
        update_tag_size(fixup, delta);
        fclose(fixup);
    }

    /* 6. Atomically replace the original file with the rebuilt one. */
    if (remove(mp3info->filename) != 0)
    {
        printf("ERROR : Unable to remove original file\n");
        remove(temp_name);
        return e_failure;
    }
    if (rename(temp_name, mp3info->filename) != 0)
    {
        printf("ERROR : Unable to finalize edited file\n");
        return e_failure;
    }

    printf("Edited Successfully\n");
    return e_success;
}

