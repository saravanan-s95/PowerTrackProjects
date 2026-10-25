#ifndef CURSOR_H
#define CURSOR_H

/* Print cursor navigation menu */
void printCursorMenu();

/* Moving the cursor up */
void move_cursor_up(TextEditor* editor);

/* Moving the cursor down */
void move_cursor_down(TextEditor* editor);

/* Moving the cursor left */
void move_cursor_left(TextEditor* editor);

/* Moving the cursor right */
void move_cursor_right(TextEditor* editor);

/* Start of line */
void start_of_line(TextEditor* editor);

/* End of line */
void end_of_line(TextEditor* editor);

/* Start of text editor */
void start_of_text_editor(TextEditor* editor);

/* End of text editor */
void end_of_text_editor(TextEditor* editor);

#endif