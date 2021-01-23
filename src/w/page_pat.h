#ifndef NT_WIDGET_PAGE_PAT_H
#define NT_WIDGET_PAGE_PAT_H

#include "w/page.h"
#include "w/text.h"

namespace w {

struct PagePat : public Page {
  w::Text text_{"PAT"};
  PagePat() {
    add(&text_);
    setCurrentWidget(&text_);
  }
};

} // namespace w

#endif
