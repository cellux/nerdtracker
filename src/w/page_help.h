#ifndef NT_WIDGET_PAGE_HELP_H
#define NT_WIDGET_PAGE_HELP_H

#include "w/page.h"
#include "w/text.h"

namespace w {

struct PageHelp : public Page {
  w::Text text_{"HELP"};
  PageHelp() {
    add(&text_);
    setCurrentWidget(&text_);
  }
};

} // namespace w

#endif
