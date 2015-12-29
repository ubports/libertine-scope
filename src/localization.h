#ifndef LOCALIZATION_H_
#define LOCALIZATION_H_

#include <libintl.h>
#include <string>


inline char*
_(const char *__msgid)
{
  return dgettext(GETTEXT_PACKAGE, __msgid);
}


inline std::string
_(char const*__msgid1, char const*__msgid2, unsigned long int __n)
{
  char buffer [256];
  if (snprintf(buffer, 256, dngettext(GETTEXT_PACKAGE, __msgid1, __msgid2, __n), __n ) >= 0)
  {
    return buffer;
  }
  else
  {
    return std::string();
  }
}

#endif // LOCALIZATION_H_


