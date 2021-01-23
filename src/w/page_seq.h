#ifndef NT_WIDGET_PAGE_SEQ_H
#define NT_WIDGET_PAGE_SEQ_H

#include "w/page.h"
#include "w/text.h"

namespace w {

struct PageSeq : public Page {
  w::Text text_{"SEQ"};
  PageSeq() {
    add(&text_);
    setCurrentWidget(&text_);
  }
};

} // namespace w

#endif
