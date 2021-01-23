#ifndef NT_WIDGET_PAGE_INS_H
#define NT_WIDGET_PAGE_INS_H

#include "w/page.h"
#include "w/vbox.h"
#include "w/hbox.h"
#include "w/text.h"
#include "w/byt.h"

#include "types.h"

namespace w {

struct PageIns : public Page {
  w::Text headerText1_{"HEADER 1"};
  w::Text headerText2_{"POLY:"};
  w::Byt byteEditor_{getInsPoly, setInsPoly};
  w::Widget headerSpacer_;
  w::Text headerText3_{"HEADER 3"};
  w::Widget middleSpacer1_;
  w::Text middleText_{"MIDDLE"};
  w::Widget middleSpacer2_;
  w::Text footerText_{"FOOTER"};
  w::HBox header_;
  w::HBox footer_;
  PageIns() {
    header_.add(&headerText1_);
    header_.add(&headerText2_);
    header_.add(&byteEditor_);
    header_.add(&headerSpacer_);
    header_.add(&headerText3_);
    add(&header_);
    add(&middleSpacer1_);
    add(&middleText_);
    add(&middleSpacer2_);
    footer_.add(&footerText_);
    add(&footer_);
    setCurrentWidget(&byteEditor_);
  }
};

} // namespace w

#endif
