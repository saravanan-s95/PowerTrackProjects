
#include <stdio.h>
#include "s2html_event.h"
#include "s2html_conv.h"

static int line_num = 1;
static int start_of_line = 1;
static int line_number_flag = 0;

void set_line_number_flag(int flag)
{
	line_number_flag = flag;
	line_num = 1;
	start_of_line = 1;
}

static void check_and_print_line_num(FILE *fp)
{
	if (line_number_flag && start_of_line)
	{
		fprintf(fp, "%2d ", line_num++);
		start_of_line = 0;
	}
}

static void print_escaped_char(FILE *fp, char c)
{
	check_and_print_line_num(fp);
	if (c == '<')
	{
		fprintf(fp, "&lt;");
	}
	else if (c == '>')
	{
		fprintf(fp, "&gt;");
	}
	else if (c == '&')
	{
		fprintf(fp, "&amp;");
	}
	else
	{
		fputc(c, fp);
	}
	if (c == '\n')
	{
		start_of_line = 1;
	}
}

static void print_escaped_str(FILE *fp, const char *str)
{
	while (*str)
	{
		print_escaped_char(fp, *str++);
	}
}

static void print_span_token(FILE *fp, const char *class_name, const char *data)
{
	check_and_print_line_num(fp);
	fprintf(fp, "<span class=\"%s\">", class_name);
	while (*data)
	{
		print_escaped_char(fp, *data++);
	}
	fprintf(fp, "</span>");
}

/* start_or_end_conv function definitation */
void html_begin(FILE* dest_fp, int type) /* type => not used, but can be used to add differnet HTML tags */
{
	(void)type;
	/* Add HTML begining tags into destination file */
	fprintf(dest_fp, "<!DOCTYPE html>\n");
	fprintf(dest_fp, "<html lang=\"en-US\">\n");
	fprintf(dest_fp, "<head>\n");
	fprintf(dest_fp, "<title>%s</title>\n", "sode2html");
	fprintf(dest_fp, "<meta charset=\"UTF-8\">\n");
	fprintf(dest_fp, "<link rel=\"stylesheet\" href=\"styles.css\">\n");
	fprintf(dest_fp, "</head>\n");
	fprintf(dest_fp, "<body style=\"background-color:lightgrey;\">\n");
	fprintf(dest_fp, "<pre>\n");
}

void html_end(FILE* dest_fp, int type) /* type => not used, but can be used to add differnet HTML tags */
{
	(void)type;
	/* Add HTML closing tags into destination file */
	fprintf(dest_fp, "</pre>\n");
	fprintf(dest_fp, "</body>\n");
	fprintf(dest_fp, "</html>\n");
}

/* sourc_to_html function definitation */
void source_to_html(FILE* fp, pevent_t *event)
{
#ifdef DEBUG
	printf("%s", event->data);
#endif
	switch(event->type)
	{
		case PEVENT_PREPROCESSOR_DIRECTIVE:
			print_span_token(fp, "preprocess_dir", event->data);
			break;
		case PEVENT_MULTI_LINE_COMMENT:
		case PEVENT_SINGLE_LINE_COMMENT:
			print_span_token(fp, "comment", event->data);
			break;
		case PEVENT_STRING:
			print_span_token(fp, "string", event->data);
			break;
		case PEVENT_HEADER_FILE:
			check_and_print_line_num(fp);
			if(event->property == USER_HEADER_FILE)
			{
				fprintf(fp, "<span class=\"header_file\">");
				print_escaped_str(fp, event->data);
				fprintf(fp, "</span>");
			}
			else
			{
				fprintf(fp, "<span class=\"header_file\">&lt;");
				print_escaped_str(fp, event->data);
				fprintf(fp, "&gt;</span>");
			}
			break;
		case PEVENT_REGULAR_EXP:
		case PEVENT_EOF :
			print_escaped_str(fp, event->data);
			break;
		case PEVENT_NUMERIC_CONSTANT:
			print_span_token(fp, "numeric_constant", event->data);
			break;
		case PEVENT_RESERVE_KEYWORD:
			if(event->property == RES_KEYWORD_DATA)
			{
				print_span_token(fp, "reserved_key1", event->data);
			}
			else
			{
				print_span_token(fp, "reserved_key2", event->data);
			}
			break;
		case PEVENT_ASCII_CHAR:
			print_span_token(fp, "ascii_char", event->data);
			break;
		default :
			printf("Unknow event\n");
			break;
	}
}

