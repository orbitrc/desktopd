#include <stdint.h>
#include <stdbool.h>

/// \brief When to use?
void string_free(char *string);

//==============
// Desktop
//==============

/// \brief Dekstop.NumberOfDesktops property.
int32_t desktop_property_number_of_desktops();

/// \brief Desktop.InsertDesktop method.
bool desktop_method_insert_desktop(int32_t value);

bool desktop_method_remove_desktop(int32_t value);

int32_t desktop_property_current_desktop();

bool desktop_method_set_current_desktop(int32_t value);

//==============
// Appearance
//==============

char* appearance_property_color_scheme();

bool appearance_method_set_color_scheme(const char *value);
