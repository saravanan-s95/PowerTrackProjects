#ifndef CONTACT_H
#define CONTACT_H
//Creating a stuct name with contact
typedef struct {
    char name[50];//for name 
    char phone[15];//for phone no
    char email[50];//for email id

    int is_favourite;//to mark favorite
    char fav_type[20];//to store the type of fav like->friend,family,others
} Contact;
//create other struct with address book name
typedef struct {
    Contact contacts[100];//declaring contacts with size of 100
    int contactCount;//to follow the count of contacts
} AddressBook;
// ================= MAIN FUNCTIONS =================
void initialize(AddressBook *addressBook);
void createContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void favoritecontacts(AddressBook *addressBook);

// ================= VALIDATION =================
int my_alpha(char *s);
int valid_phone(char *phone);
int validate_email(char *str);

// ================= DUPLICATES =================
int is_duplicate_phone(AddressBook *addressBook, char *phone, int index);
int is_duplicate_email(AddressBook *addressBook, char *email, int index);

// ================= SEARCH =================
void search_contact(char *key, AddressBook *addressBook, int type);
int search_and_select(AddressBook *addressBook);

// ================= CHECKING =================
void sort_structure(AddressBook *addressBook);
int is_empty(AddressBook *addressBook);

// ================= WELCOME =================
void welcome_and_loading();

#endif